#------------------------------------------------------------------------------
# FoamMacros.cmake - helpers mapping wmake targets to CMake
#
#   foam_add_library(<dir> [ALIAS <name>] [OBJECT|SHARED|STATIC])
#   foam_add_executable(<dir>)
#   foam_lninclude(<dir>)
#
# Requires: Python3, FOAM_SRC_DIR, FOAM_LNINCLUDE_ROOT
#------------------------------------------------------------------------------

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(FOAM_WMAKE2CMAKE "${CMAKE_CURRENT_LIST_DIR}/wmake2cmake.py")

# Resolve @MARKER@ paths produced by wmake2cmake into real paths
function(foam_resolve_marker outvar token)
    set(val "${token}")
    string(REPLACE "@FOAM_LIBBIN@" "${FOAM_LIBBIN_DIR}" val "${val}")
    string(REPLACE "@FOAM_USER_LIBBIN@" "${FOAM_LIBBIN_DIR}" val "${val}")
    string(REPLACE "@FOAM_MPI_LIBBIN@" "${FOAM_LIBBIN_DIR}/${FOAM_MPI_SUBDIR}" val "${val}")
    string(REPLACE "@FOAM_APPBIN@" "${FOAM_APPBIN_DIR}" val "${val}")
    string(REPLACE "@FOAM_USER_APPBIN@" "${FOAM_APPBIN_DIR}" val "${val}")
    string(REPLACE "@OBJECTS_DIR@" "${FOAM_GEN_DIR}" val "${val}")
    string(REPLACE "@FOAM_MPI@" "${FOAM_MPI_NAME}" val "${val}")
    # Third-party markers - resolve to configured dirs (may stay markers if unset)
    foreach(mp PTSCOTCH_INC_DIR PTSCOTCH_LIB_DIR SCOTCH_INC_DIR SCOTCH_LIB_DIR
               KAHIP_INC_DIR KAHIP_LIB_DIR METIS_INC_DIR METIS_LIB_DIR
               ZOLTAN_INC_DIR ZOLTAN_LIB_DIR FFTW_INC_DIR FFTW_LIB_DIR
               CGAL_INC_DIR CGAL_LIB_DIR BOOST_INC_DIR BOOST_LIB_DIR PETSC_DIR
               GMP_INC_DIR GMP_LIB_DIR MPFR_INC_DIR MPFR_LIB_DIR)
        if(DEFINED ${mp} AND ${mp})
            string(REPLACE "@${mp}@" "${${mp}}" val "${val}")
        endif()
    endforeach()
    set(${outvar} "${val}" PARENT_SCOPE)
endfunction()


# Map a -l<name> link item (or @path@/libName.o) to a CMake target or flag
function(foam_map_lib outvar item)
    set(lib "${item}")
    foam_resolve_marker(lib "${lib}")

    if(lib MATCHES "^@" OR lib STREQUAL "")
        # Unresolved third-party marker - drop it (feature disabled)
        set(${outvar} "" PARENT_SCOPE)
        return()
    endif()

    # Explicit object/archive path e.g. @FOAM_LIBBIN@/libOSspecific.o
    if(lib MATCHES "\\.o$|\\.obj$|\\.a$|\\.lib$")
        if(lib MATCHES "libOSspecific\\.o")
            set(${outvar} "$<TARGET_OBJECTS:OSspecific>" PARENT_SCOPE)
        elseif(lib MATCHES "libPstream\\.o")
            set(${outvar} "$<TARGET_OBJECTS:Pstream_objs>" PARENT_SCOPE)
        elseif(EXISTS "${lib}")
            set(${outvar} "${lib}" PARENT_SCOPE)
        else()
            set(${outvar} "" PARENT_SCOPE)
        endif()
        return()
    endif()

    # Pure POSIX libs that don't exist on Windows
    if(lib MATCHES "^(dl|rt|pthread|m|util|resolv|nsl|readline|history|ncurses)$")
        set(${outvar} "" PARENT_SCOPE)
        return()
    endif()

    # Known system/third-party mappings
    if(lib STREQUAL "z" OR lib STREQUAL "zlib")
        if(TARGET ZLIB::ZLIB)
            set(${outvar} "ZLIB::ZLIB" PARENT_SCOPE)
        else()
            set(${outvar} "" PARENT_SCOPE)
        endif()
        return()
    endif()
    if(lib MATCHES "^(mpi|msmpi|ms-mpi)" OR lib STREQUAL "${FOAM_MPI_NAME}")
        if(FOAM_HAVE_MPI)
            set(${outvar} "MPI::MPI_CXX" PARENT_SCOPE)
        else()
            set(${outvar} "" PARENT_SCOPE)
        endif()
        return()
    endif()
    if(lib STREQUAL "psapi" OR lib STREQUAL "Psapi")
        set(${outvar} "psapi" PARENT_SCOPE)
        return()
    endif()
    if(lib MATCHES "^(ws2_32|wsock32|iphlpapi|userenv|advapi32|shell32|ole32|dbghelp|imagehlp|version|shlwapi)$")
        set(${outvar} "${lib}" PARENT_SCOPE)
        return()
    endif()
    # Third party optional libs - only resolve when a matching target exists
    if(lib MATCHES "^(scotch|ptscotch|ptscotcherrexit|scotcherrexit|metis|kahip|zoltan|parmetis|fftw3|fftw3f|gsl|gslcblas|mgridGen|petsc|hdf5|adios2.*|boost_.*|gmp|mpfr)$")
        set(tgt "")
        if(TARGET ${lib})
            set(tgt "${lib}")
        elseif(TARGET ${lib}::${lib})
            set(tgt "${lib}::${lib}")
        endif()
        set(${outvar} "${tgt}" PARENT_SCOPE)
        return()
    endif()

    # Otherwise assume an OpenFOAM library target of the same name
    set(${outvar} "${lib}" PARENT_SCOPE)
