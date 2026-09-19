/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2022 OpenCFD Ltd.
    Copyright (C) 2026 Keysight Technologies
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "IntegralScaleBox.H"
#include "cartesianCS.H"
#include "OBJstream.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class Type>
const Foam::Enum
<
    typename Foam::turbulence::IntegralScaleBox<Type>::kernelType
>
Foam::turbulence::IntegralScaleBox<Type>::kernelTypeNames
({
    { kernelType::GAUSSIAN, "Gaussian" },
    { kernelType::EXPONENTIAL , "exponential" }
});


template<class Type>
int Foam::turbulence::IntegralScaleBox<Type>::debug = 0;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
Foam::autoPtr<Foam::coordinateSystem>
Foam::turbulence::IntegralScaleBox<Type>::calcCoordinateSystem
(
    const dictionary& dict
) const
{
    return coordinateSystem::NewIfPresent(dict);
}


template<class Type>
void Foam::turbulence::IntegralScaleBox<Type>::calcCoordinateSystem()
{
    // Get patch normal direction into the domain
    const vector nf((-gAverage(p_.nf())).normalise());

    // Find the second local coordinate direction
    direction minCmpt = 0;
    scalar minMag = mag(nf[minCmpt]);
    for (direction cmpt = 1; cmpt < pTraits<vector>::nComponents; ++cmpt)
    {
        const scalar s = mag(nf[cmpt]);
        if (s < minMag)
        {
            minMag = s;
            minCmpt = cmpt;
        }
    }

    // Create the second local coordinate direction
    vector e2(Zero);
    e2[minCmpt] = 1;

    // Remove normal component
    e2 -= (nf&e2)*nf;

    // Create the local coordinate system - default e3-e1 order
    csysPtr_.reset
    (
        new coordSystem::cartesian
        (
            Zero,   // origin
            nf^e2,  // e3
            nf      // e1
        )
    );
}


template<class Type>
Foam::scalar Foam::turbulence::IntegralScaleBox<Type>::gaussHash
(
    const label dir,
    const label i1,
    const label i2,
    const label i3
) const
{
    // Counter-based deterministic standard-normal generator.
    // The output depends only on (seed_, dir, i1, i2, i3), so the same logical
    // cell/component gets the same value on every rank.

    constexpr auto splitmix64_golden_ratio = std::uint64_t{0x9E3779B97F4A7C15};
    constexpr auto splitmix64_multiplier1 = std::uint64_t{0xBF58476D1CE4E5B9};
    constexpr auto splitmix64_multiplier2 = std::uint64_t{0x94D049BB133111EB};

    const auto mix = [&](uint64_t x) noexcept -> uint64_t
    {
        // splitmix64 finaliser. Unsigned overflow is intentional and defined.
        x += splitmix64_golden_ratio;
        x = (x ^ (x >> 30))*splitmix64_multiplier1;
        x = (x ^ (x >> 27))*splitmix64_multiplier2;
        return x ^ (x >> 31);
    };

    const auto toUint64 = [&](const label x) noexcept -> uint64_t
    {
        // Preserve the full label width.
        // If label is signed, conversion to uint64_t is well-defined
        // modulo 2^64.
        return static_cast<uint64_t>(x);
    };

    const auto hashCombine =
        [&](uint64_t h, const uint64_t v) noexcept -> uint64_t
    {
        // Similar spirit to boost::hash_combine, but using splitmix64
        // to avalanche each field and avoid simple additive structure.
        h ^= mix(v + splitmix64_golden_ratio + (h << 6) + (h >> 2));
        return mix(h);
    };

    // Seed modifiers for specific input dimensions
    constexpr auto salt_dir = std::uint64_t{0x01D1};
    constexpr auto salt_i1 = std::uint64_t{0x11A1};
    constexpr auto salt_i2 = std::uint64_t{0x22B2};
    constexpr auto salt_i3 = std::uint64_t{0x33C3};

    uint64_t key = mix(static_cast<uint64_t>(seed_));

    key = hashCombine(key, salt_dir ^ toUint64(dir));
    key = hashCombine(key, salt_i1 ^ toUint64(i1));
    key = hashCombine(key, salt_i2 ^ toUint64(i2));
    key = hashCombine(key, salt_i3 ^ toUint64(i3));

    // Alternating bitmask constants used for Box-Muller stream decorrelation
    constexpr auto decorrelation_mask_a = std::uint64_t{0xA5A5A5A5A5A5A5A5};
    constexpr auto decorrelation_mask_b = std::uint64_t{0x5A5A5A5A5A5A5A5A};

    // Generate two decorrelated 64-bit hashes for Box-Muller.
    const uint64_t h1 = mix(key ^ decorrelation_mask_a);
    const uint64_t h2 = mix(key ^ decorrelation_mask_b);

    const auto uniformOpen01 = [&](const uint64_t h) -> double
    {
        // Use the top 53 bits, suitable for double precision.
        //
        // Map k in [0, 2^53 - 1] to:
        //
        //     (k + 1) / (2^53 + 2)
        //
        // This avoids both 0 and 1, so log(u1) is always finite and negative.
        constexpr double denom = 9007199254740994.0; // 2^53 + 2
        const uint64_t k = h >> 11;

        return (static_cast<double>(k) + 1.0)/denom;
    };

    const double u1 = uniformOpen01(h1);
    const double u2 = uniformOpen01(h2);

    const double z =
        Foam::sqrt(-2.0*Foam::log(u1))
       *Foam::cos(static_cast<double>(constant::mathematical::twoPi)*u2);

    return static_cast<scalar>(z);
}


