/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
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

#include "BaiGosman.H"
#include "LocalInteraction.H"
#include "fvMesh.H"
#include "mathematicalConstants.H"
#include "meshTools.H"
#include "Random.H"
#include "SLGThermo.H"
#include "volFields.H"

// * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * * //

template<class CloudType>
Foam::localInteractionModels::BaiGosman<CloudType>::BaiGosman
(
    LocalInteraction<CloudType>& localInteraction
)
:
    localInteraction_(localInteraction),
    rndGen_(localInteraction.owner().rndGen()),
    thermoPtr_(nullptr)
{}


template<class CloudType>
Foam::localInteractionModels::BaiGosman<CloudType>::BaiGosman
(
    const BaiGosman<CloudType>& bg,
    LocalInteraction<CloudType>& localInteraction
)
:
    localInteraction_(localInteraction),
    rndGen_(localInteraction.owner().rndGen()),
    thermoPtr_(bg.thermoPtr_)
{}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class CloudType>
void Foam::localInteractionModels::BaiGosman<CloudType>::initialise
(
    std::true_type
)
{
    thermoPtr_ =
        localInteraction_.owner().db().template cfindObject<SLGThermo>
        (
            SLGThermo::typeName
        );

    if (!thermoPtr_)
    {
        FatalErrorInFunction
            << "Local patch interaction type "
            << patchInteractionData::beiGosmanTypeName
            << " requires " << SLGThermo::typeName
            << " to be registered for cloud "
            << localInteraction_.owner().name()
            << nl << exit(FatalError);
    }

    if (!thermoPtr_->hasLiquids() || thermoPtr_->liquids().size() == 0)
    {
        FatalErrorInFunction
            << "Local patch interaction type "
            << patchInteractionData::beiGosmanTypeName
            << " requires at least one liquid component in "
            << SLGThermo::typeName << nl << exit(FatalError);
    }
}


template<class CloudType>
void Foam::localInteractionModels::BaiGosman<CloudType>::initialise
(
    std::false_type
)
{
    FatalErrorInFunction
        << "Local patch interaction type "
        << patchInteractionData::beiGosmanTypeName
        << " requires a thermo parcel type exposing T() and rho(). "
        << "Use a thermo/reacting cloud or replace the patch entry with a "
        << "standard localInteraction type."
        << nl << exit(FatalError);
}


template<class CloudType>
const Foam::SLGThermo&
Foam::localInteractionModels::BaiGosman<CloudType>::thermo() const
{
    if (!thermoPtr_)
    {
        FatalErrorInFunction
            << "Local patch interaction type "
            << patchInteractionData::beiGosmanTypeName
            << " was used before " << SLGThermo::typeName
            << " was initialised" << nl << exit(FatalError);
    }

    return *thermoPtr_;
}


template<class CloudType>
Foam::vector Foam::localInteractionModels::BaiGosman<CloudType>::tangentVector
(
    const vector& v
) const
{
    vector tangent(Zero);
    scalar magTangent = 0;

    // Try a bounded number of random samples first.
    for (label iter = 0; iter < 100 && magTangent < SMALL; ++iter)
    {
        const vector vTest(rndGen_.sample01<vector>());
        tangent = vTest - (vTest & v)*v;
        magTangent = mag(tangent);
    }

    // Deterministic fallback: project away the axis least aligned with v.
    if (magTangent < SMALL)
    {
        const vector absV(mag(v.x()), mag(v.y()), mag(v.z()));
        vector axis(1, 0, 0);
        if (absV.y() <= absV.x() && absV.y() <= absV.z())
        {
            axis = vector(0, 1, 0);
        }
        else if (absV.z() <= absV.x() && absV.z() <= absV.y())
        {
            axis = vector(0, 0, 1);
        }

        tangent = axis - (axis & v)*v;
        magTangent = mag(tangent);

        if (magTangent < SMALL)
        {
            // v is degenerate (zero or NaN); return any unit vector.
            return vector(1, 0, 0);
        }
    }

    return tangent/magTangent;
}