endfunction()


# Run wmake2cmake over <dir> and load the generated fragment.
# Sets FOAM_* vars in parent scope.
function(foam_parse_dir dir)
    get_filename_component(abs "${dir}" ABSOLUTE BASE_DIR "${FOAM_SRC_DIR}")
    file(RELATIVE_PATH rel "${FOAM_SRC_DIR}" "${abs}")
    string(REPLACE "/" "_" relu "${rel}")
    set(gen "${CMAKE_BINARY_DIR}/cmake-gen/${relu}.cmake")
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/cmake-gen")

    set(lninc "")
    string(REGEX MATCH "^(.*)/Make$" _m "${abs}")
    set(lndir "${FOAM_LNINCLUDE_ROOT}/${rel}")
    set(lninc "${lndir}")

    execute_process(
        COMMAND "${Python3_EXECUTABLE}" "${FOAM_WMAKE2CMAKE}"
                --dir "${abs}"
                --out "${gen}"
                --lninclude "${lndir}"
                --sysinclude "${FOAM_UCRT_INCLUDE_DIR}"
                --sysinclude "${FOAM_VC_INCLUDE_DIR}"
                --sysinclude "${FOAM_UM_INCLUDE_DIR}"
                --sysinclude "${FOAM_SHARED_INCLUDE_DIR}"
        RESULT_VARIABLE rc
        OUTPUT_VARIABLE out
        ERROR_VARIABLE err
        TIMEOUT 120
    )
    if(NOT rc EQUAL 0)
        message(FATAL_ERROR "wmake2cmake failed for ${dir}:\n${err}")
    endif()
    if(NOT EXISTS "${gen}")
        message(FATAL_ERROR "wmake2cmake produced no output for ${dir}")
    endif()
    include("${gen}")
    # Export vars to caller
    foreach(v FOAM_SOURCES FOAM_FLEX_SOURCES FOAM_LEMON_SOURCES FOAM_TARGET_NAME
              FOAM_TARGET_TYPE FOAM_TARGET_OUTDIR FOAM_TARGET_DIR
              FOAM_INCLUDES FOAM_DEFINES FOAM_CFLAGS FOAM_LIBS
              FOAM_LIBDIRS FOAM_MISSING_SOURCES)
        set(${v} "${${v}}" PARENT_SCOPE)
    endforeach()
    set(FOAM_LNINCLUDE_DIR "${lndir}" PARENT_SCOPE)
endfunction()


# Translate parsed FOAM_INCLUDES entries to real dirs
function(foam_resolve_includes outvar)
    set(res "")
    foreach(inc ${ARGN})
        foam_resolve_marker(i "${inc}")
        if(i MATCHES "^@")
            continue()  # unresolved marker -> skip
        endif()
        # Map <src>/X/lnInclude -> generated lnInclude root for <src>/X
        if(i MATCHES "/lnInclude$")
            string(REGEX REPLACE "/lnInclude$" "" isrc "${i}")
            file(RELATIVE_PATH r "${FOAM_SRC_DIR}" "${isrc}")
            set(i "${FOAM_LNINCLUDE_ROOT}/${r}")
        endif()
        list(APPEND res "${i}")
    endforeach()
    set(${outvar} "${res}" PARENT_SCOPE)
