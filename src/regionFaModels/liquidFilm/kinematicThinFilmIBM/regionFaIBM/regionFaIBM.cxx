/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2022-2025 OpenCFD Ltd.
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

#include "regionFaIBM.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "Time.H"
#include "scalarMatrices.H"
#include "ListOps.H"
#include "OBJstream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace regionModels
{
namespace areaSurfaceFilmModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(regionFaIBM, 0);

const Foam::Enum<regionFaIBM::solveType> regionFaIBM::solveTypeNames
({
    { solveType::invDistance, "invDistance" },
    { solveType::direct, "direct" }
});


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

void regionFaIBM::dumpStencil
(
    const fileName& fname,
    const mapDistribute& mapDist,
    const labelListList& stencil,
    const pointField& centres
)
{
    pointField testFaceCentres(centres);
    mapDist.distribute(testFaceCentres);
    OBJstream os(fname);
    forAll(stencil, facei)
    {
        if (const auto& slots = stencil[facei]; slots.size() > 2)
        {
            forAll(slots, sloti)
            {
                os.write
                (
                    linePointRef
                    (
                        centres[facei],
                        testFaceCentres[slots[sloti]]
                    )
                );
            }
            break;
        }
    }
    os.flush();
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

bool regionFaIBM::moveBody()
{
    tmp<pointField> tnewPoints
    (
        transformPoints(SBMFPtr_().transformation(), points0_)
    );
    const auto& newPoints = tnewPoints();

    // const bool moving = max(magSqr(newPoints - surface_.points())) > 1e-6;

    // // Exit early if there is no motion
    // if (!moving)
    // {
    //     return false;
    // }

    surface_.movePoints(newPoints);

    const Time& runTime = aMesh_.time();
    deltaT_ = runTime.value() - updateTimeOld_;
    updateTimeOld_ = runTime.value();

    // Transformation from new position to old position
    invTransformDelta_ = inv(SBMFPtr_().transformation() * inv(transformOld_));
    transformOld_ = SBMFPtr_().transformation();

    setFaceTypes();

    return true;
}


void regionFaIBM::setFaceTypes()
{
    // Get the volume types from the surface
    List<volumeType> volTypeID;
    surface_.getVolumeType(aMesh_.areaCentres(), volTypeID);

    // Initialise faceTypes_ to 'outside'
    faceTypes_.resize_nocopy(aMesh_.nFaces());
    faceTypes_ = 0;

    DynamicList<label> forcingFaceIDs;
    DynamicList<label> insideFaceIDs;

    forAll(aMesh_.owner(), facei)
    {
        const label own = aMesh_.owner()[facei];
        const label nbr = aMesh_.neighbour()[facei];

        // Inside
        // Add faces to 'inside' faces
        if (volTypeID[own] == volumeType::INSIDE)
        {
            faceTypes_[own] = 2;
            insideFaceIDs.push_back(own);
        }
        if (volTypeID[nbr] == volumeType::INSIDE)
        {
            faceTypes_[nbr] = 2;
            insideFaceIDs.push_back(nbr);
        }

        // Transition inside<>outside
        // Add faces to 'forcing' faces
        if
        (
            volTypeID[own] == volumeType::INSIDE
         && volTypeID[nbr] != volumeType::INSIDE
        )
        {
            faceTypes_[nbr] = 1;
            forcingFaceIDs.push_back(nbr);
        }
        if
        (
            volTypeID[own] != volumeType::INSIDE
         && volTypeID[nbr] == volumeType::INSIDE
        )
        {
            faceTypes_[own] = 1;
            forcingFaceIDs.push_back(own);
        }
    }

    forcingFaceIDs_.transfer(forcingFaceIDs);
    insideFaceIDs_.transfer(insideFaceIDs);

    if (aMesh_.time().writeTime())
    {
        areaScalarField faceTypeField
        (
            IOobject
            (
                "faceType",
                aMesh_.time().timeName(),
                aMesh_.mesh(),
                IOobjectOption::NO_READ,
                IOobjectOption::NO_WRITE,
                IOobjectOption::NO_REGISTER
            ),
            aMesh_,
            Foam::zero{},
            dimless,
            faPatchFieldBase::zeroGradientType()
        );

        auto& faceTypeFieldi = faceTypeField.primitiveFieldRef();
        forAll(faceTypeFieldi, facei)
        {
            faceTypeFieldi[facei] = faceTypes_[facei];
        }

        faceTypeField.write();
    }
}


void regionFaIBM::setStencilAddressing
(
    const pointField& forcingFaceCentres,
    const scalarField& forcingFaceAreas,
    pointField& mirrorPoints
)
{
    const label nForcingFaces = forcingFaceCentres.size();

    // Early exit if there are no forcing faces
    if (returnReduceAnd(!nForcingFaces, UPstream::worldComm))
    {
        return;
    }

    // Find closest point on surface for each forcing face
    List<pointIndexHit> hits(nForcingFaces);
    surface_.findNearest
    (
        forcingFaceCentres, // samples
        10*forcingFaceAreas, // nearestDistSqr
        hits
    );

    // Determine physical size of interpolation stencil equivalent sphere radius
    const scalar r = rFactor_*Foam::sqrt(gAverage(forcingFaceAreas));
    DebugInfo << "regionFaIBM: r = " << r << endl;

    pointField surfaceFace(nForcingFaces, point::zero);
    pointField extendedFacePoint(nForcingFaces, point::zero);

    DebugInfo << "regionFaIBM: Surface point hits " << hits.size() << endl;

    forAll(hits, i)
    {
        if (!hits[i].hit())
        {
            FatalErrorInFunction
                << "Unable to find hit point for face centre "
                << forcingFaceCentres[i]
                << abort(FatalError);
        }

        mirrorPoints[i] = hits[i].point();

        const vector n(normalised(forcingFaceCentres[i] - mirrorPoints[i]));
        extendedFacePoint[i] = mirrorPoints[i] + n*r;
    }

    h_.resize_nocopy(nForcingFaces);
    forAll(h_, i)
    {
        const scalar h1 = mag(extendedFacePoint[i] - forcingFaceCentres[i]);
        const scalar h2 = mag(forcingFaceCentres[i] - mirrorPoints[i]);
        h_[i] = h1/(h1 + h2);
    }

    // Processor domain bounds
    List<boundBox> procBoundingBoxes(UPstream::nProcs());
    procBoundingBoxes[UPstream::myProcNo()] = boundBox(aMesh_.points(), false);
    Pstream::allGatherList(procBoundingBoxes);

    PstreamBuffers pBufs(UPstream::defaultCommsType);
    const globalIndex globalFaces(aMesh_.nFaces(), UPstream::worldComm);

    // Send forcing face IDs (in global addressing) and extended face centres
    forAll(procBoundingBoxes, proci)
    {
        const auto& procBb = procBoundingBoxes[proci];

        DynamicList<label> faceIDs;
        DynamicList<point> extendedFacePts;

        forAll(extendedFacePoint, facei)
        {
            const point& pt = extendedFacePoint[facei];

            if (procBb.overlaps(pt, r*r))
            {
                const label forcingFacei = forcingFaceIDs_[facei];
                faceIDs.push_back(globalFaces.toGlobal(forcingFacei));
                extendedFacePts.push_back(pt);
            }
        }

        UOPstream os(proci, pBufs);
        os  << faceIDs << extendedFacePts;
    }

    pBufs.finishedSends();

    // Reverse stencil
    // - identify forcing face for each outside/donor face
    List<DynamicList<label>> rStencil0(aMesh_.nFaces());
    forAll(procBoundingBoxes, proci)
    {
        UIPstream is(proci, pBufs);

        const labelList procForcingIDs(is);
        const List<point> procExtendedFacePts(is);

        forAll(procForcingIDs, pti)
        {
            const auto donorFaceIDs =
                treePtr_->findSphere(procExtendedFacePts[pti], r*r);

            for (const label id : donorFaceIDs)
            {
                if (faceTypes_[id] == 0)
                {
                    rStencil0[id].push_uniq(procForcingIDs[pti]);
                }
            }
        }
    }

    // Transfer to labelListList for mapDistribute
    labelListList rStencil(rStencil0.size());
    forAll(rStencil, facei)
    {
        rStencil[facei].transfer(rStencil0[facei]);
    }


    DebugInfo << "regionFaIBM: Creating reverse stencil mapDistribute" << endl;

    List<Map<label>> compactMap;
    const mapDistribute rmap(globalFaces, rStencil, compactMap);

    // Clear stencil - may contain addressing from earlier evaluation
    stencil_.resize_nocopy(rmap.constructSize());
    for (auto& donors : stencil_)
    {
        donors.clear();
    }

    forAll(rStencil, facei)
    {
        const auto& slots = rStencil[facei];
        for (const auto sloti : slots)
        {
            stencil_[sloti].push_back(globalFaces.toGlobal(facei));
        }
    }

    // Reverse distribute
    DebugInfo << "regionFaIBM: Creating stencil mapDistribute" << endl;

    mapDistributeBase::distribute
    (
        UPstream::commsTypes::nonBlocking,
        UList<labelPair>::null(),
        aMesh_.nFaces(),
        rmap.constructMap(),
        false, // subHasFlip
        rmap.subMap(),
        false, // constructHasFlip
        stencil_,
        labelList(),
        ListOps::appendEqOp<label>(),
        identityOp()       // No flipping
    );

    label stencilMin = labelMax;
    label stencilMax = 0;
    label stencilCount = 0;
    scalar stencilSum = 0;
    for (const auto& donors: stencil_)
    {
        if (donors.size() > 0)
        {
            ++stencilCount;

            const label sz = donors.size();
            stencilMin = min(stencilMin, sz);
            stencilMax = max(stencilMax, sz);
            stencilSum += sz;
        }
    }

    reduce(stencilCount, sumOp<label>());

    if (stencilCount == 0)
    {
        Info<< "- stencil empty" << endl;
        return;
    }

    reduce(stencilMin, minOp<label>());
    reduce(stencilMax, maxOp<label>());
    reduce(stencilSum, sumOp<scalar>());

    Info<< "- stencil donors:" << stencilCount
        << " min:" << stencilMin
        << " max:" << stencilMax
        << " ave:" << (stencilSum/stencilCount)
        << endl;

    compactMap.clear();

    mapPtr_.reset
    (
        autoPtr<mapDistribute>::New
        (
            globalFaces,
            stencil_,
            compactMap
        )
    );

    if (debug > 1)
    {
        dumpStencil
        (
            "regionFaIBM-stencil.obj",
            mapPtr_(),
            stencil_,
            aMesh_.areaCentres()
        );
    }
}


void regionFaIBM::setStencilWeights(const pointField& mirrorPoints)
{
    // Clear weights - may contain values from earlier evaluation
    weights_.resize_nocopy(mirrorPoints.size());
    for (auto& donors : weights_)
    {
        donors.clear();
    }

    pointField donorFaceCentres(aMesh_.areaCentres());
    mapPtr_->distribute(donorFaceCentres);

    switch (solveType_)
    {
        case solveType::invDistance:
        {
            // Inverse distance
            forAll(forcingFaceIDs_, i)
            {
                const label facei = forcingFaceIDs_[i];
                const auto& slots = stencil_[facei];
                const label n = slots.size();

                if (n == 0)
                {
                    continue;
                }

                const point& mp = mirrorPoints[i];

                scalar sumInvMagDx = 0;
                for (const label sloti : slots)
                {
                    const vector dx(donorFaceCentres[sloti] - mp);
                    sumInvMagDx += 1.0/mag(dx);
                }

                weights_[i].resize_nocopy(slots.size());
                forAll(slots, j)
                {
                    const label sloti = slots[j];
                    const vector dx(donorFaceCentres[sloti] - mp);
                    weights_[i][j] = 1.0/mag(dx)/sumInvMagDx;
                }
            }
            break;
        }

        case solveType::direct:
        {
            // Solve using matrix system
            forAll(forcingFaceIDs_, i)
            {
                const label facei = forcingFaceIDs_[i];
                const auto& slots = stencil_[facei];
                const label n = slots.size();

                if (n == 0)
                {
                    continue;
                }

                const point& mp = mirrorPoints[i];

                // Precompute sum(squared distances)
                scalarField sqrDist(n);
                forAll(slots, j)
                {
                    const label sloti = slots[j];
                    const vector dx(donorFaceCentres[sloti] - mp);
                    sqrDist[j] = dx & dx;
                }

                // Build system: [n+2 x n+2]
                // - n weights
                // - 2 Lagrange mult. constraints (laplacian, normalisation)
                scalarSquareMatrix A(n+2, 0.0);
                scalarField b(n+2, 0.0);

                // Top-left: 2*I
                for (label i = 0; i < n; ++i)
                {
                    A[i][i] = 2.0;
                }

                // Constraints
                for (label i = 0; i < n; ++i)
                {
                    A[i][n] = sqrDist[i];      // lambda1 column
                    A[i][n+1] = 1.0;           // lambda2 column
                    A[n][i] = sqrDist[i];      // Laplacian row
                    A[n+1][i] = 1.0;           // Normalisation row
                }

                // RHS: sum(w) = 1
                b[n+1] = 1.0;

                // Solve - result in b
                LUsolve(A, b);

                // Extract weights
                weights_[i] = SubList<scalar>(b, n);
            }
            break;
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

regionFaIBM::regionFaIBM
(
    const Time& runTime,
    const faMesh& aMesh,
    const dictionary& dict
)
:
    aMesh_(aMesh),
    surface_
    (
        IOobject
        (
            dict.get<fileName>("surface").expand(),
            runTime,
            IOobject::MUST_READ
        )
    ),
    points0_(surface_.points()),
    SBMFPtr_(solidBodyMotionFunction::New(dict, runTime)),
    deltaT_(0),
    updateTimeOld_(0),
    transformOld_(SBMFPtr_->transformation()),
    invTransformDelta_(Zero),
    solveType_
    (
        solveTypeNames.getOrDefault("solveType", dict, solveType::direct)
    ),
    rFactor_(dict.getOrDefault<scalar>("stencilRadiusFactor", 1.5))
{
    updateMesh();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void regionFaIBM::updateMesh()
{
    treeBoundBox bb(aMesh_.mesh().bounds());
    bb.inflate(0.01);

    treePtr_.reset
    (
        new indexedOctree<treeType>
        (
            treeType
            (
                false,
                aMesh_.patch(),
                indexedOctree<treeType>::perturbTol()
            ),
            bb,                         // overall search domain
            8,                          // maxLevel
            10,                         // leaf size
            3.0                         // duplicity
        )
    );
}


bool regionFaIBM::correct(const areaVectorField& Us, areaVectorField& Ustar)
{
    if (!moveBody()) return false;

    Info<< "Updating immersed boundary" << endl;

    const label nForcingFaces = forcingFaceIDs_.size();

    DebugInfo << "regionFaIBM: nForcingFaces = " << nForcingFaces << endl;

    pointField forcingFaceCentres(nForcingFaces);
    scalarField forcingFaceAreas(nForcingFaces);
    forAll(forcingFaceIDs_, i)
    {
        const label facei = forcingFaceIDs_[i];
        forcingFaceCentres[i] = aMesh_.areaCentres()[facei];
        forcingFaceAreas[i] = aMesh_.S()[facei];
    }

    // Set stencil addresses
    pointField mirrorPoints(nForcingFaces, point::zero);
    setStencilAddressing
    (
        forcingFaceCentres,
        forcingFaceAreas,
        mirrorPoints
    );

    // Set stencil weights
    setStencilWeights(mirrorPoints);

    const auto& areaCentres = aMesh_.areaCentres();

    forAll(insideFaceIDs_, i)
    {
        const label facei = insideFaceIDs_[i];
        const point& ip = areaCentres[facei];
        const point ip0 = invTransformDelta_.transformPoint(ip);

        Ustar[facei] = (ip - ip0)/deltaT_;
    }

    vectorField Uforcing(nForcingFaces, vector::zero);
    forAll(mirrorPoints, i)
    {
        const point& mp = mirrorPoints[i];
        const point mp0 = invTransformDelta_.transformPoint(mp);

        Uforcing[i] = (mp - mp0)/deltaT_;
    }

    DebugInfo
        << "regionFaIBM: min,max,ave(Uforcing) = "
        << gMin(Uforcing)
        << "," << gMax(Uforcing)
        << "," << gAverage(Uforcing)
        << endl;

    vectorField work;
    if (UPstream::parRun())
    {
        work = Us;
        mapPtr_->distribute(work);
    }

    const vectorField& Ufar =
    (
        UPstream::parRun()
      ? work
      : static_cast<const vectorField&>(Us.primitiveField())
    );

    forAll(forcingFaceIDs_, i)
    {
        if (const auto& w = weights_[i]; w.size() > 0)
        {
            const label facei = forcingFaceIDs_[i];

            vector Uextend(Zero);
            forAll(w, j)
            {
                Uextend += w[j]*Ufar[stencil_[facei][j]];
            }

            Ustar[facei] = h_[i]*Uforcing[i] + (1 - h_[i])*Uextend;
        }
    }

    return true;
}


void regionFaIBM::addToMask(areaScalarField& mask) const
{
    UIndirectList<scalar>(mask, forcingFaceIDs_) = 1.0;
    UIndirectList<scalar>(mask, insideFaceIDs_) = 1.0;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace areaSurfaceFilmModels
} // End namespace regionModels
} // End namespace Foam

// ************************************************************************* //
