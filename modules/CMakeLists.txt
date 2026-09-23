#------------------------------------------------------------------------------
# modules/CMakeLists.txt - upstream modules (git submodules)
#
# Uninitialised submodules skip themselves (EXISTS guard). Modules needing
# vendored third-party SDKs default OFF:
#   adios            -> ADIOS2 (sinclude'd vars expand empty -> compile fails)
#   visualization    -> ParaView SDK / Catalyst / VTK
#   OpenQBMM         -> self-contained but ~50 targets, not yet verified
#------------------------------------------------------------------------------

set(_M "${CMAKE_SOURCE_DIR}/modules")

function(foam_module dir)
    cmake_parse_arguments(A "" "TYPE" "" ${ARGN})
    if(NOT EXISTS "${dir}/Make/files")
        message(STATUS "  [skip] ${dir} (no Make/files - submodule not checked out?)")
        return()
    endif()
    if(A_TYPE STREQUAL "EXE")
        foam_add_executable("${dir}")
    else()
        foam_add_library("${dir}")
    endif()
endfunction()

# external-solver/petsc4Foam - auto-skips via unresolved @PETSC_DIR@ marker
foam_module("${_M}/external-solver/src/petsc4Foam")

option(FOAM_MODULE_ADIOS "Build modules/adios (needs vendored ADIOS2)" OFF)
if(FOAM_MODULE_ADIOS)
    foam_module("${_M}/adios/src/adiosFoam")
    foam_module("${_M}/adios/src/adiosWrite")
endif()

option(FOAM_MODULE_VISUALIZATION "Build modules/visualization (needs ParaView/VTK)" OFF)
if(FOAM_MODULE_VISUALIZATION)
    foam_module("${_M}/visualization/src/runTimePostProcessing/dummy")
    foam_module("${_M}/visualization/src/paraview-plugins/common")
    foam_module("${_M}/visualization/src/paraview-plugins/blockMeshReader/library")
    foam_module("${_M}/visualization/src/paraview-plugins/foamReader/library")
endif()

option(FOAM_MODULE_OPENQBMM "Build modules/OpenQBMM (~50 targets, unverified)" OFF)
if(FOAM_MODULE_OPENQBMM)
    file(GLOB_RECURSE _qbmm_files "${_M}/OpenQBMM/src/*/Make/files")
    foreach(_f ${_qbmm_files})
        get_filename_component(_d "${_f}" DIRECTORY)
        get_filename_component(_d "${_d}" DIRECTORY)
        foam_module("${_d}")
    endforeach()
    file(GLOB_RECURSE _qbmm_files "${_M}/OpenQBMM/applications/*/Make/files")
    foreach(_f ${_qbmm_files})
        get_filename_component(_d "${_f}" DIRECTORY)
        get_filename_component(_d "${_d}" DIRECTORY)
        foam_module("${_d}" TYPE EXE)
    endforeach()
endif()