endfunction()


# Common per-target setup: PROJECT_INC equivalents + gen dir
function(foam_apply_common name)
    target_include_directories(${name} PRIVATE
        "${FOAM_LNINCLUDE_ROOT}/OpenFOAM"
        "${FOAM_LNINCLUDE_ROOT}/OSspecific/MSwindows"
        "${FOAM_GEN_DIR}"
        "${FOAM_SRC_DIR}/OpenFOAM/include"          # OSspecific.H etc.
        "${FOAM_SRC_DIR}/OSspecific/MSwindows"      # MSwindows.H etc.
    )
endfunction()


function(foam_enable_static_registration name)
    get_target_property(target_type ${name} TYPE)
    if(FOAM_STATIC_LIBS AND target_type STREQUAL "STATIC_LIBRARY")
        set_property(TARGET ${name} APPEND PROPERTY
            INTERFACE_LINK_OPTIONS
            "-WHOLEARCHIVE:$<TARGET_FILE:${name}>"
        )
    endif()
endfunction()


#------------------------------------------------------------------------------
# foam_gen_lemon(<outvar> <name>)
#   Emit custom commands for FOAM_LEMON_SOURCES (*.lyy-m4 -> m4 -> lemon -> .cc)
#   and append generated sources to <outvar> in caller scope.
#------------------------------------------------------------------------------
function(foam_gen_lemon outvar name)
    set(gens "")
    foreach(f ${FOAM_LEMON_SOURCES})
        get_filename_component(fbase "${f}" NAME_WE)
        get_filename_component(fdir "${f}" DIRECTORY)
        set(gdir "${FOAM_GEN_DIR}/lemon/${name}")
        set(lyy "${gdir}/${fbase}.lyy")
        set(cc "${gdir}/${fbase}.cc")

        # m4 include dirs: parser dir, target-root include/, OpenFOAM include/
        set(m4args "-I" "${fdir}" "-I" "${FOAM_SRC_DIR}/OpenFOAM/include")
        file(RELATIVE_PATH rel "${FOAM_SRC_DIR}" "${fdir}")
        string(REGEX REPLACE "/.*$" "" top "${rel}")
        if(EXISTS "${FOAM_SRC_DIR}/${top}/include")
            list(APPEND m4args "-I" "${FOAM_SRC_DIR}/${top}/include")
        endif()

        if(f MATCHES "m4$")
            add_custom_command(
                OUTPUT "${lyy}"
                COMMAND "${CMAKE_COMMAND}" -E make_directory "${gdir}"
                COMMAND "${FOAM_M4_EXE}" ${m4args} "${f}" > "${lyy}"
                DEPENDS "${f}"
                COMMENT "m4 ${fbase}.lyy-m4 -> ${lyy}"
                VERBATIM
            )
        else()
            add_custom_command(
                OUTPUT "${lyy}"
                COMMAND "${CMAKE_COMMAND}" -E copy "${f}" "${lyy}"
                DEPENDS "${f}"
            )
        endif()

        if(f MATCHES "\\.ly$")
            set(lemon_args "")
        else()
            set(lemon_args "-ecc")
        endif()
        # Generated parser code includes OpenFOAM C++ headers - always C++
        set(outf "${cc}")
        add_custom_command(
            OUTPUT "${outf}"
            COMMAND "${FOAM_LEMON_EXE}" "-T${FOAM_LEMPAR}" "-d${gdir}"
                    ${lemon_args} "-Dm4" "${lyy}"
            DEPENDS "${lyy}"
            COMMENT "lemon ${fbase}.lyy -> ${outf}"
            VERBATIM
        )
        set_source_files_properties("${outf}" PROPERTIES LANGUAGE CXX)
        list(APPEND gens "${outf}")
    endforeach()
    set(${outvar} "${gens}" PARENT_SCOPE)
endfunction()


