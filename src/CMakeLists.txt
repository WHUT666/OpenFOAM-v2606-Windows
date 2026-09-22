#------------------------------------------------------------------------------
# src/CMakeLists.txt - OpenFOAM libraries, mirroring src/Allwmake order
#------------------------------------------------------------------------------

# Each entry: relative dir under src/ containing Make/files.
# FOAM_ENABLE_<TAG>=OFF skips it. Third-party-dependent libs default OFF.

set(FOAM_LIB_LIST "")

function(foam_lib dir)
    cmake_parse_arguments(A "OPTIONAL;OBJECT;BOOTSTRAP" "NAME;ENABLE_VAR" "" ${ARGN})
    set(optname "FOAM_ENABLE_${A_ENABLE_VAR}")
    if(NOT A_ENABLE_VAR)
        # derive option name from dir
        string(REPLACE "/" "_" oname "${dir}")
        set(optname "FOAM_ENABLE_${oname}")
    endif()
    if(A_OPTIONAL)
        option(${optname} "Build ${dir}" OFF)
    else()
        option(${optname} "Build ${dir}" ON)
    endif()
    if(NOT ${optname})
        # Generate lnInclude anyway: dummy/stub libraries may need
        # these headers even when the real backend is disabled.
        if(EXISTS "${FOAM_SRC_DIR}/${dir}/Make/files")
            foam_parse_dir("${dir}")
        endif()
        message(STATUS "  [off] ${dir}")
        return()
    endif()
    if(NOT EXISTS "${FOAM_SRC_DIR}/${dir}/Make/files")
        # Header-only tree (wmakeLnInclude -u): still emit lnInclude so
        # dependents can resolve includes; no target is created.
        foam_parse_dir("${dir}")
        message(STATUS "  [hdrs] ${dir}")
        return()
    endif()
    if(A_OBJECT)
        foam_add_library(${dir} OBJECT)
    else()
        foam_add_library(${dir})
    endif()
    list(APPEND FOAM_LIB_LIST "${dir}")
    set(FOAM_LIB_LIST "${FOAM_LIB_LIST}" PARENT_SCOPE)
endfunction()

#------------------------------------------------------------------------------
# 1. OSspecific (object library - libo equivalent)
#------------------------------------------------------------------------------
foam_parse_dir("${FOAM_SRC_DIR}/OSspecific/MSwindows")
set_source_files_properties(${FOAM_SOURCES} PROPERTIES LANGUAGE CXX)
add_library(OSspecific OBJECT ${FOAM_SOURCES})
foam_apply_common(OSspecific)
target_include_directories(OSspecific PRIVATE
    "${FOAM_TARGET_DIR}"
    "${FOAM_LNINCLUDE_DIR}"
)
target_compile_definitions(OSspecific PRIVATE FOAM_USE_WINDOWS_PSAPI)
if(FOAM_STATIC_LIBS)
    # Static fallback inside dlOpen(): resolve plugins from the process image
    target_compile_definitions(OSspecific PRIVATE FOAM_STATIC_BUILD)
else()
    # Objects link into libOpenFOAM.dll - its data decls must dllexport,
    # not dllimport (CMake only auto-defines OpenFOAM_EXPORTS on the
    # OpenFOAM target itself, not on this OBJECT library)
    target_compile_definitions(OSspecific PRIVATE OpenFOAM_EXPORTS)
endif()

#------------------------------------------------------------------------------
# 2. Pstream - dummy or MPI.
#
#    OpenFOAM <-> Pstream have a cyclic dependency.
#    - STATIC build: static lib cycles are fine for MSVC's linker; just
#      link them against each other.
#    - SHARED build: mirror wmake's two-pass link via a throwaway
#      OpenFOAM_bootstrap DLL that embeds the Pstream objects.
#------------------------------------------------------------------------------
set(PSTREAM_DIR "dummy")
if(FOAM_HAVE_MPI)
    set(PSTREAM_DIR "mpi")
endif()