template<class CloudType>
Foam::vector Foam::localInteractionModels::BaiGosman<CloudType>::splashDirection
(
    const vector& tanVec1,
    const vector& tanVec2,
    const vector& nf
) const
{
    const scalar phiSi =
        constant::mathematical::twoPi*rndGen_.sample01<scalar>();

    const scalar thetaMin = 5.0;
    const scalar thetaMax = 50.0;
    const scalar thetaSi =
        (thetaMin + (thetaMax - thetaMin)*rndGen_.sample01<scalar>())
      * constant::mathematical::pi/180.0;

    const scalar alpha = sin(thetaSi);
    const scalar dcorr = cos(thetaSi);
    const vector normal(alpha*(tanVec1*cos(phiSi) + tanVec2*sin(phiSi)));
    vector dirVec(dcorr*nf);
    dirVec += normal;

    return dirVec/mag(dirVec);
}


template<class CloudType>
void Foam::localInteractionModels::BaiGosman<CloudType>::absorbInteraction
(
    typename CloudType::parcelType& p,
    const polyPatch& pp,
    const label patchi,
    const label idx,
    const scalar mass,
    bool& keepParticle,
    const bool retainParticle,
    const bool incrementStickCount
)
{
    keepParticle = retainParticle;
    p.active(false);
    p.U() = Zero;

    const label facei = pp.whichFace(p.face());

    localInteraction_.addStickCounters
    (
        patchi,
        idx,
        mass,
        pp,
        facei,
        incrementStickCount
    );
}


template<class CloudType>
void Foam::localInteractionModels::BaiGosman<CloudType>::bounceInteraction
(
    const scalar e,
    const vector& Un,
    vector& U
) const
{
    if (mag(Un) > 0)
    {
        U -= (1.0 + e)*Un;
    }
}