template<class Type>
void Foam::turbulence::IntegralScaleBox<Type>::calcOwnership()
{
    const label ny = n_.y();

    // Default: own nothing
    j0_ = 0;
    j1_ = 0;

    if (ny <= 0)
    {
        return;
    }

    const scalar dz = delta_.y();

    if (dz <= SMALL)
    {
        FatalErrorInFunction
            << "Invalid generation-plane spacing delta_.y() = " << dz
            << ". Expected a strictly positive value."
            << exit(FatalError);
    }

    // This rank's e3-row footprint on the generation plane.
    // Empty default for ranks without local patch faces.
    label fpMin = ny;
    label fpMax = 0;

    if (p_.size() > 0)
    {
        const pointField localPos
        (
            csysPtr_->localPosition
            (
                pointField
                (
                    p_.patch().points(),
                    p_.patch().meshPoints()
                )
            )
        );

        if (localPos.size() > 0)
        {
            scalar zmin = GREAT;
            scalar zmax = -GREAT;

            for (const point& pt : localPos)
            {
                zmin = Foam::min(zmin, pt.z());
                zmax = Foam::max(zmax, pt.z());
            }

            const scalar invDz = scalar(1)/dz;

            const scalar rawMin = (zmin - boundingBoxMin_[2])*invDz;

            const scalar rawMax = (zmax - boundingBoxMin_[2])*invDz;

            if
            (
                !std::isfinite(static_cast<double>(rawMin))
             || !std::isfinite(static_cast<double>(rawMax))
            )
            {
                FatalErrorInFunction
                    << "Non-finite footprint coordinates: rawMin = "
                    << rawMin << ", rawMax = " << rawMax
                    << exit(FatalError);
            }

            // Small tolerance to avoid off-by-one changes when a coordinate
            // lies very close to a generation-plane row boundary.
            const scalar tol = scalar(10)*SMALL;

            const auto clampFloorToRow = []
            (
                const scalar x,
                const scalar ny,
                const scalar tol
            ) -> label
            {
                const scalar y = x + tol;

                if (y <= scalar(0))
                {
                    return label(0);
                }

                if (y >= scalar(ny - 1))
                {
                    return ny - 1;
                }

                return label(std::floor(y));
            };

            auto clampCeilToRowEnd = []
            (
                const scalar x,
                const scalar ny,
                const scalar tol
            ) -> label
            {
                const scalar y = x - tol;

                if (y <= scalar(0))
                {
                    return label(0);
                }

                if (y >= scalar(ny))
                {
                    return ny;
                }

                return label(std::ceil(y));
            };

            fpMin = clampFloorToRow(rawMin, ny, tol);
            fpMax = clampCeilToRowEnd(rawMax, ny, tol);

            // Guarantee a non-empty footprint for ranks with faces.
            fpMax = Foam::max(fpMin + 1, fpMax);
            fpMax = Foam::min(fpMax, ny);
        }
    }

    // Gather all footprints.
    List<labelPair> allLimits(UPstream::nProcs());
    allLimits[UPstream::myProcNo()] = labelPair(fpMin, fpMax);

    Pstream::allGatherList(allLimits);

    // Participating ranks: ranks with non-empty footprints.
    DynamicList<label> parts;

    forAll(allLimits, proci)
    {
        const auto& [minVal, maxVal] = allLimits[proci];
        if (minVal < maxVal)
        {
            parts.push_back(proci);
        }
    }

    label m = parts.size();

    if (m == 0)
    {
        // Degenerate fallback: no rank contributed a non-empty footprint.
        //
        // Keep execution alive with an explicit, deterministic ownership
        // policy shared by all ranks: the master owns the full plane.
        const label fallbackOwner = UPstream::masterNo();

        allLimits[fallbackOwner] = labelPair(0, ny);
        parts.push_back(fallbackOwner);
        m = 1;

        if (UPstream::master())
        {
            WarningInFunction
                << "No non-empty patch footprint was found on any rank. "
                << "Falling back to single-rank ownership on rank "
                << fallbackOwner << " for generation-plane rows [0, " << ny
                << ")."
                << endl;
        }
    }

    // Footprint centre for ordering and slab boundary construction.
    labelList centre(UPstream::nProcs(), label(0));

    for (const label proci : parts)
    {
        const auto& [minVal, maxVal] = allLimits[proci];

        centre[proci] = minVal + (maxVal - minVal)/2;
    }

    // Deterministic spatial ordering.
    //
    // Tie-breaks matter: if two ranks have the same footprint start or centre,
    // the ownership must still be reproducible.
    Foam::sort
    (
        parts,
        [&](const label a, const label b)
        {
            if (centre[a] != centre[b])
            {
                return centre[a] < centre[b];
            }

            const auto& [minVal_a, maxVal_a] = allLimits[a];
            const auto& [minVal_b, maxVal_b] = allLimits[b];

            if (minVal_a != minVal_b)
            {
                return minVal_a < minVal_b;
            }

            if (maxVal_a != maxVal_b)
            {
                return maxVal_a < maxVal_b;
            }

            return a < b;
        }
    );

    labelList start(m, label(0));
    labelList end(m, label(0));

    if (m <= ny)
    {
        // Tile [0, ny) into m non-empty contiguous slabs.
        //
        // Boundaries are placed halfway between neighbouring footprint centres,
        // then clamped so every participating rank gets at least one row.
        start[0] = 0;

        for (label k = 0; k < m - 1; ++k)
        {
            const label a = parts[k];
            const label b = parts[k + 1];

            // Overflow-safe midpoint, also valid if centres are equal.
            label bnd = centre[a] + (centre[b] - centre[a])/2;

            // Enforce non-empty slabs:
            //
            // - current slab must contain at least one row
            // - enough rows must remain for the remaining ranks
            const label lo = start[k] + 1;
            const label hi = ny - (m - k - 1);

            bnd = Foam::max(lo, Foam::min(bnd, hi));

            end[k] = bnd;
            start[k + 1] = bnd;
        }

        end[m - 1] = ny;
    }
    else
    {
        // There are more participating ranks than rows.
        // It is mathematically impossible for every participant to own a
        // non-empty slab, so distribute rows deterministically in sorted order
        //
        // Some ranks will get [j, j), i.e. no ownership.
        for (label k = 0; k < m; ++k)
        {
            start[k] = label
            (
                (
                    static_cast<long double>(k)
                   *static_cast<long double>(ny)
                )
               /static_cast<long double>(m)
            );

            end[k] = label
            (
                (
                    static_cast<long double>(k + 1)
                   *static_cast<long double>(ny)
                )
               /static_cast<long double>(m)
            );
        }
    }

    // Assign this rank's ownership interval.
    forAll(parts, k)
    {
        if (parts[k] == UPstream::myProcNo())
        {
            j0_ = start[k];
            j1_ = end[k];
            break;
        }
    }
}