foam_parse_dir("${FOAM_SRC_DIR}/Pstream/${PSTREAM_DIR}")
set(PSTREAM_SOURCES ${FOAM_SOURCES})
set_source_files_properties(${PSTREAM_SOURCES} PROPERTIES LANGUAGE CXX)
set(PSTREAM_DIR_ABS "${FOAM_TARGET_DIR}")
set(PSTREAM_LNINCLUDE "${FOAM_LNINCLUDE_DIR}")
foam_resolve_includes(PSTREAM_INCS ${FOAM_INCLUDES})

#------------------------------------------------------------------------------
# 3. libOpenFOAM
#------------------------------------------------------------------------------
foam_parse_dir("${FOAM_SRC_DIR}/OpenFOAM")
set(OPENFOAM_SOURCES ${FOAM_SOURCES})
set(OPENFOAM_DIR "${FOAM_TARGET_DIR}")
set(OPENFOAM_LNINCLUDE "${FOAM_LNINCLUDE_DIR}")
foam_resolve_includes(OPENFOAM_INCS ${FOAM_INCLUDES})
set(OPENFOAM_DEFINES ${FOAM_DEFINES})
foam_gen_lemon(OPENFOAM_GENSRCS OpenFOAM)

# --- configure foamConfig.Cver -> generated .cc ---
set(FOAM_CONFIG_SRC "")
foreach(s ${OPENFOAM_SOURCES})
    if(s MATCHES "\\.Cver$")
        get_filename_component(base "${s}" NAME_WE)
        set(gen "${FOAM_GEN_DIR}/${base}.Cver.cc")
        file(READ "${s}" _cver)
        string(REPLACE "@PATCH@" "${FOAM_VERSION_PATCH}" _cver "${_cver}")
        string(REPLACE "@BUILD@" "${FOAM_VERSION_BUILD}" _cver "${_cver}")
        string(REPLACE "@VERSION@" "${FOAM_VERSION_STR}" _cver "${_cver}")
        file(WRITE "${gen}" "${_cver}")
        list(APPEND FOAM_CONFIG_SRC "${gen}")
        list(REMOVE_ITEM OPENFOAM_SOURCES "${s}")
    endif()
endforeach()
set_source_files_properties(${OPENFOAM_SOURCES} PROPERTIES LANGUAGE CXX)

set(_pstream_syslibs "")
if(FOAM_HAVE_MPI)
    set(_pstream_syslibs MPI::MPI_CXX)
endif()
set(_openfoam_syslibs psapi Dbghelp)
if(HAVE_LIBZ_DEFINE)
    list(APPEND _openfoam_syslibs ZLIB::ZLIB)
endif()

if(FOAM_STATIC_LIBS)
    # --- static libs: cyclic dep resolves at final exe link ---
    add_library(Pstream STATIC ${PSTREAM_SOURCES})
    set_target_properties(Pstream PROPERTIES PREFIX "lib"
        ARCHIVE_OUTPUT_DIRECTORY "${FOAM_LIBBIN_DIR}/${FOAM_MPI_SUBDIR}")
    foam_apply_common(Pstream)
    target_include_directories(Pstream PRIVATE
        "${PSTREAM_DIR_ABS}" "${PSTREAM_LNINCLUDE}" ${PSTREAM_INCS})
    target_link_libraries(Pstream PUBLIC ${_pstream_syslibs})

    add_library(OpenFOAM STATIC
        ${OPENFOAM_SOURCES} ${FOAM_CONFIG_SRC} ${OPENFOAM_GENSRCS}
        $<TARGET_OBJECTS:OSspecific>)
    set_target_properties(OpenFOAM PROPERTIES PREFIX "lib"
        ARCHIVE_OUTPUT_DIRECTORY "${FOAM_LIBBIN_DIR}")
    foam_apply_common(OpenFOAM)
    target_include_directories(OpenFOAM PRIVATE
        "${OPENFOAM_DIR}" "${OPENFOAM_LNINCLUDE}" ${OPENFOAM_INCS})
    target_compile_definitions(OpenFOAM PRIVATE ${OPENFOAM_DEFINES}
        # Baked-in resource dir so etc/caseDicts resolve without env setup
        FOAM_CONFIGURED_PROJECT_DIR="${CMAKE_SOURCE_DIR}")
    target_link_libraries(OpenFOAM PUBLIC Pstream ${_openfoam_syslibs})
    target_link_libraries(Pstream PUBLIC OpenFOAM)

