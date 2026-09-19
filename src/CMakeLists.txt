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
target_compile_definitions(OSspecific PRIVATE FOAM_USE_WINDOWS_PSAPI
    FOAM_STATIC_BUILD)

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
set(_openfoam_syslibs psapi)
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
    set_property(GLOBAL APPEND PROPERTY FOAM_ALL_LIB_TARGETS OpenFOAM Pstream)

else()
    # --- shared: two-pass bootstrap link (like wmake/mingw) ---
    add_library(Pstream_objs OBJECT ${PSTREAM_SOURCES})
    foam_apply_common(Pstream_objs)
    target_include_directories(Pstream_objs PRIVATE
        "${PSTREAM_DIR_ABS}" "${PSTREAM_LNINCLUDE}" ${PSTREAM_INCS})
    target_link_libraries(Pstream_objs PRIVATE ${_pstream_syslibs})

    add_library(OpenFOAM_bootstrap SHARED
        ${OPENFOAM_SOURCES} ${FOAM_CONFIG_SRC}
        $<TARGET_OBJECTS:OSspecific>
        $<TARGET_OBJECTS:Pstream_objs>
    )
    set_target_properties(OpenFOAM_bootstrap PROPERTIES
        PREFIX "lib"
        OUTPUT_NAME "OpenFOAM_bootstrap"
        WINDOWS_EXPORT_ALL_SYMBOLS ON
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bootstrap"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bootstrap"
    )
    foam_apply_common(OpenFOAM_bootstrap)
    target_include_directories(OpenFOAM_bootstrap PRIVATE
        "${OPENFOAM_DIR}" "${OPENFOAM_LNINCLUDE}" ${OPENFOAM_INCS})
    target_compile_definitions(OpenFOAM_bootstrap PRIVATE ${OPENFOAM_DEFINES})
    target_link_libraries(OpenFOAM_bootstrap PRIVATE
        ${_openfoam_syslibs} ${_pstream_syslibs})

    add_library(Pstream SHARED ${PSTREAM_SOURCES})
    set_target_properties(Pstream PROPERTIES
        PREFIX "lib"
        WINDOWS_EXPORT_ALL_SYMBOLS ON
        RUNTIME_OUTPUT_DIRECTORY "${FOAM_LIBBIN_DIR}/${FOAM_MPI_SUBDIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${FOAM_IMPBIN_DIR}/${FOAM_MPI_SUBDIR}"
    )
    foam_apply_common(Pstream)
    target_include_directories(Pstream PRIVATE
        "${PSTREAM_DIR_ABS}" "${PSTREAM_LNINCLUDE}" ${PSTREAM_INCS})
    target_link_libraries(Pstream PRIVATE
        OpenFOAM_bootstrap ${_pstream_syslibs})

    add_library(OpenFOAM SHARED
        ${OPENFOAM_SOURCES} ${FOAM_CONFIG_SRC} ${OPENFOAM_GENSRCS}
        $<TARGET_OBJECTS:OSspecific>)
    set_target_properties(OpenFOAM PROPERTIES
        PREFIX "lib"
        WINDOWS_EXPORT_ALL_SYMBOLS ON
        RUNTIME_OUTPUT_DIRECTORY "${FOAM_LIBBIN_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${FOAM_IMPBIN_DIR}"
    )
    foam_apply_common(OpenFOAM)
    target_include_directories(OpenFOAM PRIVATE
        "${OPENFOAM_DIR}" "${OPENFOAM_LNINCLUDE}" ${OPENFOAM_INCS})
    target_compile_definitions(OpenFOAM PRIVATE ${OPENFOAM_DEFINES})
    target_link_libraries(OpenFOAM PRIVATE
        Pstream ${_openfoam_syslibs})
    add_dependencies(OpenFOAM Pstream)
    set_property(GLOBAL APPEND PROPERTY FOAM_ALL_LIB_TARGETS OpenFOAM Pstream)
endif()

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
foam_lib(dummyThirdParty/scotchDecomp)
foam_lib(dummyThirdParty/ptscotchDecomp)
foam_lib(dummyThirdParty/metisDecomp)
foam_lib(dummyThirdParty/kahipDecomp)
foam_lib(dummyThirdParty/MGridGen)
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
foam_lib(fvAgglomerationMethods/MGridGenGamgAgglomeration OPTIONAL)
foam_lib(waveModels)
foam_lib(engine)
foam_lib(conversion)
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
foam_lib(lagrangian/molecularDynamics/molecularDynamics OPTIONAL)
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

# MPI-dependent parallel libs
foam_lib(parallel/decompose/ptscotchDecomp OPTIONAL)
foam_lib(parallel/decompose/scotchDecomp OPTIONAL)
foam_lib(parallel/decompose/metisDecomp OPTIONAL)
foam_lib(parallel/decompose/kahipDecomp OPTIONAL)
foam_lib(parallel/decompose/decompose)
