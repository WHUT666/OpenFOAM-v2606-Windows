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
    if(NOT A_TYPE)
        # Auto-dispatch: an EXE= assignment in Make/files marks an app
        file(STRINGS "${dir}/Make/files" _exe REGEX "^[ \t]*(SEXE|EXE)[ \t]*=")
        if(_exe)
            set(A_TYPE EXE)
        else()
            set(A_TYPE LIB)
        endif()
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

option(FOAM_MODULE_OPENQBMM "Build modules/OpenQBMM (~50 targets)" ON)
if(FOAM_MODULE_OPENQBMM)
    # OpenQBMM's phase-system libs are mutually referential (Linux ELF
    # allows cyclic DT_NEEDED; MSVC does not). Break the hub->spoke
    # edges into raw import-lib paths; spoke->hub target deps keep the
    # hub built first. Fresh build trees need the spoke import libs
    # seeded once - see etc/seed-cyclic-stubs.ps1.
    set(FOAM_LINK_AS_FILE_pbePhaseModels
        pbeInterfacialModels pbeTurbulenceModels)
    set(FOAM_LINK_AS_FILE_pdPhaseSystem
        pdEulerianInterfacialModels pdTurbulenceModels)
    # foam_module auto-dispatches LIB vs EXE from Make/files;
    # applications/ contains both solver exes and helper libraries.
    # Targets nest at varying depth: recurse the whole tree and pick
    # Make/files entries (GLOB_RECURSE * doesn't span path components).
    file(GLOB_RECURSE _qbmm_files "${_M}/OpenQBMM/*")
    foreach(_f ${_qbmm_files})
        if(NOT _f MATCHES "/(src|applications|test)/.*/Make/files$")
            continue()
        endif()
        get_filename_component(_d "${_f}" DIRECTORY)
        get_filename_component(_d "${_d}" DIRECTORY)
        foam_module("${_d}")
    endforeach()
endif()