else()
    # --- shared: fold the MPI Pstream implementation into libOpenFOAM.dll ---
    # libPstream is never dlopen'd by name (only Make/files reference it) and
    # msmpi is the only backend, so merging eliminates the OpenFOAM<->Pstream
    # link cycle without a throwaway bootstrap DLL (whose WINDOWS_EXPORT_ALL
    # .exp generation hit LNK1106 anyway on ~25k symbols).
    add_library(OpenFOAM SHARED
        ${OPENFOAM_SOURCES} ${FOAM_CONFIG_SRC} ${OPENFOAM_GENSRCS}
        ${PSTREAM_SOURCES}
        $<TARGET_OBJECTS:OSspecific>)
    set_target_properties(OpenFOAM PROPERTIES
        PREFIX "lib"
        RUNTIME_OUTPUT_DIRECTORY "${FOAM_LIBBIN_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${FOAM_IMPBIN_DIR}"
    )
    foam_export_all(OpenFOAM)
    foam_apply_common(OpenFOAM)
    target_include_directories(OpenFOAM PRIVATE
        "${OPENFOAM_DIR}" "${OPENFOAM_LNINCLUDE}" ${OPENFOAM_INCS}
        "${PSTREAM_DIR_ABS}" "${PSTREAM_LNINCLUDE}" ${PSTREAM_INCS})
    target_compile_definitions(OpenFOAM PRIVATE ${OPENFOAM_DEFINES}
        FOAM_CONFIGURED_PROJECT_DIR="${CMAKE_SOURCE_DIR}")
    target_link_libraries(OpenFOAM PRIVATE
        ${_openfoam_syslibs} ${_pstream_syslibs})

    # Name-compat shim so link lists mentioning Pstream still resolve
    add_library(Pstream INTERFACE)
    target_link_libraries(Pstream INTERFACE OpenFOAM)
endif()

foam_enable_static_registration(Pstream)
foam_enable_static_registration(OpenFOAM)

#------------------------------------------------------------------------------
# 4. Frequently needed base libraries
#------------------------------------------------------------------------------
foam_lib(fileFormats)
foam_lib(surfMesh)
foam_lib(meshTools)
foam_lib(finiteArea)
foam_lib(finiteVolume)
foam_lib(fused/finiteVolume)
foam_lib(mesh/blockMesh)
foam_lib(mesh/extrudeModel)
foam_lib(dynamicMesh)
foam_lib(genericPatchFields)
foam_lib(parallel/decompose/decompositionMethods)
foam_lib(parallel/decompose/faDecompose)
foam_lib(parallel/reconstruct/reconstruct)
foam_lib(parallel/reconstruct/faReconstruct)
foam_lib(parallel/distributed)
# Decomposition backends: real implementation when the third-party SDK is
# available, otherwise the dummy stub (same target name).
set(_scotch_decomp_default OFF)
if(SCOTCH_INC_DIR)
    set(_scotch_decomp_default ON)
endif()
option(FOAM_ENABLE_parallel_decompose_scotchDecomp "Build scotchDecomp" ${_scotch_decomp_default})
if(FOAM_ENABLE_parallel_decompose_scotchDecomp)
    foam_lib(parallel/decompose/scotchDecomp)
else()
    foam_lib(dummyThirdParty/scotchDecomp)
endif()

set(_ptscotch_decomp_default OFF)
if(PTSCOTCH_INC_DIR AND FOAM_HAVE_MPI)
    set(_ptscotch_decomp_default ON)
endif()
option(FOAM_ENABLE_parallel_decompose_ptscotchDecomp "Build ptscotchDecomp" ${_ptscotch_decomp_default})
if(FOAM_ENABLE_parallel_decompose_ptscotchDecomp)
    foam_lib(parallel/decompose/ptscotchDecomp)