# foam_add_library(<dir> [NAME <n>] [OBJECT])
#------------------------------------------------------------------------------
function(foam_add_library dir)
    cmake_parse_arguments(A "OBJECT;NOGLOBAL" "NAME" "" ${ARGN})
    foam_parse_dir("${dir}")

    set(name "${FOAM_TARGET_NAME}")
    if(A_NAME)
        set(name "${A_NAME}")
    endif()

    # Missing third-party SDK (CGAL, scotch, ...): headers cannot resolve,
    # so the target can never compile - skip it entirely.
    foreach(i ${FOAM_INCLUDES})
        foam_resolve_marker(_im "${i}")
        if(_im MATCHES "^@")
            message(STATUS "  [skip] ${dir} - unavailable dep ${_im}")
            return()
        endif()
    endforeach()

    if(NOT FOAM_SOURCES AND NOT FOAM_FLEX_SOURCES)
        message(STATUS "  [skip] ${dir} - no sources")
        return()
    endif()

    # Source list: handle .Cver (configure) and .L (flex)
    set(srcs "")
    foreach(s ${FOAM_SOURCES})
        if(NOT s MATCHES "\\.Cver$")
            list(APPEND srcs "${s}")
        endif()
    endforeach()
    # Flex sources: generate .C with win_flex into the gen dir
    foreach(f ${FOAM_FLEX_SOURCES})
        get_filename_component(fbase "${f}" NAME_WE)
        set(fout "${FOAM_GEN_DIR}/flex/${name}/${fbase}.C")
        add_custom_command(
            OUTPUT "${fout}"
            COMMAND "${CMAKE_COMMAND}" -E make_directory
                    "${FOAM_GEN_DIR}/flex/${name}"
            COMMAND "${FOAM_FLEX_EXE}" --c++ "--outfile=${fout}" "${f}"
            DEPENDS "${f}"
            COMMENT "flex ${fbase}.L -> ${fout}"
            VERBATIM
        )
        list(APPEND srcs "${fout}")
    endforeach()
    # Lemon parser sources: generate .cc via m4+lemon into the gen dir
    foam_gen_lemon(gensrcs "${name}")
    list(APPEND srcs ${gensrcs})

    # MSVC treats ".C" as C - force C++ for all sources
    set_source_files_properties(${srcs} PROPERTIES LANGUAGE CXX)

    if(A_OBJECT)
        add_library(${name} OBJECT ${srcs})
    elseif(FOAM_STATIC_LIBS)
        add_library(${name} STATIC ${srcs})
        set_target_properties(${name} PROPERTIES
            PREFIX "lib"
            ARCHIVE_OUTPUT_DIRECTORY "${FOAM_LIBBIN_DIR}"
        )
    else()
        add_library(${name} SHARED ${srcs})
        set_target_properties(${name} PROPERTIES
            PREFIX "lib"
            WINDOWS_EXPORT_ALL_SYMBOLS ON
            RUNTIME_OUTPUT_DIRECTORY "${FOAM_LIBBIN_DIR}"
            LIBRARY_OUTPUT_DIRECTORY "${FOAM_LIBBIN_DIR}"
            ARCHIVE_OUTPUT_DIRECTORY "${FOAM_IMPBIN_DIR}"
        )
    endif()
    foam_apply_common(${name})
    foam_enable_static_registration(${name})

    # Include dirs: own lnInclude + global OpenFOAM/OSspecific + parsed.
    # Order matches wmake: EXE_INC dirs first, own lnInclude last, so that
    # same-basename headers in other libs resolve like upstream (eg
    # finiteVolume/surfaceInterpolate.H vs functionObjects/field one)
    target_include_directories(${name} PRIVATE "${FOAM_TARGET_DIR}")
    if(FOAM_FLEX_SOURCES)
        # FlexLexer.h for generated lexers
        target_include_directories(${name} PRIVATE "${FOAM_FLEX_INCLUDE_DIR}")
    endif()
    foam_resolve_includes(incs ${FOAM_INCLUDES})
    target_include_directories(${name} PRIVATE ${incs})
    target_include_directories(${name} PRIVATE "${FOAM_LNINCLUDE_DIR}")

    foreach(d ${FOAM_DEFINES})
        if(d MATCHES "^__UNDEF__")
            string(REGEX REPLACE "^__UNDEF__" "" d "${d}")
        else()
            target_compile_definitions(${name} PRIVATE "${d}")
        endif()
    endforeach()

    # Link libraries
    set(linklibs "")
    foreach(l ${FOAM_LIBS})
        foam_map_lib(t "${l}")
        if(t)
            list(APPEND linklibs "${t}")
        endif()
    endforeach()
    if(NOT A_OBJECT)
        # PUBLIC for static libs so transitive deps propagate to exes
        if(FOAM_STATIC_LIBS)
            target_link_libraries(${name} PUBLIC ${linklibs})
        else()
            target_link_libraries(${name} PRIVATE ${linklibs})
        endif()
    endif()

    set(${name}_LNINCLUDE "${FOAM_LNINCLUDE_DIR}" PARENT_SCOPE)
    message(STATUS "  [lib] ${name} (${dir})")