template<class CloudType>
void Foam::localInteractionModels::BaiGosman<CloudType>::splashInteraction
(
    typename CloudType::parcelType& p,
    const polyPatch& pp,
    const label facei,
    const label patchi,
    const label idx,
    const scalar mRatio,
    const scalar We,
    const scalar Wec,
    const scalar sigma,
    const patchInteractionData& coeffs,
    bool& keepParticle
)
{
    const fvMesh& mesh = localInteraction_.owner().mesh();
    const vector& Up =
        localInteraction_.owner().U().boundaryField()[pp.index()][facei];
    const vector& nf = pp.faceNormals()[facei];

    const vector tanVec1(tangentVector(nf));
    const vector tanVec2(nf^tanVec1);

    const scalar np = p.nParticle();
    const scalar m = p.mass()*np;
    const scalar d = p.d();
    const vector Urel(p.U() - Up);
    const vector Un(nf*(Urel & nf));
    const vector Ut(Urel - Un);
    const vector& posC = mesh.C()[p.cell()];
    const vector& posCf = mesh.Cf().boundaryField()[pp.index()][facei];

    const scalar mRatioLimited = min(max(mRatio, scalar(0)), scalar(1));

    if (mRatioLimited <= VSMALL || Wec <= VSMALL || We <= Wec)
    {
        absorbInteraction(p, pp, patchi, idx, m, keepParticle);
        return;
    }

    const scalar mSplash = m*mRatioLimited;
    const scalar Ns = 5.0*(We/Wec - 1.0);

    if (Ns <= VSMALL)
    {
        absorbInteraction(p, pp, patchi, idx, m, keepParticle);
        return;
    }

    const scalar dBarSplash =
        (1/cbrt(6.0))*cbrt(mRatioLimited/Ns)*d + ROOTVSMALL;

    scalar dMax = coeffs.dMaxSplash();
    if (dMax <= 0)
    {
        dMax = 0.9*cbrt(max(mRatioLimited, scalar(ROOTVSMALL)))*d;
    }

    scalar dMin = coeffs.dMinSplash();
    if (dMin <= 0)
    {
        dMin = 0.1*dMax;
    }

    if (dMin <= ROOTVSMALL || dMax <= dMin)
    {
        absorbInteraction(p, pp, patchi, idx, m, keepParticle);
        return;
    }

    const scalar K = exp(-dMin/dBarSplash) - exp(-dMax/dBarSplash);
    if (K <= ROOTVSMALL)
    {
        absorbInteraction(p, pp, patchi, idx, m, keepParticle);
        return;
    }

    scalar ESigmaSec = 0;

    const label parcelsPerSplash = coeffs.parcelsPerSplash();
    scalarList dNew(parcelsPerSplash);
    scalarList npNew(parcelsPerSplash);

    forAll(dNew, i)
    {
        const scalar y = rndGen_.sample01<scalar>();
        dNew[i] = -dBarSplash*Foam::log(exp(-dMin/dBarSplash) - y*K);
        npNew[i] = mRatioLimited*np*pow3(d)/pow3(dNew[i])/parcelsPerSplash;
        ESigmaSec += npNew[i]*sigma*p.areaS(dNew[i]);
    }

    const scalar EKIn = 0.5*m*magSqr(Un);
    const scalar ESigmaIn = np*sigma*p.areaS(d);
    const scalar Ed =
        max
        (
            0.8*EKIn,
            np*Wec/12*constant::mathematical::pi*sqr(d)*sigma
        );
    const scalar EKs = EKIn + ESigmaIn - ESigmaSec - Ed;

    if (EKs <= 0)
    {
        absorbInteraction(p, pp, patchi, idx, m, keepParticle);
        return;
    }

    const scalar logD = Foam::log(d);
    const scalar coeff2 = Foam::log(dNew[0]) - logD + ROOTVSMALL;
    scalar coeff1 = 0;

    for (int i = 1; i < parcelsPerSplash; ++i)
    {
        coeff1 += sqr(Foam::log(dNew[i]) - logD);
    }

    const scalar magUns0 =
        sqrt
        (
            max
            (
                0.0,
                2.0*parcelsPerSplash*EKs/mSplash/(1.0 + coeff1/sqr(coeff2))
            )
        );

    label nSplashParcels = 0;

    forAll(dNew, i)
    {
        if (dNew[i] <= ROOTVSMALL || npNew[i] <= ROOTVSMALL)
        {
            continue;
        }

        const vector dirVec = splashDirection(tanVec1, tanVec2, -nf);

        auto* pPtr = new typename CloudType::parcelType(p);

        pPtr->origId() = pPtr->getNewParticleID();
        pPtr->origProc() = Pstream::myProcNo();

        if (coeffs.splashParcelType() >= 0)
        {
            pPtr->typeId() = coeffs.splashParcelType();
        }

        pPtr->nParticle() = npNew[i];
        pPtr->d() = dNew[i];

        pPtr->U() =
            dirVec
           *(
                mag(coeffs.Cf()*Ut)
              + magUns0*(Foam::log(dNew[i]) - logD)/coeff2
            );
        pPtr->U() += Up;

        meshTools::constrainDirection(mesh, mesh.solutionD(), pPtr->U());

        // Perturb new parcels towards the owner cell centre.
        pPtr->track(0.5*rndGen_.sample01<scalar>()*(posC - posCf), 0);

        // Hand over ownership to the cloud immediately. Mirrors the
        // canonical KinematicSurfaceFilm::splashInteraction pattern and
        // avoids the lifetime/iterator hazards of a deferred buffer.
        localInteraction_.owner().addParticle(pPtr);

        nSplashParcels++;
    }

    if (!nSplashParcels)
    {
        absorbInteraction(p, pp, patchi, idx, m, keepParticle);
        return;
    }

    localInteraction_.addSplashCounters(patchi, idx, mSplash);

    const scalar mDash = m - mSplash;
    if (mDash > VSMALL)
    {
        localInteraction_.addStickCounters
        (
            patchi,
            idx,
            mDash,
            pp,
            facei,
            false
        );
    }

    keepParticle = false;
    p.active(false);
    p.U() = Zero;
}


