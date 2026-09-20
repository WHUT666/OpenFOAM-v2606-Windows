#ifndef FOAM_API_H
#define FOAM_API_H

/*
 * Per-library import/export macros for the Windows shared (DLL) build.
 *
 * <Target>_API expands to __declspec(dllexport) when compiling the library
 * that owns the symbol (CMake automatically defines <Target>_EXPORTS for
 * SHARED library targets) and to __declspec(dllimport) for consumers.
 * In static builds (FOAM_SHARED_LIBS undefined) every macro is empty.
 *
 * MSVC cannot auto-import data symbols (verified: import libraries expose
 * only __imp_X, never bare X), so every data declaration that crosses a
 * DLL boundary must carry its owning library's macro. Function calls
 * resolve through linker-generated thunks and need no annotation.
 *
 * Symbols declared by OSspecific/ and Pstream/ headers live in
 * libOpenFOAM.dll - annotate those with OpenFOAM_API.
 *
 * <Target>_TEMPLATE_IMPORT(X<args>) / <Target>_TEMPLATE_EXPORT(X<args>)
 * emit the extern-template / explicit-instantiation pair for a class
 * template instantiation that lives entirely in the owning library.
 * This is the only MSVC-legal way to import static members of a class
 * template specialization: declspec on "template<>" member declarations
 * is rejected (C2720), and annotating the primary template's member
 * breaks downstream specialization definitions (C2491).
 */

#if defined(_WIN32) && defined(FOAM_SHARED_LIBS)
#  define FOAM_EXPORT__ __declspec(dllexport)
#  define FOAM_IMPORT__ __declspec(dllimport)
#  define FOAM_TEMPLATE_EXPORT__(...) template class FOAM_EXPORT__ __VA_ARGS__
#  define FOAM_TEMPLATE_IMPORT__(...) extern template class FOAM_IMPORT__ __VA_ARGS__
#else
#  define FOAM_EXPORT__
#  define FOAM_IMPORT__
#  define FOAM_TEMPLATE_EXPORT__(...)
#  define FOAM_TEMPLATE_IMPORT__(...)
#endif