else()
    foam_lib(dummyThirdParty/ptscotchDecomp)
endif()

set(_metis_decomp_default OFF)
if(METIS_INC_DIR)
    set(_metis_decomp_default ON)
endif()
option(FOAM_ENABLE_parallel_decompose_metisDecomp "Build metisDecomp" ${_metis_decomp_default})
if(FOAM_ENABLE_parallel_decompose_metisDecomp)
    foam_lib(parallel/decompose/metisDecomp)
else()
    foam_lib(dummyThirdParty/metisDecomp)
endif()

set(_kahip_decomp_default OFF)
if(KAHIP_INC_DIR)
    set(_kahip_decomp_default ON)
endif()
option(FOAM_ENABLE_parallel_decompose_kahipDecomp "Build kahipDecomp" ${_kahip_decomp_default})
if(FOAM_ENABLE_parallel_decompose_kahipDecomp)
    foam_lib(parallel/decompose/kahipDecomp)
else()
    foam_lib(dummyThirdParty/kahipDecomp)
endif()

# MGridGen: real sources vendored under thirdparty/mgridgen build the
# 'mgrid' target; the dummy stub provides the same MGridGen() symbol and
# must not be built alongside it.
if(NOT TARGET mgrid)
    foam_lib(dummyThirdParty/MGridGen)
endif()
foam_lib(fvOptions)
foam_lib(lagrangian/basic)
foam_lib(lagrangian/distributionModels)
foam_lib(renumber/zoltanRenumber OPTIONAL)
foam_lib(renumber/renumberMethods)
foam_lib(renumber/SloanRenumber)
foam_lib(dynamicFvMesh)
foam_lib(topoChangerFvMesh)
foam_lib(sampling)
foam_lib(transportModels/incompressible)
foam_lib(transportModels/compressible)
foam_lib(transportModels/geometricVoF)
foam_lib(transportModels/interfaceProperties)
foam_lib(transportModels/twoPhaseMixture)
foam_lib(transportModels/immiscibleIncompressibleTwoPhaseMixture)
foam_lib(transportModels/twoPhaseProperties)
foam_lib(randomProcesses)
foam_lib(ODE)
foam_lib(thermophysicalModels/specie)
foam_lib(thermophysicalModels/solidSpecie)
foam_lib(thermophysicalModels/thermophysicalProperties)
foam_lib(thermophysicalModels/basic)
foam_lib(thermophysicalModels/reactionThermo)
foam_lib(thermophysicalModels/laminarFlameSpeed)
foam_lib(thermophysicalModels/chemistryModel)
foam_lib(thermophysicalModels/barotropicCompressibilityModel)
foam_lib(thermophysicalModels/SLGThermo)
foam_lib(thermophysicalModels/solidThermo)
foam_lib(thermophysicalModels/solidChemistryModel)
foam_lib(thermophysicalModels/radiation)
foam_lib(TurbulenceModels/turbulenceModels)
foam_lib(TurbulenceModels/incompressible)
foam_lib(TurbulenceModels/compressible)
foam_lib(TurbulenceModels/schemes)
foam_lib(TurbulenceModels/phaseCompressible)
foam_lib(TurbulenceModels/phaseIncompressible)
foam_lib(fused/turbulenceModels)
foam_lib(combustionModels)
foam_lib(overset)
foam_lib(fvMotionSolver)
foam_lib(mesh/snappyHexMesh)
foam_lib(fvAgglomerationMethods/AllwmakeTest OPTIONAL)
foam_lib(fvAgglomerationMethods/pairPatchAgglomeration)
# MGridGen GAMG agglomerator - needs the vendored 'mgrid' target
set(_mgridgen_agglom_default OFF)
if(TARGET mgrid)
    set(_mgridgen_agglom_default ON)
