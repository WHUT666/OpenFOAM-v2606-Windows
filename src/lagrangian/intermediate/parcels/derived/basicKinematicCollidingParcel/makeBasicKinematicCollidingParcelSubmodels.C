#define Foam_DispersionRASModel_defines_typeName
#define Foam_ParticleForce_defines_typeName
#define Foam_CellZoneInjection_defines_typeName
#define Foam_CloudFunctionObject_defines_typeName
#define Foam_CollisionModel_defines_typeName
#define Foam_ConeInjection_defines_typeName
#define Foam_ConeNozzleInjection_defines_typeName
#define Foam_CoulombForce_defines_typeName
#define Foam_DampingModel_defines_typeName
#define Foam_DispersionModel_defines_typeName
#define Foam_ErgunWenYuDragForce_defines_typeName
#define Foam_Explicit_defines_typeName
#define Foam_FaceInteraction_defines_typeName
#define Foam_FacePostProcessing_defines_typeName
#define Foam_FieldActivatedInjection_defines_typeName
#define Foam_FreezeParticles_defines_typeName
#define Foam_GravityForce_defines_typeName
#define Foam_Implicit_defines_typeName
#define Foam_InflationInjection_defines_typeName
#define Foam_InjectedParticleDistributionInjection_defines_typeName
#define Foam_InjectedParticleInjection_defines_typeName
#define Foam_InjectionModel_defines_typeName
#define Foam_InterfaceForce_defines_typeName
#define Foam_IsotropyModel_defines_typeName
#define Foam_KinematicLookupTableInjection_defines_typeName
#define Foam_KinematicReynoldsNumber_defines_typeName
#define Foam_KinematicSurfaceFilm_defines_typeName
#define Foam_KinematicWeberNumber_defines_typeName
#define Foam_LocalInteraction_defines_typeName
#define Foam_ManualInjection_defines_typeName
#define Foam_MultiInteraction_defines_typeName
#define Foam_NoCollision_defines_typeName
#define Foam_NoDamping_defines_typeName
#define Foam_NoDispersion_defines_typeName
#define Foam_NoInjection_defines_typeName
#define Foam_NoInteraction_defines_typeName
#define Foam_NoIsotropy_defines_typeName
#define Foam_NoPacking_defines_typeName
#define Foam_NoPair_defines_typeName
#define Foam_NoStochasticCollision_defines_typeName
#define Foam_NoSurfaceFilm_defines_typeName
#define Foam_NonInertialFrameForce_defines_typeName
#define Foam_NonSphereDragForce_defines_typeName
#define Foam_PackingModel_defines_typeName
#define Foam_PairCollision_defines_typeName
#define Foam_PairModel_defines_typeName
#define Foam_PairSpringSliderDashpot_defines_typeName
#define Foam_ParamagneticForce_defines_typeName
#define Foam_ParticleCollector_defines_typeName
#define Foam_ParticleDose_defines_typeName
#define Foam_ParticleErosion_defines_typeName
#define Foam_ParticleHistogram_defines_typeName
#define Foam_ParticlePostProcessing_defines_typeName
#define Foam_ParticleTracks_defines_typeName
#define Foam_ParticleTrap_defines_typeName
#define Foam_ParticleZoneInfo_defines_typeName
#define Foam_PatchCollisionDensity_defines_typeName
#define Foam_PatchFlowRateInjection_defines_typeName
#define Foam_PatchInjection_defines_typeName
#define Foam_PatchInteractionFields_defines_typeName
#define Foam_PatchInteractionModel_defines_typeName
#define Foam_PlessisMasliyahDragForce_defines_typeName
#define Foam_PressureGradientForce_defines_typeName
#define Foam_Rebound_defines_typeName
#define Foam_RecycleInteraction_defines_typeName
#define Foam_Relaxation_defines_typeName
#define Foam_RemoveParcels_defines_typeName
#define Foam_SRFForce_defines_typeName
#define Foam_SaffmanMeiLiftForce_defines_typeName
#define Foam_SphereDragForce_defines_typeName
#define Foam_StandardWallInteraction_defines_typeName
#define Foam_Stochastic_defines_typeName
#define Foam_StochasticCollisionModel_defines_typeName
#define Foam_SurfaceFilmModel_defines_typeName
#define Foam_TomiyamaDragForce_defines_typeName
#define Foam_TomiyamaLiftForce_defines_typeName
#define Foam_VirtualMassForce_defines_typeName
#define Foam_VoidFraction_defines_typeName
#define Foam_WallLocalSpringSliderDashpot_defines_typeName
#define Foam_WallModel_defines_typeName
#define Foam_WallSpringSliderDashpot_defines_typeName
#define Foam_WenYuDragForce_defines_typeName
/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
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

#include "basicKinematicCollidingCloud.H"

#include "makeParcelCloudFunctionObjects.H"

// Kinematic
#include "makeParcelForces.H"
#include "makeParcelDispersionModels.H"
#include "makeParcelInjectionModels.H"
#include "makeParcelCollisionModels.H"
#include "makeParcelPatchInteractionModels.H"
#include "makeParcelStochasticCollisionModels.H"
#include "makeParcelSurfaceFilmModels.H"

// MPPIC sub-models
#include "makeMPPICParcelDampingModels.H"
#include "makeMPPICParcelIsotropyModels.H"
#include "makeMPPICParcelPackingModels.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makeParcelCloudFunctionObjects(basicKinematicCollidingCloud);

// Kinematic sub-models
makeParcelForces(basicKinematicCollidingCloud);
makeParcelDispersionModels(basicKinematicCollidingCloud);
makeParcelInjectionModels(basicKinematicCollidingCloud);
makeParcelCollisionModels(basicKinematicCollidingCloud);
makeParcelPatchInteractionModels(basicKinematicCollidingCloud);
makeParcelStochasticCollisionModels(basicKinematicCollidingCloud);
makeParcelSurfaceFilmModels(basicKinematicCollidingCloud);

// MPPIC sub-models
makeMPPICParcelDampingModels(basicKinematicCollidingCloud);
makeMPPICParcelIsotropyModels(basicKinematicCollidingCloud);
makeMPPICParcelPackingModels(basicKinematicCollidingCloud);

// ************************************************************************* //