#ifdef DSMC_EXPORTS
#  define DSMC_API FOAM_EXPORT__
#  define DSMC_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define DSMC_TEMPLATE_IMPORT(...)
#else
#  define DSMC_API FOAM_IMPORT__
#  define DSMC_TEMPLATE_EXPORT(...)
#  define DSMC_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef MGridGenGAMGAgglomeration_EXPORTS
#  define MGridGenGAMGAgglomeration_API FOAM_EXPORT__
#  define MGridGenGAMGAgglomeration_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define MGridGenGAMGAgglomeration_TEMPLATE_IMPORT(...)
#else
#  define MGridGenGAMGAgglomeration_API FOAM_IMPORT__
#  define MGridGenGAMGAgglomeration_TEMPLATE_EXPORT(...)
#  define MGridGenGAMGAgglomeration_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef ODE_EXPORTS
#  define ODE_API FOAM_EXPORT__
#  define ODE_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define ODE_TEMPLATE_IMPORT(...)
#else
#  define ODE_API FOAM_IMPORT__
#  define ODE_TEMPLATE_EXPORT(...)
#  define ODE_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef OpenFOAM_EXPORTS
#  define OpenFOAM_API FOAM_EXPORT__
#  define OpenFOAM_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define OpenFOAM_TEMPLATE_IMPORT(...)
#else
#  define OpenFOAM_API FOAM_IMPORT__
#  define OpenFOAM_TEMPLATE_EXPORT(...)
#  define OpenFOAM_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef SLGThermo_EXPORTS
#  define SLGThermo_API FOAM_EXPORT__
#  define SLGThermo_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define SLGThermo_TEMPLATE_IMPORT(...)
#else
#  define SLGThermo_API FOAM_IMPORT__
#  define SLGThermo_TEMPLATE_EXPORT(...)
#  define SLGThermo_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef SloanRenumber_EXPORTS
#  define SloanRenumber_API FOAM_EXPORT__
#  define SloanRenumber_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define SloanRenumber_TEMPLATE_IMPORT(...)
#else
#  define SloanRenumber_API FOAM_IMPORT__
#  define SloanRenumber_TEMPLATE_EXPORT(...)
#  define SloanRenumber_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef VoFphaseTurbulentTransportModels_EXPORTS
#  define VoFphaseTurbulentTransportModels_API FOAM_EXPORT__
#  define VoFphaseTurbulentTransportModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define VoFphaseTurbulentTransportModels_TEMPLATE_IMPORT(...)
#else
#  define VoFphaseTurbulentTransportModels_API FOAM_IMPORT__
#  define VoFphaseTurbulentTransportModels_TEMPLATE_EXPORT(...)
#  define VoFphaseTurbulentTransportModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef adjointOptimisation_EXPORTS
#  define adjointOptimisation_API FOAM_EXPORT__
#  define adjointOptimisation_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define adjointOptimisation_TEMPLATE_IMPORT(...)
#else
#  define adjointOptimisation_API FOAM_IMPORT__
#  define adjointOptimisation_TEMPLATE_EXPORT(...)
#  define adjointOptimisation_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef atmosphericModels_EXPORTS
#  define atmosphericModels_API FOAM_EXPORT__
#  define atmosphericModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define atmosphericModels_TEMPLATE_IMPORT(...)
#else
#  define atmosphericModels_API FOAM_IMPORT__
#  define atmosphericModels_TEMPLATE_EXPORT(...)
#  define atmosphericModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef barotropicCompressibilityModel_EXPORTS
#  define barotropicCompressibilityModel_API FOAM_EXPORT__
#  define barotropicCompressibilityModel_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define barotropicCompressibilityModel_TEMPLATE_IMPORT(...)
#else
#  define barotropicCompressibilityModel_API FOAM_IMPORT__
#  define barotropicCompressibilityModel_TEMPLATE_EXPORT(...)
#  define barotropicCompressibilityModel_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef blockMesh_EXPORTS
#  define blockMesh_API FOAM_EXPORT__
#  define blockMesh_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define blockMesh_TEMPLATE_IMPORT(...)
#else
#  define blockMesh_API FOAM_IMPORT__
#  define blockMesh_TEMPLATE_EXPORT(...)
#  define blockMesh_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef chemistryModel_EXPORTS
#  define chemistryModel_API FOAM_EXPORT__
#  define chemistryModel_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define chemistryModel_TEMPLATE_IMPORT(...)
#else
#  define chemistryModel_API FOAM_IMPORT__
#  define chemistryModel_TEMPLATE_EXPORT(...)
#  define chemistryModel_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef coalCombustion_EXPORTS
#  define coalCombustion_API FOAM_EXPORT__
#  define coalCombustion_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define coalCombustion_TEMPLATE_IMPORT(...)
#else
#  define coalCombustion_API FOAM_IMPORT__
#  define coalCombustion_TEMPLATE_EXPORT(...)
#  define coalCombustion_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef combustionModels_EXPORTS
#  define combustionModels_API FOAM_EXPORT__
#  define combustionModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define combustionModels_TEMPLATE_IMPORT(...)
#else
#  define combustionModels_API FOAM_IMPORT__
#  define combustionModels_TEMPLATE_EXPORT(...)
#  define combustionModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef compressibleMultiPhaseTurbulenceModels_EXPORTS
#  define compressibleMultiPhaseTurbulenceModels_API FOAM_EXPORT__
#  define compressibleMultiPhaseTurbulenceModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define compressibleMultiPhaseTurbulenceModels_TEMPLATE_IMPORT(...)
#else
#  define compressibleMultiPhaseTurbulenceModels_API FOAM_IMPORT__
#  define compressibleMultiPhaseTurbulenceModels_TEMPLATE_EXPORT(...)
#  define compressibleMultiPhaseTurbulenceModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef compressibleTransportModels_EXPORTS
#  define compressibleTransportModels_API FOAM_EXPORT__
#  define compressibleTransportModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define compressibleTransportModels_TEMPLATE_IMPORT(...)
#else
#  define compressibleTransportModels_API FOAM_IMPORT__
#  define compressibleTransportModels_TEMPLATE_EXPORT(...)
#  define compressibleTransportModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef compressibleTurbulenceModels_EXPORTS
#  define compressibleTurbulenceModels_API FOAM_EXPORT__
#  define compressibleTurbulenceModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define compressibleTurbulenceModels_TEMPLATE_IMPORT(...)
#else
#  define compressibleTurbulenceModels_API FOAM_IMPORT__
#  define compressibleTurbulenceModels_TEMPLATE_EXPORT(...)
#  define compressibleTurbulenceModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef compressibleTwoPhaseSystem_EXPORTS
#  define compressibleTwoPhaseSystem_API FOAM_EXPORT__
#  define compressibleTwoPhaseSystem_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define compressibleTwoPhaseSystem_TEMPLATE_IMPORT(...)
#else
#  define compressibleTwoPhaseSystem_API FOAM_IMPORT__
#  define compressibleTwoPhaseSystem_TEMPLATE_EXPORT(...)
#  define compressibleTwoPhaseSystem_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef conversion_EXPORTS
#  define conversion_API FOAM_EXPORT__
#  define conversion_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define conversion_TEMPLATE_IMPORT(...)
#else
#  define conversion_API FOAM_IMPORT__
#  define conversion_TEMPLATE_EXPORT(...)
#  define conversion_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef decompose_EXPORTS
#  define decompose_API FOAM_EXPORT__
#  define decompose_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define decompose_TEMPLATE_IMPORT(...)
#else
#  define decompose_API FOAM_IMPORT__
#  define decompose_TEMPLATE_EXPORT(...)
#  define decompose_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef decompositionMethods_EXPORTS
#  define decompositionMethods_API FOAM_EXPORT__
#  define decompositionMethods_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define decompositionMethods_TEMPLATE_IMPORT(...)
#else
#  define decompositionMethods_API FOAM_IMPORT__
#  define decompositionMethods_TEMPLATE_EXPORT(...)
#  define decompositionMethods_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef distributed_EXPORTS
#  define distributed_API FOAM_EXPORT__
#  define distributed_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define distributed_TEMPLATE_IMPORT(...)
#else
#  define distributed_API FOAM_IMPORT__
#  define distributed_TEMPLATE_EXPORT(...)
#  define distributed_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef distributionModels_EXPORTS
#  define distributionModels_API FOAM_EXPORT__
#  define distributionModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define distributionModels_TEMPLATE_IMPORT(...)
#else
#  define distributionModels_API FOAM_IMPORT__
#  define distributionModels_TEMPLATE_EXPORT(...)
#  define distributionModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef dynamicFvMesh_EXPORTS
#  define dynamicFvMesh_API FOAM_EXPORT__
#  define dynamicFvMesh_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define dynamicFvMesh_TEMPLATE_IMPORT(...)
#else
#  define dynamicFvMesh_API FOAM_IMPORT__
#  define dynamicFvMesh_TEMPLATE_EXPORT(...)
#  define dynamicFvMesh_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef dynamicMesh_EXPORTS
#  define dynamicMesh_API FOAM_EXPORT__
#  define dynamicMesh_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define dynamicMesh_TEMPLATE_IMPORT(...)
#else
#  define dynamicMesh_API FOAM_IMPORT__
#  define dynamicMesh_TEMPLATE_EXPORT(...)
#  define dynamicMesh_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef engine_EXPORTS
#  define engine_API FOAM_EXPORT__
#  define engine_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define engine_TEMPLATE_IMPORT(...)
#else
#  define engine_API FOAM_IMPORT__
#  define engine_TEMPLATE_EXPORT(...)
#  define engine_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef extrudeModel_EXPORTS
#  define extrudeModel_API FOAM_EXPORT__
#  define extrudeModel_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define extrudeModel_TEMPLATE_IMPORT(...)
#else
#  define extrudeModel_API FOAM_IMPORT__
#  define extrudeModel_TEMPLATE_EXPORT(...)
#  define extrudeModel_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef faDecompose_EXPORTS
#  define faDecompose_API FOAM_EXPORT__
#  define faDecompose_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define faDecompose_TEMPLATE_IMPORT(...)
#else
#  define faDecompose_API FOAM_IMPORT__
#  define faDecompose_TEMPLATE_EXPORT(...)
#  define faDecompose_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef faOptions_EXPORTS
#  define faOptions_API FOAM_EXPORT__
#  define faOptions_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define faOptions_TEMPLATE_IMPORT(...)
#else
#  define faOptions_API FOAM_IMPORT__
#  define faOptions_TEMPLATE_EXPORT(...)
#  define faOptions_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef faReconstruct_EXPORTS
#  define faReconstruct_API FOAM_EXPORT__
#  define faReconstruct_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define faReconstruct_TEMPLATE_IMPORT(...)
#else
#  define faReconstruct_API FOAM_IMPORT__
#  define faReconstruct_TEMPLATE_EXPORT(...)
#  define faReconstruct_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef fieldFunctionObjects_EXPORTS
#  define fieldFunctionObjects_API FOAM_EXPORT__
#  define fieldFunctionObjects_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define fieldFunctionObjects_TEMPLATE_IMPORT(...)
#else
#  define fieldFunctionObjects_API FOAM_IMPORT__
#  define fieldFunctionObjects_TEMPLATE_EXPORT(...)
#  define fieldFunctionObjects_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef fileFormats_EXPORTS
#  define fileFormats_API FOAM_EXPORT__
#  define fileFormats_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define fileFormats_TEMPLATE_IMPORT(...)
#else
#  define fileFormats_API FOAM_IMPORT__
#  define fileFormats_TEMPLATE_EXPORT(...)
#  define fileFormats_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef finiteArea_EXPORTS
#  define finiteArea_API FOAM_EXPORT__
#  define finiteArea_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define finiteArea_TEMPLATE_IMPORT(...)
#else
#  define finiteArea_API FOAM_IMPORT__
#  define finiteArea_TEMPLATE_EXPORT(...)
#  define finiteArea_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef finiteVolume_EXPORTS
#  define finiteVolume_API FOAM_EXPORT__
#  define finiteVolume_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define finiteVolume_TEMPLATE_IMPORT(...)
#else
#  define finiteVolume_API FOAM_IMPORT__
#  define finiteVolume_TEMPLATE_EXPORT(...)
#  define finiteVolume_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef fluidThermophysicalModels_EXPORTS
#  define fluidThermophysicalModels_API FOAM_EXPORT__
#  define fluidThermophysicalModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define fluidThermophysicalModels_TEMPLATE_IMPORT(...)
#else
#  define fluidThermophysicalModels_API FOAM_IMPORT__
#  define fluidThermophysicalModels_TEMPLATE_EXPORT(...)
#  define fluidThermophysicalModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef forces_EXPORTS
#  define forces_API FOAM_EXPORT__
#  define forces_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define forces_TEMPLATE_IMPORT(...)
#else
#  define forces_API FOAM_IMPORT__
#  define forces_TEMPLATE_EXPORT(...)
#  define forces_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef fusedFiniteVolume_EXPORTS
#  define fusedFiniteVolume_API FOAM_EXPORT__
#  define fusedFiniteVolume_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define fusedFiniteVolume_TEMPLATE_IMPORT(...)
#else
#  define fusedFiniteVolume_API FOAM_IMPORT__
#  define fusedFiniteVolume_TEMPLATE_EXPORT(...)
#  define fusedFiniteVolume_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef fusedTurbulenceModels_EXPORTS
#  define fusedTurbulenceModels_API FOAM_EXPORT__
#  define fusedTurbulenceModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define fusedTurbulenceModels_TEMPLATE_IMPORT(...)
#else
#  define fusedTurbulenceModels_API FOAM_IMPORT__
#  define fusedTurbulenceModels_TEMPLATE_EXPORT(...)
#  define fusedTurbulenceModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef fvMotionSolvers_EXPORTS
#  define fvMotionSolvers_API FOAM_EXPORT__
#  define fvMotionSolvers_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define fvMotionSolvers_TEMPLATE_IMPORT(...)
#else
#  define fvMotionSolvers_API FOAM_IMPORT__
#  define fvMotionSolvers_TEMPLATE_EXPORT(...)
#  define fvMotionSolvers_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef fvOptions_EXPORTS
#  define fvOptions_API FOAM_EXPORT__
#  define fvOptions_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define fvOptions_TEMPLATE_IMPORT(...)
#else
#  define fvOptions_API FOAM_IMPORT__
#  define fvOptions_TEMPLATE_EXPORT(...)
#  define fvOptions_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef genericPatchFields_EXPORTS
#  define genericPatchFields_API FOAM_EXPORT__
#  define genericPatchFields_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define genericPatchFields_TEMPLATE_IMPORT(...)
#else
#  define genericPatchFields_API FOAM_IMPORT__
#  define genericPatchFields_TEMPLATE_EXPORT(...)
#  define genericPatchFields_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef geometricVoF_EXPORTS
#  define geometricVoF_API FOAM_EXPORT__
#  define geometricVoF_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define geometricVoF_TEMPLATE_IMPORT(...)
#else
#  define geometricVoF_API FOAM_IMPORT__
#  define geometricVoF_TEMPLATE_EXPORT(...)
#  define geometricVoF_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef immiscibleIncompressibleTwoPhaseMixture_EXPORTS
#  define immiscibleIncompressibleTwoPhaseMixture_API FOAM_EXPORT__
#  define immiscibleIncompressibleTwoPhaseMixture_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define immiscibleIncompressibleTwoPhaseMixture_TEMPLATE_IMPORT(...)
#else
#  define immiscibleIncompressibleTwoPhaseMixture_API FOAM_IMPORT__
#  define immiscibleIncompressibleTwoPhaseMixture_TEMPLATE_EXPORT(...)
#  define immiscibleIncompressibleTwoPhaseMixture_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef incompressibleInterPhaseTransportModels_EXPORTS
#  define incompressibleInterPhaseTransportModels_API FOAM_EXPORT__
#  define incompressibleInterPhaseTransportModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define incompressibleInterPhaseTransportModels_TEMPLATE_IMPORT(...)
#else
#  define incompressibleInterPhaseTransportModels_API FOAM_IMPORT__
#  define incompressibleInterPhaseTransportModels_TEMPLATE_EXPORT(...)
#  define incompressibleInterPhaseTransportModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef incompressibleMultiphaseSystems_EXPORTS
#  define incompressibleMultiphaseSystems_API FOAM_EXPORT__
#  define incompressibleMultiphaseSystems_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define incompressibleMultiphaseSystems_TEMPLATE_IMPORT(...)
#else
#  define incompressibleMultiphaseSystems_API FOAM_IMPORT__
#  define incompressibleMultiphaseSystems_TEMPLATE_EXPORT(...)
#  define incompressibleMultiphaseSystems_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef incompressibleTransportModels_EXPORTS
#  define incompressibleTransportModels_API FOAM_EXPORT__
#  define incompressibleTransportModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define incompressibleTransportModels_TEMPLATE_IMPORT(...)
#else
#  define incompressibleTransportModels_API FOAM_IMPORT__
#  define incompressibleTransportModels_TEMPLATE_EXPORT(...)
#  define incompressibleTransportModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef incompressibleTurbulenceModels_EXPORTS
#  define incompressibleTurbulenceModels_API FOAM_EXPORT__
#  define incompressibleTurbulenceModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define incompressibleTurbulenceModels_TEMPLATE_IMPORT(...)
#else
#  define incompressibleTurbulenceModels_API FOAM_IMPORT__
#  define incompressibleTurbulenceModels_TEMPLATE_EXPORT(...)
#  define incompressibleTurbulenceModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef initialisationFunctionObjects_EXPORTS
#  define initialisationFunctionObjects_API FOAM_EXPORT__
#  define initialisationFunctionObjects_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define initialisationFunctionObjects_TEMPLATE_IMPORT(...)
#else
#  define initialisationFunctionObjects_API FOAM_IMPORT__
#  define initialisationFunctionObjects_TEMPLATE_EXPORT(...)
#  define initialisationFunctionObjects_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef interfaceProperties_EXPORTS
#  define interfaceProperties_API FOAM_EXPORT__
#  define interfaceProperties_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define interfaceProperties_TEMPLATE_IMPORT(...)
#else
#  define interfaceProperties_API FOAM_IMPORT__
#  define interfaceProperties_TEMPLATE_EXPORT(...)
#  define interfaceProperties_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef interfaceTrackingFvMesh_EXPORTS
#  define interfaceTrackingFvMesh_API FOAM_EXPORT__
#  define interfaceTrackingFvMesh_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define interfaceTrackingFvMesh_TEMPLATE_IMPORT(...)
#else
#  define interfaceTrackingFvMesh_API FOAM_IMPORT__
#  define interfaceTrackingFvMesh_TEMPLATE_EXPORT(...)
#  define interfaceTrackingFvMesh_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef kahipDecomp_EXPORTS
#  define kahipDecomp_API FOAM_EXPORT__
#  define kahipDecomp_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define kahipDecomp_TEMPLATE_IMPORT(...)
#else
#  define kahipDecomp_API FOAM_IMPORT__
#  define kahipDecomp_TEMPLATE_EXPORT(...)
#  define kahipDecomp_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef lagrangian_EXPORTS
#  define lagrangian_API FOAM_EXPORT__
#  define lagrangian_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define lagrangian_TEMPLATE_IMPORT(...)
#else
#  define lagrangian_API FOAM_IMPORT__
#  define lagrangian_TEMPLATE_EXPORT(...)
#  define lagrangian_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef lagrangianFunctionObjects_EXPORTS
#  define lagrangianFunctionObjects_API FOAM_EXPORT__
#  define lagrangianFunctionObjects_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define lagrangianFunctionObjects_TEMPLATE_IMPORT(...)
#else
#  define lagrangianFunctionObjects_API FOAM_IMPORT__
#  define lagrangianFunctionObjects_TEMPLATE_EXPORT(...)
#  define lagrangianFunctionObjects_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef lagrangianIntermediate_EXPORTS
#  define lagrangianIntermediate_API FOAM_EXPORT__
#  define lagrangianIntermediate_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define lagrangianIntermediate_TEMPLATE_IMPORT(...)
#else
#  define lagrangianIntermediate_API FOAM_IMPORT__
#  define lagrangianIntermediate_TEMPLATE_EXPORT(...)
#  define lagrangianIntermediate_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef lagrangianSpray_EXPORTS
#  define lagrangianSpray_API FOAM_EXPORT__
#  define lagrangianSpray_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define lagrangianSpray_TEMPLATE_IMPORT(...)
#else
#  define lagrangianSpray_API FOAM_IMPORT__
#  define lagrangianSpray_TEMPLATE_EXPORT(...)
#  define lagrangianSpray_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef lagrangianTurbulence_EXPORTS
#  define lagrangianTurbulence_API FOAM_EXPORT__
#  define lagrangianTurbulence_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define lagrangianTurbulence_TEMPLATE_IMPORT(...)
#else
#  define lagrangianTurbulence_API FOAM_IMPORT__
#  define lagrangianTurbulence_TEMPLATE_EXPORT(...)
#  define lagrangianTurbulence_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef laminarFlameSpeedModels_EXPORTS
#  define laminarFlameSpeedModels_API FOAM_EXPORT__
#  define laminarFlameSpeedModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define laminarFlameSpeedModels_TEMPLATE_IMPORT(...)
#else
#  define laminarFlameSpeedModels_API FOAM_IMPORT__
#  define laminarFlameSpeedModels_TEMPLATE_EXPORT(...)
#  define laminarFlameSpeedModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef lumpedPointMotion_EXPORTS
#  define lumpedPointMotion_API FOAM_EXPORT__
#  define lumpedPointMotion_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define lumpedPointMotion_TEMPLATE_IMPORT(...)
#else
#  define lumpedPointMotion_API FOAM_IMPORT__
#  define lumpedPointMotion_TEMPLATE_EXPORT(...)
#  define lumpedPointMotion_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef meshTools_EXPORTS
#  define meshTools_API FOAM_EXPORT__
#  define meshTools_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define meshTools_TEMPLATE_IMPORT(...)
#else
#  define meshTools_API FOAM_IMPORT__
#  define meshTools_TEMPLATE_EXPORT(...)
#  define meshTools_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef metisDecomp_EXPORTS
#  define metisDecomp_API FOAM_EXPORT__
#  define metisDecomp_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define metisDecomp_TEMPLATE_IMPORT(...)
#else
#  define metisDecomp_API FOAM_IMPORT__
#  define metisDecomp_TEMPLATE_EXPORT(...)
#  define metisDecomp_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef multiphaseSystem_EXPORTS
#  define multiphaseSystem_API FOAM_EXPORT__
#  define multiphaseSystem_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define multiphaseSystem_TEMPLATE_IMPORT(...)
#else
#  define multiphaseSystem_API FOAM_IMPORT__
#  define multiphaseSystem_TEMPLATE_EXPORT(...)
#  define multiphaseSystem_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef overset_EXPORTS
#  define overset_API FOAM_EXPORT__
#  define overset_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define overset_TEMPLATE_IMPORT(...)
#else
#  define overset_API FOAM_IMPORT__
#  define overset_TEMPLATE_EXPORT(...)
#  define overset_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef pairPatchAgglomeration_EXPORTS
#  define pairPatchAgglomeration_API FOAM_EXPORT__
#  define pairPatchAgglomeration_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define pairPatchAgglomeration_TEMPLATE_IMPORT(...)
#else
#  define pairPatchAgglomeration_API FOAM_IMPORT__
#  define pairPatchAgglomeration_TEMPLATE_EXPORT(...)
#  define pairPatchAgglomeration_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef phaseCompressible_EXPORTS
#  define phaseCompressible_API FOAM_EXPORT__
#  define phaseCompressible_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define phaseCompressible_TEMPLATE_IMPORT(...)
#else
#  define phaseCompressible_API FOAM_IMPORT__
#  define phaseCompressible_TEMPLATE_EXPORT(...)
#  define phaseCompressible_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef phaseCompressibleTurbulenceModels_EXPORTS
#  define phaseCompressibleTurbulenceModels_API FOAM_EXPORT__
#  define phaseCompressibleTurbulenceModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define phaseCompressibleTurbulenceModels_TEMPLATE_IMPORT(...)
#else
#  define phaseCompressibleTurbulenceModels_API FOAM_IMPORT__
#  define phaseCompressibleTurbulenceModels_TEMPLATE_EXPORT(...)
#  define phaseCompressibleTurbulenceModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef phaseFunctionObjects_EXPORTS
#  define phaseFunctionObjects_API FOAM_EXPORT__
#  define phaseFunctionObjects_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define phaseFunctionObjects_TEMPLATE_IMPORT(...)
#else
#  define phaseFunctionObjects_API FOAM_IMPORT__
#  define phaseFunctionObjects_TEMPLATE_EXPORT(...)
#  define phaseFunctionObjects_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef phaseIncompressible_EXPORTS
#  define phaseIncompressible_API FOAM_EXPORT__
#  define phaseIncompressible_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define phaseIncompressible_TEMPLATE_IMPORT(...)
#else
#  define phaseIncompressible_API FOAM_IMPORT__
#  define phaseIncompressible_TEMPLATE_EXPORT(...)
#  define phaseIncompressible_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef ptscotchDecomp_EXPORTS
#  define ptscotchDecomp_API FOAM_EXPORT__
#  define ptscotchDecomp_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define ptscotchDecomp_TEMPLATE_IMPORT(...)
#else
#  define ptscotchDecomp_API FOAM_IMPORT__
#  define ptscotchDecomp_TEMPLATE_EXPORT(...)
#  define ptscotchDecomp_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef pyrolysisModels_EXPORTS
#  define pyrolysisModels_API FOAM_EXPORT__
#  define pyrolysisModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define pyrolysisModels_TEMPLATE_IMPORT(...)
#else
#  define pyrolysisModels_API FOAM_IMPORT__
#  define pyrolysisModels_TEMPLATE_EXPORT(...)
#  define pyrolysisModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef radiationModels_EXPORTS
#  define radiationModels_API FOAM_EXPORT__
#  define radiationModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define radiationModels_TEMPLATE_IMPORT(...)
#else
#  define radiationModels_API FOAM_IMPORT__
#  define radiationModels_TEMPLATE_EXPORT(...)
#  define radiationModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef randomProcesses_EXPORTS
#  define randomProcesses_API FOAM_EXPORT__
#  define randomProcesses_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define randomProcesses_TEMPLATE_IMPORT(...)
#else
#  define randomProcesses_API FOAM_IMPORT__
#  define randomProcesses_TEMPLATE_EXPORT(...)
#  define randomProcesses_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef reactingMultiphaseSystem_EXPORTS
#  define reactingMultiphaseSystem_API FOAM_EXPORT__
#  define reactingMultiphaseSystem_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define reactingMultiphaseSystem_TEMPLATE_IMPORT(...)
#else
#  define reactingMultiphaseSystem_API FOAM_IMPORT__
#  define reactingMultiphaseSystem_TEMPLATE_EXPORT(...)
#  define reactingMultiphaseSystem_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef reactingTwoPhaseSystem_EXPORTS
#  define reactingTwoPhaseSystem_API FOAM_EXPORT__
#  define reactingTwoPhaseSystem_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define reactingTwoPhaseSystem_TEMPLATE_IMPORT(...)
#else
#  define reactingTwoPhaseSystem_API FOAM_IMPORT__
#  define reactingTwoPhaseSystem_TEMPLATE_EXPORT(...)
#  define reactingTwoPhaseSystem_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef reactionThermophysicalModels_EXPORTS
#  define reactionThermophysicalModels_API FOAM_EXPORT__
#  define reactionThermophysicalModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define reactionThermophysicalModels_TEMPLATE_IMPORT(...)
#else
#  define reactionThermophysicalModels_API FOAM_IMPORT__
#  define reactionThermophysicalModels_TEMPLATE_EXPORT(...)
#  define reactionThermophysicalModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef reconstruct_EXPORTS
#  define reconstruct_API FOAM_EXPORT__
#  define reconstruct_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define reconstruct_TEMPLATE_IMPORT(...)
#else
#  define reconstruct_API FOAM_IMPORT__
#  define reconstruct_TEMPLATE_EXPORT(...)
#  define reconstruct_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef regionCoupling_EXPORTS
#  define regionCoupling_API FOAM_EXPORT__
#  define regionCoupling_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define regionCoupling_TEMPLATE_IMPORT(...)
#else
#  define regionCoupling_API FOAM_IMPORT__
#  define regionCoupling_TEMPLATE_EXPORT(...)
#  define regionCoupling_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef regionFaModels_EXPORTS
#  define regionFaModels_API FOAM_EXPORT__
#  define regionFaModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define regionFaModels_TEMPLATE_IMPORT(...)
#else
#  define regionFaModels_API FOAM_IMPORT__
#  define regionFaModels_TEMPLATE_EXPORT(...)
#  define regionFaModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef regionModels_EXPORTS
#  define regionModels_API FOAM_EXPORT__
#  define regionModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define regionModels_TEMPLATE_IMPORT(...)
#else
#  define regionModels_API FOAM_IMPORT__
#  define regionModels_TEMPLATE_EXPORT(...)
#  define regionModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef renumberMethods_EXPORTS
#  define renumberMethods_API FOAM_EXPORT__
#  define renumberMethods_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define renumberMethods_TEMPLATE_IMPORT(...)
#else
#  define renumberMethods_API FOAM_IMPORT__
#  define renumberMethods_TEMPLATE_EXPORT(...)
#  define renumberMethods_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef rigidBodyDynamics_EXPORTS
#  define rigidBodyDynamics_API FOAM_EXPORT__
#  define rigidBodyDynamics_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define rigidBodyDynamics_TEMPLATE_IMPORT(...)
#else
#  define rigidBodyDynamics_API FOAM_IMPORT__
#  define rigidBodyDynamics_TEMPLATE_EXPORT(...)
#  define rigidBodyDynamics_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef rigidBodyMeshMotion_EXPORTS
#  define rigidBodyMeshMotion_API FOAM_EXPORT__
#  define rigidBodyMeshMotion_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define rigidBodyMeshMotion_TEMPLATE_IMPORT(...)
#else
#  define rigidBodyMeshMotion_API FOAM_IMPORT__
#  define rigidBodyMeshMotion_TEMPLATE_EXPORT(...)
#  define rigidBodyMeshMotion_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef sampling_EXPORTS
#  define sampling_API FOAM_EXPORT__
#  define sampling_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define sampling_TEMPLATE_IMPORT(...)
#else
#  define sampling_API FOAM_IMPORT__
#  define sampling_TEMPLATE_EXPORT(...)
#  define sampling_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef saturationModel_EXPORTS
#  define saturationModel_API FOAM_EXPORT__
#  define saturationModel_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define saturationModel_TEMPLATE_IMPORT(...)
#else
#  define saturationModel_API FOAM_IMPORT__
#  define saturationModel_TEMPLATE_EXPORT(...)
#  define saturationModel_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef scotchDecomp_EXPORTS
#  define scotchDecomp_API FOAM_EXPORT__
#  define scotchDecomp_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define scotchDecomp_TEMPLATE_IMPORT(...)
#else
#  define scotchDecomp_API FOAM_IMPORT__
#  define scotchDecomp_TEMPLATE_EXPORT(...)
#  define scotchDecomp_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef sixDoFRigidBodyMotion_EXPORTS
#  define sixDoFRigidBodyMotion_API FOAM_EXPORT__
#  define sixDoFRigidBodyMotion_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define sixDoFRigidBodyMotion_TEMPLATE_IMPORT(...)
#else
#  define sixDoFRigidBodyMotion_API FOAM_IMPORT__
#  define sixDoFRigidBodyMotion_TEMPLATE_EXPORT(...)
#  define sixDoFRigidBodyMotion_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef sixDoFRigidBodyState_EXPORTS
#  define sixDoFRigidBodyState_API FOAM_EXPORT__
#  define sixDoFRigidBodyState_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define sixDoFRigidBodyState_TEMPLATE_IMPORT(...)
#else
#  define sixDoFRigidBodyState_API FOAM_IMPORT__
#  define sixDoFRigidBodyState_TEMPLATE_EXPORT(...)
#  define sixDoFRigidBodyState_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef snappyHexMesh_EXPORTS
#  define snappyHexMesh_API FOAM_EXPORT__
#  define snappyHexMesh_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define snappyHexMesh_TEMPLATE_IMPORT(...)
#else
#  define snappyHexMesh_API FOAM_IMPORT__
#  define snappyHexMesh_TEMPLATE_EXPORT(...)
#  define snappyHexMesh_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef solidChemistryModel_EXPORTS
#  define solidChemistryModel_API FOAM_EXPORT__
#  define solidChemistryModel_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define solidChemistryModel_TEMPLATE_IMPORT(...)
#else
#  define solidChemistryModel_API FOAM_IMPORT__
#  define solidChemistryModel_TEMPLATE_EXPORT(...)
#  define solidChemistryModel_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef solidParticle_EXPORTS
#  define solidParticle_API FOAM_EXPORT__
#  define solidParticle_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define solidParticle_TEMPLATE_IMPORT(...)
#else
#  define solidParticle_API FOAM_IMPORT__
#  define solidParticle_TEMPLATE_EXPORT(...)
#  define solidParticle_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef solidSpecie_EXPORTS
#  define solidSpecie_API FOAM_EXPORT__
#  define solidSpecie_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define solidSpecie_TEMPLATE_IMPORT(...)
#else
#  define solidSpecie_API FOAM_IMPORT__
#  define solidSpecie_TEMPLATE_EXPORT(...)
#  define solidSpecie_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef solidThermo_EXPORTS
#  define solidThermo_API FOAM_EXPORT__
#  define solidThermo_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define solidThermo_TEMPLATE_IMPORT(...)
#else
#  define solidThermo_API FOAM_IMPORT__
#  define solidThermo_TEMPLATE_EXPORT(...)
#  define solidThermo_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef solverFunctionObjects_EXPORTS
#  define solverFunctionObjects_API FOAM_EXPORT__
#  define solverFunctionObjects_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define solverFunctionObjects_TEMPLATE_IMPORT(...)
#else
#  define solverFunctionObjects_API FOAM_IMPORT__
#  define solverFunctionObjects_TEMPLATE_EXPORT(...)
#  define solverFunctionObjects_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef specie_EXPORTS
#  define specie_API FOAM_EXPORT__
#  define specie_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define specie_TEMPLATE_IMPORT(...)
#else
#  define specie_API FOAM_IMPORT__
#  define specie_TEMPLATE_EXPORT(...)
#  define specie_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef surfMesh_EXPORTS
#  define surfMesh_API FOAM_EXPORT__
#  define surfMesh_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define surfMesh_TEMPLATE_IMPORT(...)
#else
#  define surfMesh_API FOAM_IMPORT__
#  define surfMesh_TEMPLATE_EXPORT(...)
#  define surfMesh_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef surfaceFilmModels_EXPORTS
#  define surfaceFilmModels_API FOAM_EXPORT__
#  define surfaceFilmModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define surfaceFilmModels_TEMPLATE_IMPORT(...)
#else
#  define surfaceFilmModels_API FOAM_IMPORT__
#  define surfaceFilmModels_TEMPLATE_EXPORT(...)
#  define surfaceFilmModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef thermalBaffleModels_EXPORTS
#  define thermalBaffleModels_API FOAM_EXPORT__
#  define thermalBaffleModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define thermalBaffleModels_TEMPLATE_IMPORT(...)
#else
#  define thermalBaffleModels_API FOAM_IMPORT__
#  define thermalBaffleModels_TEMPLATE_EXPORT(...)
#  define thermalBaffleModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef thermoTools_EXPORTS
#  define thermoTools_API FOAM_EXPORT__
#  define thermoTools_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define thermoTools_TEMPLATE_IMPORT(...)
#else
#  define thermoTools_API FOAM_IMPORT__
#  define thermoTools_TEMPLATE_EXPORT(...)
#  define thermoTools_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef thermophysicalProperties_EXPORTS
#  define thermophysicalProperties_API FOAM_EXPORT__
#  define thermophysicalProperties_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define thermophysicalProperties_TEMPLATE_IMPORT(...)
#else
#  define thermophysicalProperties_API FOAM_IMPORT__
#  define thermophysicalProperties_TEMPLATE_EXPORT(...)
#  define thermophysicalProperties_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef topoChangerFvMesh_EXPORTS
#  define topoChangerFvMesh_API FOAM_EXPORT__
#  define topoChangerFvMesh_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define topoChangerFvMesh_TEMPLATE_IMPORT(...)
#else
#  define topoChangerFvMesh_API FOAM_IMPORT__
#  define topoChangerFvMesh_TEMPLATE_EXPORT(...)
#  define topoChangerFvMesh_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef turbulenceModelSchemes_EXPORTS
#  define turbulenceModelSchemes_API FOAM_EXPORT__
#  define turbulenceModelSchemes_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define turbulenceModelSchemes_TEMPLATE_IMPORT(...)
#else
#  define turbulenceModelSchemes_API FOAM_IMPORT__
#  define turbulenceModelSchemes_TEMPLATE_EXPORT(...)
#  define turbulenceModelSchemes_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef turbulenceModels_EXPORTS
#  define turbulenceModels_API FOAM_EXPORT__
#  define turbulenceModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define turbulenceModels_TEMPLATE_IMPORT(...)
#else
#  define turbulenceModels_API FOAM_IMPORT__
#  define turbulenceModels_TEMPLATE_EXPORT(...)
#  define turbulenceModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef twoPhaseMixture_EXPORTS
#  define twoPhaseMixture_API FOAM_EXPORT__
#  define twoPhaseMixture_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define twoPhaseMixture_TEMPLATE_IMPORT(...)
#else
#  define twoPhaseMixture_API FOAM_IMPORT__
#  define twoPhaseMixture_TEMPLATE_EXPORT(...)
#  define twoPhaseMixture_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef twoPhaseProperties_EXPORTS
#  define twoPhaseProperties_API FOAM_EXPORT__
#  define twoPhaseProperties_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define twoPhaseProperties_TEMPLATE_IMPORT(...)
#else
#  define twoPhaseProperties_API FOAM_IMPORT__
#  define twoPhaseProperties_TEMPLATE_EXPORT(...)
#  define twoPhaseProperties_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef twoPhaseReactingTurbulenceModels_EXPORTS
#  define twoPhaseReactingTurbulenceModels_API FOAM_EXPORT__
#  define twoPhaseReactingTurbulenceModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define twoPhaseReactingTurbulenceModels_TEMPLATE_IMPORT(...)
#else
#  define twoPhaseReactingTurbulenceModels_API FOAM_IMPORT__
#  define twoPhaseReactingTurbulenceModels_TEMPLATE_EXPORT(...)
#  define twoPhaseReactingTurbulenceModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef utilityFunctionObjects_EXPORTS
#  define utilityFunctionObjects_API FOAM_EXPORT__
#  define utilityFunctionObjects_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define utilityFunctionObjects_TEMPLATE_IMPORT(...)
#else
#  define utilityFunctionObjects_API FOAM_IMPORT__
#  define utilityFunctionObjects_TEMPLATE_EXPORT(...)
#  define utilityFunctionObjects_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef waveModels_EXPORTS
#  define waveModels_API FOAM_EXPORT__
#  define waveModels_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define waveModels_TEMPLATE_IMPORT(...)
#else
#  define waveModels_API FOAM_IMPORT__
#  define waveModels_TEMPLATE_EXPORT(...)
#  define waveModels_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#ifdef zoltanRenumber_EXPORTS
#  define zoltanRenumber_API FOAM_EXPORT__
#  define zoltanRenumber_TEMPLATE_EXPORT(...) FOAM_TEMPLATE_EXPORT__(__VA_ARGS__)
#  define zoltanRenumber_TEMPLATE_IMPORT(...)
#else
#  define zoltanRenumber_API FOAM_IMPORT__
#  define zoltanRenumber_TEMPLATE_EXPORT(...)
#  define zoltanRenumber_TEMPLATE_IMPORT(...) FOAM_TEMPLATE_IMPORT__(__VA_ARGS__)
#endif

#endif // FOAM_API_H