template<class Type>
Foam::Vector2D<Foam::vector>
Foam::turbulence::IntegralScaleBox<Type>::calcBoundBox() const
{
    // Convert patch points into local coordinate system
    const pointField localPos
    (
        csysPtr_->localPosition
        (
            pointField
            (
                p_.patch().points(),
                p_.patch().meshPoints()
            )
        )
    );

    // Calculate bounding-box span and min
    const bool globalReduction = true;
    const boundBox bb(localPos, globalReduction);

    return Vector2D<vector>(bb.span(), bb.min());
}


template<class Type>
Foam::Vector2D<Foam::scalar>
Foam::turbulence::IntegralScaleBox<Type>::calcDelta() const
{
    return Vector2D<scalar>
    (
        boundingBoxSpan_[1]/n_.x(),
        boundingBoxSpan_[2]/n_.y()
    );
}


template<class Type>
Foam::labelList Foam::turbulence::IntegralScaleBox<Type>::calcSpans() const
{
    labelList spans(pTraits<TypeL>::nComponents, label(1));

    // e3 extent is restricted to this rank's owned plane rows; the streamwise
    // (e1) and e2 extents remain full so the box is a distributed slab
    const Vector<label> slice(label(1), n_.x(), j1_ - j0_);
    const TypeL L(convert(L_));

    label j = 0;
    if (fsm_)
    {
        j = pTraits<Type>::nComponents;
    }

    for (label i = j; i < pTraits<TypeL>::nComponents; ++i)
    {
        const label slicei = label(i/pTraits<Type>::nComponents);

        const label n = ceil(L[i]);
        const label twiceN = 4*n;

        spans[i] = slice[slicei] + twiceN;
    }

    return spans;
}


