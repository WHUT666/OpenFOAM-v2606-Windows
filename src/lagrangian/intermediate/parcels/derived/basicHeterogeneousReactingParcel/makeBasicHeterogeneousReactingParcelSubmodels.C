#define Foam_DispersionRASModel_defines_typeName
#define Foam_ParticleForce_defines_typeName
#define Foam_CellZoneInjection_defines_typeName
#define Foam_CloudFunctionObject_defines_typeName
#define Foam_CompositionModel_defines_typeName
#define Foam_ConeInjection_defines_typeName
#define Foam_ConeNozzleInjection_defines_typeName
#define Foam_CoulombForce_defines_typeName
#define Foam_DampingModel_defines_typeName
#define Foam_DispersionModel_defines_typeName
#define Foam_Explicit_defines_typeName
#define Foam_FaceInteraction_defines_typeName
#define Foam_FacePostProcessing_defines_typeName
#define Foam_FieldActivatedInjection_defines_typeName
#define Foam_FreezeParticles_defines_typeName
#define Foam_GravityForce_defines_typeName
#define Foam_HeatTransferCoeff_defines_typeName
#define Foam_HeatTransferModel_defines_typeName
#define Foam_HeterogeneousReactingModel_defines_typeName
#define Foam_Implicit_defines_typeName
#define Foam_InjectionModel_defines_typeName
#define Foam_IsotropyModel_defines_typeName
#define Foam_KinematicSurfaceFilm_defines_typeName
#define Foam_LiquidEvapFuchsKnudsen_defines_typeName
#define Foam_LiquidEvaporation_defines_typeName
#define Foam_LiquidEvaporationBoil_defines_typeName
#define Foam_LocalInteraction_defines_typeName
#define Foam_MUCSheterogeneousRate_defines_typeName
#define Foam_ManualInjection_defines_typeName
#define Foam_MultiInteraction_defines_typeName
#define Foam_NoComposition_defines_typeName
#define Foam_NoDamping_defines_typeName
#define Foam_NoDispersion_defines_typeName
#define Foam_NoHeatTransfer_defines_typeName
#define Foam_NoInjection_defines_typeName
#define Foam_NoInteraction_defines_typeName
#define Foam_NoIsotropy_defines_typeName
#define Foam_NoPacking_defines_typeName
#define Foam_NoPhaseChange_defines_typeName
#define Foam_NoStochasticCollision_defines_typeName
#define Foam_NoSurfaceFilm_defines_typeName
#define Foam_NoheterogeneousReacting_defines_typeName
#define Foam_NonInertialFrameForce_defines_typeName
#define Foam_NonSphereDragForce_defines_typeName
#define Foam_NusseltNumber_defines_typeName
#define Foam_PackingModel_defines_typeName
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
#define Foam_PhaseChangeModel_defines_typeName
#define Foam_PressureGradientForce_defines_typeName
#define Foam_RanzMarshall_defines_typeName
#define Foam_ReactingLookupTableInjection_defines_typeName
#define Foam_ReactingWeberNumber_defines_typeName
#define Foam_Rebound_defines_typeName
#define Foam_RecycleInteraction_defines_typeName
#define Foam_Relaxation_defines_typeName
#define Foam_RemoveParcels_defines_typeName
#define Foam_SRFForce_defines_typeName
#define Foam_SaffmanMeiLiftForce_defines_typeName
#define Foam_SingleMixtureFraction_defines_typeName
#define Foam_SphereDragForce_defines_typeName
#define Foam_StandardWallInteraction_defines_typeName
#define Foam_Stochastic_defines_typeName
#define Foam_StochasticCollisionModel_defines_typeName
#define Foam_SurfaceFilmModel_defines_typeName
#define Foam_ThermoReynoldsNumber_defines_typeName
#define Foam_ThermoSurfaceFilm_defines_typeName
#define Foam_TomiyamaDragForce_defines_typeName
#define Foam_TomiyamaLiftForce_defines_typeName
#define Foam_VirtualMassForce_defines_typeName
#define Foam_VoidFraction_defines_typeName
/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018-2021 OpenCFD Ltd.
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

#include "basicHeterogeneousReactingCloud.H"

#include "makeReactingParcelCloudFunctionObjects.H"

// Kinematic
#include "makeThermoParcelForces.H" // thermo variant
#include "makeParcelDispersionModels.H"
#include "makeReactingParcelInjectionModels.H" // Reacting variant
#include "makeThermoParcelPatchInteractionModels.H"
#include "makeParcelStochasticCollisionModels.H"
#include "makeReactingParcelSurfaceFilmModels.H" // Reacting variant
#include "makeHeterogeneousReactingParcelHeterogeneousReactingModels.H"

// Thermodynamic
#include "makeParcelHeatTransferModels.H"

// Reacting
#include "makeReactingMultiphaseParcelCompositionModels.H"
#include "makeReactingParcelPhaseChangeModels.H"

// MPPIC sub-models
#include "makeMPPICParcelDampingModels.H"
#include "makeMPPICParcelIsotropyModels.H"
#include "makeMPPICParcelPackingModels.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makeReactingParcelCloudFunctionObjects(basicHeterogeneousReactingCloud);

// Kinematic sub-models
makeThermoParcelForces(basicHeterogeneousReactingCloud);
makeParcelDispersionModels(basicHeterogeneousReactingCloud);
makeReactingParcelInjectionModels(basicHeterogeneousReactingCloud);
makeThermoParcelPatchInteractionModels(basicHeterogeneousReactingCloud);
makeParcelStochasticCollisionModels(basicHeterogeneousReactingCloud);
makeReactingParcelSurfaceFilmModels(basicHeterogeneousReactingCloud);

// Thermo sub-models
makeParcelHeatTransferModels(basicHeterogeneousReactingCloud);

// Reacting sub-models
makeReactingMultiphaseParcelCompositionModels(basicHeterogeneousReactingCloud);
makeReactingParcelPhaseChangeModels(basicHeterogeneousReactingCloud);
makeHeterogeneousReactingParcelHeterogeneousReactingModels
(
    basicHeterogeneousReactingCloud
);

// MPPIC sub-models
makeMPPICParcelDampingModels(basicHeterogeneousReactingCloud);
makeMPPICParcelIsotropyModels(basicHeterogeneousReactingCloud);
makeMPPICParcelPackingModels(basicHeterogeneousReactingCloud);

// ************************************************************************* //
