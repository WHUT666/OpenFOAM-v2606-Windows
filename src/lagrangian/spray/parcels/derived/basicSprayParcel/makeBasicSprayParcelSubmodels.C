#define Foam_DispersionRASModel_defines_typeName
#define Foam_ParticleForce_defines_typeName
#define Foam_AtomizationModel_defines_typeName
#define Foam_BlobsSheetAtomization_defines_typeName
#define Foam_BreakupModel_defines_typeName
#define Foam_BrownianMotionForce_defines_typeName
#define Foam_CellZoneInjection_defines_typeName
#define Foam_CloudFunctionObject_defines_typeName
#define Foam_CompositionModel_defines_typeName
#define Foam_ConeInjection_defines_typeName
#define Foam_ConeNozzleInjection_defines_typeName
#define Foam_CoulombForce_defines_typeName
#define Foam_DampingModel_defines_typeName
#define Foam_DispersionModel_defines_typeName
#define Foam_DistortedSphereDragForce_defines_typeName
#define Foam_ETAB_defines_typeName
#define Foam_Explicit_defines_typeName
#define Foam_FaceInteraction_defines_typeName
#define Foam_FacePostProcessing_defines_typeName
#define Foam_FieldActivatedInjection_defines_typeName
#define Foam_FreezeParticles_defines_typeName
#define Foam_GradientDispersionRAS_defines_typeName
#define Foam_GravityForce_defines_typeName
#define Foam_HeatTransferCoeff_defines_typeName
#define Foam_HeatTransferModel_defines_typeName
#define Foam_Implicit_defines_typeName
#define Foam_InflationInjection_defines_typeName
#define Foam_InjectedParticleDistributionInjection_defines_typeName
#define Foam_InjectedParticleInjection_defines_typeName
#define Foam_InjectionModel_defines_typeName
#define Foam_IsotropyModel_defines_typeName
#define Foam_KinematicSurfaceFilm_defines_typeName
#define Foam_LISAAtomization_defines_typeName
#define Foam_LiquidEvapFuchsKnudsen_defines_typeName
#define Foam_LiquidEvaporation_defines_typeName
#define Foam_LiquidEvaporationBoil_defines_typeName
#define Foam_LocalInteraction_defines_typeName
#define Foam_ManualInjection_defines_typeName
#define Foam_MultiInteraction_defines_typeName
#define Foam_NoAtomization_defines_typeName
#define Foam_NoBreakup_defines_typeName
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
#define Foam_NonInertialFrameForce_defines_typeName
#define Foam_NonSphereDragForce_defines_typeName
#define Foam_NusseltNumber_defines_typeName
#define Foam_ORourkeCollision_defines_typeName
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
#define Foam_PilchErdman_defines_typeName
#define Foam_PressureGradientForce_defines_typeName
#define Foam_RanzMarshall_defines_typeName
#define Foam_ReactingWeberNumber_defines_typeName
#define Foam_Rebound_defines_typeName
#define Foam_RecycleInteraction_defines_typeName
#define Foam_ReitzDiwakar_defines_typeName
#define Foam_ReitzKHRT_defines_typeName
#define Foam_Relaxation_defines_typeName
#define Foam_RemoveParcels_defines_typeName
#define Foam_SHF_defines_typeName
#define Foam_SRFForce_defines_typeName
#define Foam_SaffmanMeiLiftForce_defines_typeName
#define Foam_SinglePhaseMixture_defines_typeName
#define Foam_SphereDragForce_defines_typeName
#define Foam_StandardWallInteraction_defines_typeName
#define Foam_Stochastic_defines_typeName
#define Foam_StochasticCollisionModel_defines_typeName
#define Foam_StochasticDispersionRAS_defines_typeName
#define Foam_SurfaceFilmModel_defines_typeName
#define Foam_TAB_defines_typeName
#define Foam_ThermoReynoldsNumber_defines_typeName
#define Foam_ThermoSurfaceFilm_defines_typeName
#define Foam_TomiyamaDragForce_defines_typeName
#define Foam_TomiyamaLiftForce_defines_typeName
#define Foam_TrajectoryCollision_defines_typeName
#define Foam_VirtualMassForce_defines_typeName
#define Foam_VoidFraction_defines_typeName
/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2020-2021 OpenCFD Ltd.
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

#include "basicSprayCloud.H"

#include "makeReactingParcelCloudFunctionObjects.H"

// Kinematic
#include "makeThermoParcelForces.H" // thermo variant
#include "makeThermoParcelTurbulenceForces.H" // add turbulence variant
#include "makeParcelDispersionModels.H"
#include "makeParcelTurbulenceDispersionModels.H" // add turbulence variant
#include "makeSprayParcelInjectionModels.H" // Spray variant
#include "makeThermoParcelPatchInteractionModels.H"
#include "makeSprayParcelStochasticCollisionModels.H" // Spray variant

// Thermodynamic
#include "makeParcelHeatTransferModels.H"

// Reacting
#include "makeReactingParcelCompositionModels.H"
#include "makeReactingParcelPhaseChangeModels.H"
#include "makeReactingParcelSurfaceFilmModels.H"

// Spray
#include "DistortedSphereDragForce.H"
#include "makeSprayParcelAtomizationModels.H"
#include "makeSprayParcelBreakupModels.H"

// MPPIC sub-models
#include "makeMPPICParcelDampingModels.H"
#include "makeMPPICParcelIsotropyModels.H"
#include "makeMPPICParcelPackingModels.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makeReactingParcelCloudFunctionObjects(basicSprayCloud);

// Kinematic sub-models
makeThermoParcelForces(basicSprayCloud);
makeThermoParcelTurbulenceForces(basicSprayCloud);
makeParcelDispersionModels(basicSprayCloud);
makeParcelTurbulenceDispersionModels(basicSprayCloud);
makeSprayParcelInjectionModels(basicSprayCloud);
makeThermoParcelPatchInteractionModels(basicSprayCloud);
makeSprayParcelStochasticCollisionModels(basicSprayCloud);

// Thermo sub-models
makeParcelHeatTransferModels(basicSprayCloud);

// Reacting sub-models
makeReactingParcelCompositionModels(basicSprayCloud);
makeReactingParcelPhaseChangeModels(basicSprayCloud);
makeReactingParcelSurfaceFilmModels(basicSprayCloud);

// Spray sub-models
makeParticleForceModelType(DistortedSphereDragForce, basicSprayCloud);
makeSprayParcelAtomizationModels(basicSprayCloud);
makeSprayParcelBreakupModels(basicSprayCloud);

// MPPIC sub-models
makeMPPICParcelDampingModels(basicSprayCloud);
makeMPPICParcelIsotropyModels(basicSprayCloud);
makeMPPICParcelPackingModels(basicSprayCloud);

// ************************************************************************* //