endfunction()


#------------------------------------------------------------------------------
# foam_add_executable(<dir> [NAME <n>])
#------------------------------------------------------------------------------
function(foam_add_executable dir)
    cmake_parse_arguments(A "" "NAME" "" ${ARGN})
    foam_parse_dir("${dir}")
    set(name "${FOAM_TARGET_NAME}")
    if(A_NAME)
        set(name "${A_NAME}")
    endif()
    foreach(i ${FOAM_INCLUDES})
        foam_resolve_marker(_im "${i}")
        if(_im MATCHES "^@")
            message(STATUS "  [skip] ${dir} - unavailable dep ${_im}")
            return()
        endif()
    endforeach()
    if(NOT FOAM_SOURCES)
        message(STATUS "  [skip] ${dir} - no sources")
        return()
    endif()

    set(exesrcs ${FOAM_SOURCES})
    foreach(f ${FOAM_FLEX_SOURCES})
        get_filename_component(fbase "${f}" NAME_WE)
        set(fout "${FOAM_GEN_DIR}/flex/${name}/${fbase}.C")
        add_custom_command(
            OUTPUT "${fout}"
            COMMAND "${CMAKE_COMMAND}" -E make_directory
                    "${FOAM_GEN_DIR}/flex/${name}"
            COMMAND "${FOAM_FLEX_EXE}" --c++ "--outfile=${fout}" "${f}"
            DEPENDS "${f}"
            COMMENT "flex ${fbase}.L -> ${fout}"
            VERBATIM
        )
        list(APPEND exesrcs "${fout}")
    endforeach()
    foam_gen_lemon(exegens "${name}")
    list(APPEND exesrcs ${exegens})
    set_source_files_properties(${exesrcs} PROPERTIES LANGUAGE CXX)
    # Avoid collision with a same-named library target (eg blockMesh)
    set(exename "${name}")
    if(TARGET ${name})
        set(name "${name}App")
    endif()
    add_executable(${name} ${exesrcs})
    set_target_properties(${name} PROPERTIES
        OUTPUT_NAME "${exename}"
        RUNTIME_OUTPUT_DIRECTORY "${FOAM_APPBIN_DIR}")
    foam_apply_common(${name})
    if(FOAM_FLEX_SOURCES)
        # FlexLexer.h for generated lexers
        target_include_directories(${name} PRIVATE "${FOAM_FLEX_INCLUDE_DIR}")
    endif()
    foam_resolve_includes(incs ${FOAM_INCLUDES})
    target_include_directories(${name} PRIVATE ${incs})
    # App dir must come AFTER library includes: on a case-insensitive FS
    # <CorrectPhi.H> would otherwise resolve to the local "correctPhi.H"
    # fragment. Quoted "..." includes still find local headers via the
    # including file's directory, so this matches upstream semantics.
    target_include_directories(${name} PRIVATE "${FOAM_TARGET_DIR}")
    target_include_directories(${name} PRIVATE "${FOAM_LNINCLUDE_DIR}")
    foreach(d ${FOAM_DEFINES})
        if(NOT d MATCHES "^__UNDEF__")
            target_compile_definitions(${name} PRIVATE "${d}")
        endif()
    endforeach()
    set(linklibs "")
    foreach(l ${FOAM_LIBS})
        foam_map_lib(t "${l}")
        if(t)
            list(APPEND linklibs "${t}")
        endif()
    endforeach()
    # Static builds: each Foam library propagates its own /WHOLEARCHIVE via
    # INTERFACE_LINK_OPTIONS, so linking the direct libs (plus OpenFOAM)
    # whole-archives the full transitive dependency closure and keeps all
    # self-registering objects (runTimeSelection) alive.
    target_link_libraries(${name} PRIVATE ${linklibs} OpenFOAM)
    message(STATUS "  [app] ${name} (${dir})")
endfunction()