template<class CloudType>
void Foam::localInteractionModels::BaiGosman<CloudType>::drySplashInteraction
(
    const scalar sigma,
    const scalar mu,
    const polyPatch& pp,
    const label facei,
    typename CloudType::parcelType& p,
    const vector& nf,
    const patchInteractionData& coeffs,
    const label patchi,
    const label idx,
    bool& keepParticle
)
{
    const vector& Up =
        localInteraction_.owner().U().boundaryField()[pp.index()][facei];

    const scalar m = p.mass()*p.nParticle();
    const scalar rho = p.rho();
    const scalar d = p.d();
    const vector Urel(p.U() - Up);
    const vector Un(nf*(Urel & nf));

    const scalar La = rho*sigma*d/sqr(max(mu, scalar(VSMALL)));
    const scalar We = rho*magSqr(Un)*d/max(sigma, scalar(VSMALL));
    const scalar Wec = coeffs.Adry()*pow(max(La, scalar(VSMALL)), -0.183);

    if (We < Wec)
    {
        absorbInteraction(p, pp, patchi, idx, m, keepParticle);
    }
    else
    {
        const scalar mRatio = 0.2 + 0.6*rndGen_.sample01<scalar>();
        splashInteraction
        (
            p,
            pp,
            facei,
            patchi,
            idx,
            mRatio,
            We,
            Wec,
            sigma,
            coeffs,
            keepParticle
        );
    }
}


template<class CloudType>
void Foam::localInteractionModels::BaiGosman<CloudType>::wetSplashInteraction
(
    const scalar sigma,
    const scalar mu,
    const polyPatch& pp,
    const label facei,
    typename CloudType::parcelType& p,
    const vector& nf,
    const patchInteractionData& coeffs,
    const label patchi,
    const label idx,
    bool& keepParticle
)
{
    const vector& Up =
        localInteraction_.owner().U().boundaryField()[pp.index()][facei];

    const scalar m = p.mass()*p.nParticle();
    const scalar rho = p.rho();
    const scalar d = p.d();
    vector& U = p.U();
    const vector Urel(U - Up);
    const vector Un(nf*(Urel & nf));
    const vector Ut(Urel - Un);

    const scalar La = rho*sigma*d/sqr(max(mu, scalar(VSMALL)));
    const scalar We = rho*magSqr(Un)*d/max(sigma, scalar(VSMALL));
    const scalar Wec = coeffs.Awet()*pow(max(La, scalar(VSMALL)), -0.183);

    if (We < 2)
    {
        absorbInteraction(p, pp, patchi, idx, m, keepParticle);
        return;
    }

    if (We >= 2 && We < 20)
    {
        const scalar UMag = max(mag(Urel), scalar(ROOTVSMALL));
        const scalar theta =
            constant::mathematical::piByTwo - acos((Urel/UMag) & nf);
        const scalar epsilon =
            0.993 - theta*(1.76 - theta*(1.56 - theta*0.49));

        U = -epsilon*(Un) + (5.0/7.0)*Ut + Up;
        keepParticle = true;
        p.active(true);
        localInteraction_.addReboundCounters(patchi, idx, m);
        return;
    }

    if (We >= 20 && We < Wec)
    {
        absorbInteraction(p, pp, patchi, idx, m, keepParticle);
        return;
    }

    const scalar mRatio = 0.2 + 0.8*rndGen_.sample01<scalar>();
    splashInteraction
    (
        p,
        pp,
        facei,
        patchi,
        idx,
        mRatio,
        We,
        Wec,
        sigma,
        coeffs,
        keepParticle
    );
}