template<class Type>
Foam::scalarListList
Foam::turbulence::IntegralScaleBox<Type>::calcKernel() const
{
    scalarListList kernel
    (
        pTraits<TypeL>::nComponents,
        scalarList(1, scalar(1))
    );

    const TypeL L(convert(L_));

    label i = 0;
    if (fsm_)
    {
        i = pTraits<Type>::nComponents;
    }

    for (direction dir = i; dir < pTraits<TypeL>::nComponents; ++dir)
    {
        // The smallest kernel width larger than integral scale
        // (KSJ:'n' in Eq. 15)
        const label n = ceil(L[dir]);

        // Full kernel-width (except mid-zero) according to the condition
        // (KSJ:Eq. 15 whereat N is minimum = 2n)
        const label twiceN = 4*n;

        // Initialise kernel-coeffs containers with full kernel-width size
        // Extra elem is mid-zero within [-N, N]
        kernel[dir] = scalarList(twiceN + 1, Zero);

        // First element: -N within [-N, N]
        const scalar initElem = -2*scalar(n);

        // Container initialised with [-N, N] (KSJ:p. 658, item-b)
        std::iota
        (
            kernel[dir].begin(),
            kernel[dir].end(),
            initElem
        );

        // Compute kernel coefficients (KSJ:Eq. 14 (Gaussian))
        scalarList kTemp(kernel[dir]);
        scalar kSum = 0;

        // Model constant shaping the autocorrelation function (KSJ:Eq. 14)
        const scalar C = -0.5*constant::mathematical::pi;

        if (kernelType_)
        {
            const scalar nSqr = n*n;

            kTemp = sqr(Foam::exp(C*sqr(kTemp)/nSqr));
            kSum = Foam::sqrt(sum(kTemp));

            kernel[dir] = Foam::exp(C*sqr(kernel[dir])/nSqr)/kSum;
        }
        else
        {
            kTemp = sqr(Foam::exp(C*mag(kTemp)/n));
            kSum = Foam::sqrt(sum(kTemp));

            kernel[dir] = Foam::exp(C*mag(kernel[dir])/n)/kSum;
        }
    }

    return kernel;
}


template<class Type>
Foam::scalarListList Foam::turbulence::IntegralScaleBox<Type>::calcBox()
{
    scalarListList box(pTraits<Type>::nComponents, scalarList());

    constexpr label nComp3 = pTraits<TypeL>::nComponents/3;

    // Initialise: Remaining convenience factors for (e1 e2 e3)
    for (direction dir = 0; dir < pTraits<Type>::nComponents; ++dir)
    {
        scalarList& randomSet = box[dir];

        const label sz1 = spans_[dir];
        const label sz2 = spans_[dir + nComp3];
        const label sz3 = spans_[dir + 2*nComp3];

        randomSet = scalarList(sz1*sz2*sz3);

        if (randomSet.size() > 1e8)
        {
            WarningInFunction
                << "Size of random-number set is relatively high:" << nl
                << "    size = " << randomSet.size() << nl
                << "    Please consider to use the forward-stepwise method."
                << endl;
        }

        // Initialise the integral-scale box with a deterministic,
        // index-addressable standard-normal field. The flat layout is
        // streamwise (i1) outermost, then e3 (i3) then e2 (i2). The e3 index
        // is offset by j0_ so the global cell index is used in the hash,
        // making overlapping halo cells identical across ranks.
        const label sliceSpan = sz2*sz3;
        for (label i1 = 0; i1 < sz1; ++i1)
        {
            for (label i3 = 0; i3 < sz3; ++i3)
            {
                for (label i2 = 0; i2 < sz2; ++i2)
                {
                    randomSet[i1*sliceSpan + i3*sz2 + i2] =
                        gaussHash(dir, i1, i2, j0_ + i3);
                }
            }
        }
    }

    return box;
}