endif()
option(FOAM_ENABLE_fvAgglomerationMethods_MGridGenGamgAgglomeration "Build MGridGenGamgAgglomeration" ${_mgridgen_agglom_default})
if(FOAM_ENABLE_fvAgglomerationMethods_MGridGenGamgAgglomeration)
    foam_lib(fvAgglomerationMethods/MGridGenGamgAgglomeration)
    # Static build: controlDict 'libs' cannot load it at runtime - fold it
    # into finiteVolume so every solver registers 'agglomerator MGridGen'.
    # Shared build: it is a loadable plugin (libs "libMGridGenGAMGAgglomeration")
    # and must NOT be folded in - that would create a finiteVolume<->MGridGen
    # link cycle which CMake forbids for shared libraries.
    if(FOAM_STATIC_LIBS)
        target_link_libraries(finiteVolume PUBLIC MGridGenGAMGAgglomeration)
    endif()
endif()
foam_lib(waveModels)
foam_lib(engine)
foam_lib(conversion)
foam_lib(conversion/ccm)
foam_lib(phaseSystemModels/reactingEuler/saturationModels)
foam_lib(phaseSystemModels/reactingEuler/multiphaseSystem)
foam_lib(phaseSystemModels/reactingEuler/twoPhaseSystem)
foam_lib(phaseSystemModels/reactingEuler/twoPhaseCompressibleTurbulenceModels)
foam_lib(phaseSystemModels/multiphaseInter/phasesSystem)
foam_lib(phaseSystemModels/multiphaseInter/compressibleMultiPhaseTurbulenceModels)
foam_lib(phaseSystemModels/multiphaseEuler/multiphaseSystem)
foam_lib(phaseSystemModels/twoPhaseEuler/twoPhaseSystem)
foam_lib(phaseSystemModels/twoPhaseEuler/phaseCompressibleTurbulenceModels)
foam_lib(phaseSystemModels/twoPhaseInter/VoFphaseIncompressibleTurbulenceModels)
foam_lib(phaseSystemModels/twoPhaseInter/incompressibleInterPhaseTransportModel)
foam_lib(phaseSystemModels/reactingEuler/parallelTasks OPTIONAL)
foam_lib(functionObjects/field)
foam_lib(functionObjects/forces)
foam_lib(functionObjects/initialisation)
foam_lib(functionObjects/utilities)
foam_lib(functionObjects/solvers)
foam_lib(functionObjects/phaseSystems)
foam_lib(functionObjects/lagrangian)
foam_lib(thermoTools)
foam_lib(regionModels/surfaceFilmModels)
foam_lib(regionModels/surfaceFilmModels/derivedFvPatchFields/wallFunctions)
foam_lib(regionModels/pyrolysisModels)
foam_lib(regionModels/regionCoupling)
foam_lib(regionModels/thermalBaffleModels)
foam_lib(regionModels/regionModel)
foam_lib(faOptions)
foam_lib(regionFaModels)
foam_lib(lagrangian/solidParticle)
foam_lib(lagrangian/intermediate)
foam_lib(lagrangian/turbulence)
foam_lib(lagrangian/spray)
foam_lib(lagrangian/DSMC)
foam_lib(lagrangian/coalCombustion)
foam_lib(lagrangian/molecularDynamics/molecule)
foam_lib(lagrangian/molecularDynamics/potential)
foam_lib(lagrangian/molecularDynamics/molecularMeasurements)
foam_lib(lagrangian/parcel OPTIONAL)
foam_lib(lumpedPointMotion)
foam_lib(sixDoFRigidBodyMotion)
foam_lib(sixDoFRigidBodyState)
foam_lib(rigidBodyDynamics)
foam_lib(rigidBodyMeshMotion)
foam_lib(atmosphericModels)
foam_lib(optimisation/adjointOptimisation/adjoint)
foam_lib(dynamicFaMesh/interfaceTrackingFvMesh)
foam_lib(finiteArea/faMeshDecomposition OPTIONAL)

# Parallel decompose/reconstruct aggregate libs
# (scotch/ptscotch/metis/kahip backends are dispatched above)
foam_lib(parallel/decompose/decompose)
