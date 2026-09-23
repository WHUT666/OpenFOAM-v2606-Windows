#------------------------------------------------------------------------------
# plugins/CMakeLists.txt - community plugins (git submodules)
#
# Each entry is guarded by an EXISTS check: an uninitialised submodule
# silently skips itself. Targets whose third-party deps are unavailable are
# auto-skipped by foam_add_library/foam_add_executable (unresolved markers).
#------------------------------------------------------------------------------

set(_P "${CMAKE_SOURCE_DIR}/plugins")

function(foam_plugin dir)
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

# Populate the generated _apps/ lnInclude for a non-target directory.
# Needed when a plugin's Make/options references a sibling dir's lnInclude
# (wmakeLnInclude -u ../otherDir) that no target owns.
function(foam_sibling_lninclude dir)
    get_filename_component(abs "${dir}" ABSOLUTE BASE_DIR "${CMAKE_SOURCE_DIR}")
    if(NOT IS_DIRECTORY "${abs}")
        return()
    endif()
    foam_lndir_for("${abs}" lndir)
    execute_process(
        COMMAND "${Python3_EXECUTABLE}" -c
            "import sys; sys.path.insert(0, r'${CMAKE_SOURCE_DIR}/cmake');"
            "from wmake2cmake import make_lninclude;"
            "make_lninclude(r'${abs}', r'${lndir}', [r'${FOAM_UCRT_INCLUDE_DIR}', r'${FOAM_VC_INCLUDE_DIR}', r'${FOAM_UM_INCLUDE_DIR}', r'${FOAM_SHARED_INCLUDE_DIR}'])"
        RESULT_VARIABLE rc
    )
    if(rc EQUAL 0)
        message(STATUS "  lnInclude: ${lndir}")
    endif()
endfunction()

#------------------------------------------------------------------------------
# cfmesh - cartesian/pMesh/tetMesh mesh generation (self-contained)
#------------------------------------------------------------------------------
option(FOAM_PLUGIN_CFMESH "Build plugins/cfmesh" ON)
if(FOAM_PLUGIN_CFMESH)
    foam_plugin("${_P}/cfmesh/meshLibrary")
    foreach(_app cartesian2DMesh cartesianMesh pMesh tetMesh)
        foam_plugin("${_P}/cfmesh/executables/${_app}" TYPE EXE)
    endforeach()
endif()

#------------------------------------------------------------------------------
# avalanche - finite-area avalanche solvers (self-contained)
#------------------------------------------------------------------------------
option(FOAM_PLUGIN_AVALANCHE "Build plugins/avalanche" ON)
if(FOAM_PLUGIN_AVALANCHE)
    foam_plugin("${_P}/avalanche/src/avalanche")
    foreach(_app
            solvers/faParkerFukushimaFoam
            solvers/faSavageHutterFoam
            solvers/faTwoLayerAvalancheFoam
            utilities/gridToSTL
            utilities/releaseAreaMapping
            utilities/slopeMesh)
        foam_plugin("${_P}/avalanche/applications/${_app}" TYPE EXE)
    endforeach()
endif()

#------------------------------------------------------------------------------
# turbulence-community - community turbulence model libraries
#------------------------------------------------------------------------------
option(FOAM_PLUGIN_TURBULENCE_COMMUNITY "Build plugins/turbulence-community" ON)
if(FOAM_PLUGIN_TURBULENCE_COMMUNITY)
    set(_tc "${_P}/turbulence-community")
    foreach(_lib
            EllipticBlending/kEpsilonLagEB
            HelicitySpalartAllmaras/compressible
            HelicitySpalartAllmaras/incompressible
            MachineLearningTurbulenceModels/of-turbulence-models
            SpalartAllmarasRC/compressible
            SpalartAllmarasRC/incompressible
            dynamicSmagorinsky/dynamicSmagorinsky
            gammaSST/compressible
            gammaSST/incompressible
            kOmegaSSTCND/kOmegaSSTCND/compressible
            kOmegaSSTCND/kOmegaSSTCND/incompressible
            kOmegaSSTPDA/kOmegaSSTPDA
            kOmegaSSTPDA/kOmegaSSTPDA_compressible
            libWallModelledLES)
        foam_plugin("${_tc}/${_lib}")
    endforeach()

    # SAH model sources live in the sibling turbulenceModels/ dir, referenced
    # as -I../turbulenceModels/lnInclude (upstream: wmakeLnInclude -u)
    foam_sibling_lninclude("${_tc}/HelicitySpalartAllmaras/turbulenceModels")
endif()

#------------------------------------------------------------------------------
# research - community solver codes (self-contained)
#------------------------------------------------------------------------------
option(FOAM_PLUGIN_RESEARCH "Build plugins/research" ON)
if(FOAM_PLUGIN_RESEARCH)
    set(_r "${_P}/research")
    foam_plugin("${_r}/HelicalForce/code")
    foreach(_app
            FastRK/code
            PRECISE/code/preciseFoam1
            PRECISE/code/preciseFoam2
            PRECISE/code/preciseFoam3
            PRECISE/code/preciseFoam4
            PRECISE/code/preciseFoam5
            redistributedResistivityAlgorithm/code/aniPorousPimpleFoam
            redistributedResistivityAlgorithm/code/porousPimpleFoam)
        foam_plugin("${_r}/${_app}" TYPE EXE)
    endforeach()
endif()

#------------------------------------------------------------------------------
# bindings - vtk-hdf needs VTK (unavailable -> auto-skip)
#------------------------------------------------------------------------------
foam_plugin("${_P}/bindings/vtk-hdf/src/library")
foam_plugin("${_P}/bindings/vtk-hdf/src/writer")
foam_plugin("${_P}/bindings/vtk-hdf/applications/foamToVTKHDF" TYPE EXE)