template<class Type>
Foam::pointField
Foam::turbulence::IntegralScaleBox<Type>::calcPatchPoints() const
{
    // Build only the vertices of this rank's owned plane slab [j0_, j1_)
    const label nx = n_.x();
    const label nyl = j1_ - j0_;

    if (nyl <= 0)
    {
        return pointField();
    }

    // List of vertex points of the virtual patch in local coordinate system.
    // Every point uses the global (j) index formula so neighbouring ranks'
    // slab edges coincide exactly with no cracks.
    const label nPoints = (nx + 1)*(nyl + 1);
    pointField points(nPoints, Zero);

    label pointi = 0;
    for (label j = j0_; j <= j1_; ++j)
    {
        for (label i = 0; i <= nx; ++i)
        {
            const point p
            (
                boundingBoxMin_[0],
                boundingBoxMin_[1] + i*delta_.x(),
                boundingBoxMin_[2] + j*delta_.y()
            );
            points[pointi] = p;
            ++pointi;
        }
    }

    points = csysPtr_->globalPosition(points);

    return points;
}


template<class Type>
Foam::faceList Foam::turbulence::IntegralScaleBox<Type>::calcPatchFaces() const
{
    // Faces of this rank's owned plane slab, ordered e3 (local row) outer,
    // e2 inner - matching the output ordering of convolve()
    const label nx = n_.x();
    const label nyl = j1_ - j0_;

    if (nyl <= 0)
    {
        return faceList();
    }

    const label nFaces = nx*nyl;
    faceList faces(nFaces);

    label m = 0;
    for (label j = 0; j < nyl; ++j)
    {
        for (label i = 0; i < nx; ++i)
        {
            const label k = j*(nx+1) + i;
            faces[m] = face({k, k+(nx+1), k+(nx+2), k+1});
            ++m;
        }
    }

    return faces;
}


template<class Type>
void Foam::turbulence::IntegralScaleBox<Type>::calcPatch()
{
    if (debug && !patchFaces_.empty())
    {
        const auto& tm = p_.patch().boundaryMesh().mesh().time();
        OBJstream os
        (
            tm.path()
           /("patch_proc" + Foam::name(UPstream::myProcNo()) + ".obj")
        );
        os.write(patchFaces_, patchPoints_, false);
    }

    if (!patchPtr_)
    {
        patchPtr_.reset
        (
            new primitivePatch
            (
                SubList<face>(patchFaces_),
                patchPoints_
            )
        );
    }
}


template<class Type>
typename Foam::turbulence::IntegralScaleBox<Type>::TypeL
Foam::turbulence::IntegralScaleBox<Type>::convert
(
    const typename Foam::turbulence::IntegralScaleBox<Type>::TypeL& L
) const
{
    TypeL Ls(L);

    const scalar deltaT =
        p_.patch().boundaryMesh().mesh().time().deltaTValue();

    for (direction dir = 0; dir < pTraits<Type>::nComponents; ++dir)
    {
        //  (KSJ:Eq. 13)
        // Integral time scales
        Ls[dir] /= deltaT;
        // Integral length scales
        Ls[dir + Ls.size()/3] /= delta_.x();
        Ls[dir + 2*(Ls.size()/3)] /= delta_.y();
    }

    return Ls;
}


template<class Type>
Foam::scalar Foam::turbulence::IntegralScaleBox<Type>::calcC1
(
    const vector& L
) const
{
    constexpr scalar c1 = -0.25*constant::mathematical::pi;
    return Foam::exp(c1/L.x());
}


template<class Type>
Foam::vector Foam::turbulence::IntegralScaleBox<Type>::calcC1
(
    const tensor& L
) const
{
    constexpr scalar c1 = -0.25*constant::mathematical::pi;

    vector C1(Zero);
    forAll(C1, i)
    {
        C1[i] = Foam::exp(c1/L.x()[i]);
    }

    return C1;
}


template<class Type>
Foam::scalar Foam::turbulence::IntegralScaleBox<Type>::calcC2
(
    const vector& L
) const
{
    constexpr scalar c2 = -0.5*constant::mathematical::pi;
    return Foam::sqrt(scalar(1) - Foam::exp(c2/L.x()));
}


template<class Type>
Foam::vector Foam::turbulence::IntegralScaleBox<Type>::calcC2
(
    const tensor& L
) const
{
    constexpr scalar c2 = -0.5*constant::mathematical::pi;

    vector C2(Zero);
    forAll(C2, i)
    {
        C2[i] = Foam::sqrt(scalar(1) - Foam::exp(c2/L.x()[i]));
    }

    return C2;
}