template<class CloudType>
bool Foam::localInteractionModels::BaiGosman<CloudType>::correct
(
    typename CloudType::parcelType& p,
    const polyPatch& pp,
    const label patchi,
    const label idx,
    bool& keepParticle,
    std::true_type
)
{
    const patchInteractionData& coeffs = localInteraction_.patchData_[patchi];

    vector nw;
    vector Up;
    localInteraction_.owner().patchData(p, pp, nw, Up);

    const label meshFacei = p.face();
    const label facei = pp.whichFace(meshFacei);
    if (facei < 0)
    {
        return false;
    }

    if (mag(nw) <= ROOTVSMALL)
    {
        static bool warnedNormal = false;
        if (!warnedNormal)
        {
            WarningInFunction
                << "Invalid (zero-magnitude) patch normal for patch "
                << pp.name() << "; parcel absorbed. Further occurrences "
                << "will be silently absorbed." << nl;
            warnedNormal = true;
        }

        absorbInteraction
        (
            p, pp, patchi, idx, p.mass()*p.nParticle(), keepParticle
        );
        return true;
    }

    const label celli = p.cell();
    const SLGThermo& slgThermo = thermo();
    const scalar pc = slgThermo.thermo().p()[celli];
    const scalar Tc = slgThermo.thermo().T()[celli];
    const scalar Td = p.T();

    const auto& liquid = slgThermo.liquids().properties()[0];
    const scalar sigma = liquid.sigma(pc, Td);
    const scalar rho = liquid.rho(pc, Td);
    const scalar mu = liquid.mu(pc, Td);
    const scalar d = p.d();

    if (sigma <= VSMALL || rho <= VSMALL || mu <= VSMALL || d <= VSMALL)
    {
        static bool warnedProps = false;
        if (!warnedProps)
        {
            WarningInFunction
                << "Non-positive BaiGosman liquid properties for patch "
                << pp.name() << " at (pc=" << pc << ", Td=" << Td << "): "
                << "sigma=" << sigma << ", rho=" << rho << ", mu=" << mu
                << ", d=" << d << "; parcel absorbed. Check thermo "
                << "and injection settings. Further occurrences will be "
                << "silently absorbed." << nl;
            warnedProps = true;
        }

        absorbInteraction
        (
            p, pp, patchi, idx, p.mass()*p.nParticle(), keepParticle
        );
        return true;
    }

    const scalar m = p.mass()*p.nParticle();
    vector& U = p.U();
    const vector Urel(U - Up);
    const vector Un(nw*(Urel & nw));

    const scalar We = rho*magSqr(Un)*d/sigma;
    const scalar Oh = mu/sqrt(rho*sigma*d);
    const scalar K =
        pow(max(Oh, scalar(VSMALL)), -0.4)*sqrt(max(We, scalar(0)));

    if (Td > coeffs.Tmelt())
    {
        if (We < coeffs.Wec())
        {
            absorbInteraction(p, pp, patchi, idx, m, keepParticle);
        }
        else if (We > coeffs.Wec() && K > 57.7)
        {
            if (coeffs.dry())
            {
                drySplashInteraction
                (
                    sigma,
                    mu,
                    pp,
                    facei,
                    p,
                    nw,
                    coeffs,
                    patchi,
                    idx,
                    keepParticle
                );
            }
            else
            {
                wetSplashInteraction
                (
                    sigma,
                    mu,
                    pp,
                    facei,
                    p,
                    nw,
                    coeffs,
                    patchi,
                    idx,
                    keepParticle
                );
            }
        }
        else
        {
            absorbInteraction(p, pp, patchi, idx, m, keepParticle);
        }
    }
    else
    {
        const scalar denom = max(coeffs.Tmelt() - Tc, scalar(ROOTVSMALL));
        const scalar Tstar = min(max((Td - Tc)/denom, scalar(0)), scalar(1));
        const scalar sigmay = 14e6*pow(max(1.0 - Tstar, scalar(0)), 0.7);

        const scalar eArg =
            1.0
          - rho*sqr(mag(Un))
           /(6.0*max(sigmay, scalar(VSMALL))*max(Td, scalar(VSMALL)));
        const scalar e = max(0.0, sqrt(max(eArg, scalar(0))));

        if (e < 0.05)
        {
            absorbInteraction(p, pp, patchi, idx, m, keepParticle);
        }
        else
        {
            keepParticle = true;
            p.active(true);
            bounceInteraction(e, Un, U);
            localInteraction_.addReboundCounters(patchi, idx, m);
        }
    }

    return true;
}


template<class CloudType>
bool Foam::localInteractionModels::BaiGosman<CloudType>::correct
(
    typename CloudType::parcelType&,
    const polyPatch& pp,
    const label,
    const label,
    bool&,
    std::false_type
)
{
    FatalErrorInFunction
        << "Patch " << pp.name() << " uses local patch interaction type "
        << patchInteractionData::beiGosmanTypeName
        << ", but cloud " << localInteraction_.owner().name()
        << " does not use thermo parcels exposing T() and rho()."
        << nl << exit(FatalError);

    return false;
}


// ************************************************************************* //