template<class Type>
void Foam::turbulence::IntegralScaleBox<Type>::updateC1C2()
{
    if (p_.patch().boundaryMesh().mesh().time().isAdjustTimeStep())
    {
        C1_ = calcC1(convert(L_));
        C2_ = calcC2(convert(L_));
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::turbulence::IntegralScaleBox<Type>::IntegralScaleBox
(
    const fvPatch& p
)
:
    p_(p),
    patchPtr_(nullptr),
    csysPtr_(nullptr),
    kernelType_(kernelType::GAUSSIAN),
    n_(Zero),
    delta_(Zero),
    boundingBoxSpan_(Zero),
    boundingBoxMin_(Zero),
    L_(Zero),
    seed_(0),
    j0_(0),
    j1_(0),
    startTimeIndex_(-1),
    spans_(Zero),
    box_(Zero),
    kernel_(Zero),
    patchPoints_(Zero),
    patchFaces_(Zero),
    fsm_(false),
    C1_(Zero),
    C2_(Zero),
    slice_(Zero)
{}


template<class Type>
Foam::turbulence::IntegralScaleBox<Type>::IntegralScaleBox
(
    const fvPatch& p,
    const IntegralScaleBox& b
)
:
    p_(p),
    patchPtr_(nullptr),
    csysPtr_(b.csysPtr_.clone()),
    kernelType_(b.kernelType_),
    n_(b.n_),
    delta_(b.delta_),
    boundingBoxSpan_(b.boundingBoxSpan_),
    boundingBoxMin_(b.boundingBoxMin_),
    L_(b.L_),
    seed_(b.seed_),
    j0_(b.j0_),
    j1_(b.j1_),
    startTimeIndex_(b.startTimeIndex_),
    spans_(b.spans_),
    box_(b.box_),
    kernel_(b.kernel_),
    patchPoints_(b.patchPoints_),
    patchFaces_(b.patchFaces_),
    fsm_(b.fsm_),
    C1_(b.C1_),
    C2_(b.C2_),
    slice_(b.slice_)
{}


template<class Type>
Foam::turbulence::IntegralScaleBox<Type>::IntegralScaleBox
(
    const fvPatch& p,
    const dictionary& dict
)
:
    p_(p),
    patchPtr_(nullptr),
    csysPtr_(calcCoordinateSystem(dict)),
    kernelType_
    (
        kernelTypeNames.getOrDefault
        (
            "kernelType",
            dict,
            kernelType::GAUSSIAN
        )
    ),
    n_(dict.get<Vector2D<label>>("n")),
    delta_(Zero),
    boundingBoxSpan_(Zero),
    boundingBoxMin_(Zero),
    L_(dict.get<TypeL>("L")),
    seed_(dict.getOrDefault<label>("seed", label(time(0)))),
    j0_(0),
    j1_(0),
    startTimeIndex_(-1),
    spans_(Zero),
    box_(Zero),
    kernel_(Zero),
    patchPoints_(Zero),
    patchFaces_(Zero),
    fsm_(dict.getOrDefault("fsm", false)),
    C1_(Zero),
    C2_(Zero),
    slice_(Zero)
{
    if (cmptMin(L_) < ROOTVSMALL)
    {
        FatalIOErrorInFunction(dict)
            << "Integral scale set contains a very small input" << nl
            << "    L = " << L_
            << exit(FatalIOError);
    }

    if (Foam::min(n_.x(), n_.y()) <= 0)
    {
        FatalIOErrorInFunction(dict)
            << "Number of faces on box inlet plane has non-positive input"
            << "    n = " << n_
            << exit(FatalIOError);
    }
}


template<class Type>
Foam::turbulence::IntegralScaleBox<Type>::IntegralScaleBox
(
    const IntegralScaleBox& b
)
:
    p_(b.p_),
    patchPtr_(nullptr),
    csysPtr_(b.csysPtr_.clone()),
    kernelType_(b.kernelType_),
    n_(b.n_),
    delta_(b.delta_),
    boundingBoxSpan_(b.boundingBoxSpan_),
    boundingBoxMin_(b.boundingBoxMin_),
    L_(b.L_),
    seed_(b.seed_),
    j0_(b.j0_),
    j1_(b.j1_),
    startTimeIndex_(b.startTimeIndex_),
    spans_(b.spans_),
    box_(b.box_),
    kernel_(b.kernel_),
    patchPoints_(b.patchPoints_),
    patchFaces_(b.patchFaces_),
    fsm_(b.fsm_),
    C1_(b.C1_),
    C2_(b.C2_),
    slice_(b.slice_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void Foam::turbulence::IntegralScaleBox<Type>::initialise()
{
    if (!csysPtr_)
    {
        calcCoordinateSystem();
    }

    if (debug && csysPtr_)
    {
        Info<< "Local coordinate system:" << nl
            << "    - origin        = " << csysPtr_->origin() << nl
            << "    - e1-axis       = " << csysPtr_->e1() << nl
            << "    - e2-axis       = " << csysPtr_->e2() << nl
            << "    - e3-axis       = " << csysPtr_->e3() << nl << endl;
    }

    {
        const Vector2D<vector> bb(calcBoundBox());

        boundingBoxSpan_ = bb.x();

        boundingBoxMin_ = bb.y();
    }

    delta_ = calcDelta();

    // Broadcast a single global seed so the deterministic random field is
    // identical on every rank (covers a time(0) default that may differ)
    Pstream::broadcast(seed_);

    // Record the start time index for restart-safe streamwise indexing
    startTimeIndex_ = p_.patch().boundaryMesh().mesh().time().timeIndex();

    // Determine this rank's disjoint owned plane-row slab (collective)
    calcOwnership();

    spans_ = calcSpans();

    kernel_ = calcKernel();

    box_ = calcBox();

    patchPoints_ = calcPatchPoints();

    patchFaces_ = calcPatchFaces();

    calcPatch();

    if (debug)
    {
        // Verify the owned slabs form a complete, disjoint tiling of the plane
        label sumRows = j1_ - j0_;
        reduce(sumRows, sumOp<label>());
        label sumFaces = returnReduce(patchFaces_.size(), sumOp<label>());

        Info<< "IntegralScaleBox: plane partition:" << nl
            << "    - total rows owned   = " << sumRows
            << " (expected " << n_.y() << ")" << nl
            << "    - total faces        = " << sumFaces
            << " (expected " << n_.x()*n_.y() << ")" << endl;

        if (sumRows != n_.y() || sumFaces != n_.x()*n_.y())
        {
            WarningInFunction
                << "Generation-plane partition is not a complete tiling: "
                << "rows " << sumRows << "/" << n_.y()
                << ", faces " << sumFaces << "/" << n_.x()*n_.y()
                << endl;
        }
    }

    if (fsm_)
    {
        C1_ = calcC1(convert(L_));

        C2_ = calcC2(convert(L_));

        slice_ = Field<Type>(p_.size(), Zero);
    }
}


template<class Type>
void Foam::turbulence::IntegralScaleBox<Type>::shift()
{
    for (direction dir = 0; dir < pTraits<Type>::nComponents; ++dir)
    {
        scalarList& slice = box_[dir];

        // Slice span: span of each inlet-normal slice of integral-scale box
        // e.g. for U: (Lyu*Lzu, Lyv*Lzv, Lyw*Lzw)
        const label sliceSpan =
            spans_[dir + pTraits<TypeL>::nComponents/3]
           *spans_[dir + 2*(pTraits<TypeL>::nComponents/3)];

        // Shift forward from the back to the front
        inplaceRotateList(slice, sliceSpan);
    }
}


template<class Type>
void Foam::turbulence::IntegralScaleBox<Type>::refill()
{
    constexpr label nComp3 = pTraits<TypeL>::nComponents/3;

    // Streamwise/time index of the freshly introduced slice. Derived from the
    // global time index (not a free-running counter) so it is identical on
    // every rank and survives restarts/skipped calls without desync.
    const label dt =
        p_.patch().boundaryMesh().mesh().time().timeIndex() - startTimeIndex_;

    for (direction dir = 0; dir < pTraits<Type>::nComponents; ++dir)
    {
        scalarList& slice = box_[dir];

        const label sz2 = spans_[dir + nComp3];
        const label sliceSpan = sz2*spans_[dir + 2*nComp3];

        // Fresh slice is uniquely labelled after the initial sz1 slices
        const label streamwiseIndex = spans_[dir] + dt;

        // Refill the front slice (rotated in by shift()) with the deterministic
        // global random field; overlapping halo cells on neighbour ranks get
        // identical values because the global e3 index (j0_ + i3) is hashed
        for (label s = 0; s < sliceSpan; ++s)
        {
            const label i3 = s/sz2;
            const label i2 = s - i3*sz2;
            slice[s] = gaussHash(dir, streamwiseIndex, i2, j0_ + i3);
        }
    }
}


template<class Type>
Foam::Field<Type>
Foam::turbulence::IntegralScaleBox<Type>::convolve() const
{
    // Output covers only this rank's owned plane rows: ownedRows*n.x faces,
    // ordered e3 (local row) outer, e2 inner - matching the virtual patch
    const label ownedRows = j1_ - j0_;
    Field<Type> outFld(n_.x()*ownedRows, Zero);

    for (direction dir = 0; dir < pTraits<Type>::nComponents; ++dir)
    {
        const scalarList& in = box_[dir];

        Field<scalar> out(n_.x()*ownedRows, Zero);

        const scalarList& kernel1 = kernel_[dir];
        const scalarList& kernel2 =
            kernel_[dir + pTraits<TypeL>::nComponents/3];
        const scalarList& kernel3 =
            kernel_[dir + 2*(pTraits<TypeL>::nComponents/3)];

        const label szkernel1 = kernel1.size();
        const label szkernel2 = kernel2.size();
        const label szkernel3 = kernel3.size();

        const label sz1 = spans_[dir];
        const label sz2 = spans_[dir + pTraits<TypeL>::nComponents/3];
        const label sz3 = spans_[dir + 2*(pTraits<TypeL>::nComponents/3)];
        const label sz23 = sz2*sz3;
        const label sz123 = sz1*sz23;

        const label validSlice2 = sz2 - (szkernel2 - 1);
        const label validSlice3 = sz3 - (szkernel3 - 1);

        // Convolution summation - Along 1st direction
        scalarField tmp(sz123, Zero);
        {
            const label filterCentre = label(szkernel2/label(2));
            const label endIndex = sz2 - filterCentre;
            label i0 = 0;
            label i1 = 0;

            for (label i = 0; i < sz1; ++i)
            {
                for (label j = 0; j < sz3; ++j)
                {
                    i1 += filterCentre;

                    for (label k = filterCentre; k < endIndex; ++k)
                    {
                        label q = 0;

                        for (label p = szkernel2 - 1; p >= 0; --p, ++q)
                        {
                            tmp[i1] += in[i0 + q]*kernel2[p];
                        }
                        ++i0;
                        ++i1;
                    }
                    i0 += 2*filterCentre;
                    i1 += filterCentre;
                }
            }
        }

        // Convolution summation - Along 2nd direction
        {
            const scalarField tmp2(tmp);
            const label filterCentre = label(szkernel3/label(2));
            const label endIndex = sz3 - filterCentre;
            label i1 = 0;
            label i2 = 0;

            for (label i = 0; i < sz1; ++i)
            {
                const label sl = i*sz23;

                for (label j = 0; j < sz2; ++j)
                {
                    i1 = j + sl;
                    i2 = i1;

                    for (label k = 0; k < endIndex - filterCentre; ++k)
                    {
                        tmp[i1] = 0;

                        for (label p = szkernel3 - 1, q = 0; p >= 0; --p, ++q)
                        {
                            tmp[i1] += tmp2[i2 + q*sz2]*kernel3[p];
                        }
                        i1 += sz2;
                        i2 += sz2;
                    }
                    i1 += (sz2 + filterCentre);
                    i2 += (sz2 + filterCentre);
                }
            }
        }

        // Convolution summation - Along 3rd direction
        {
            label i1 = (szkernel2 - label(1))/label(2);
            label i2 = (szkernel2 - label(1))/label(2);
            label i3 = 0;

            for (label i = 0; i < validSlice3; ++i)
            {
                for (label j = 0; j < validSlice2; ++j)
                {
                    scalar sum = 0;
                    i1 = i2 + j;

                    for (label k = szkernel1 - 1; k >= 0; --k)
                    {
                        sum += tmp[i1]*kernel1[k];
                        i1 += sz23;
                    }
                    out[i3] = sum;
                    ++i3;
                }
                i2 += sz2;
            }
        }

        outFld.replace(dir, out);
    }

    return outFld;
}


template<class Type>
void Foam::turbulence::IntegralScaleBox<Type>::correlate
(
    scalarField& fld
)
{
    updateC1C2();

    fld *= C2_;
    fld += slice_*C1_;

    // Store current field for the next time-step
    slice_ = fld;
}


template<class Type>
void Foam::turbulence::IntegralScaleBox<Type>::correlate
(
    vectorField& fld
)
{
    updateC1C2();

    for (direction dir = 0; dir < pTraits<vector>::nComponents; ++dir)
    {
        fld.replace
        (
            dir,
            slice_.component(dir)*C1_[dir] + fld.component(dir)*C2_[dir]
        );
    }

    // Store current field for the next time-step
    slice_ = fld;
}


template<class Type>
void Foam::turbulence::IntegralScaleBox<Type>::write
(
    Ostream& os
) const
{
    os.writeEntryIfDifferent<bool>("fsm", false, fsm_);
    os.writeEntry("n", n_);
    os.writeEntry("L", L_);
    os.writeEntry("kernelType", kernelTypeNames[kernelType_]);
    // Persist the resolved global seed so the deterministic random field is
    // reproducible across restarts and survives decomposePar/reconstructPar
    os.writeEntry("seed", seed_);
    if (csysPtr_)
    {
        csysPtr_->writeEntry(os);
    }
}


// ************************************************************************* //
