#------------------------------------------------------------------------------
# CMakeLists.txt - OpenFOAM for Windows (MSVC / VS2022)
#
# Translates the wmake build system (Make/files, Make/options) into CMake
# targets. See cmake/wmake2cmake.py and cmake/FoamMacros.cmake.
#
# Usage:
#   cmake -S . -B build -G "Visual Studio 17 2022" -A x64
#   cmake --build build --config Release
#
# Options:
#   FOAM_MPI        = dummy | msmpi   (default: dummy)
#   FOAM_LABEL_SIZE = 32 | 64         (default: 32)
#   FOAM_PRECISION  = DP | SP | SPDP  (default: DP)
#   FOAM_ENABLE_LIBZ = ON/OFF         (default: ON - fetches zlib if needed)
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)
project(OpenFOAM LANGUAGES C CXX)

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------
set(FOAM_MPI "msmpi" CACHE STRING "MPI implementation: dummy|msmpi")
set_property(CACHE FOAM_MPI PROPERTY STRINGS dummy msmpi)
set(FOAM_LABEL_SIZE "32" CACHE STRING "Label size: 32|64")
set_property(CACHE FOAM_LABEL_SIZE PROPERTY STRINGS 32 64)
set(FOAM_PRECISION "DP" CACHE STRING "Precision: DP|SP|SPDP")
set_property(CACHE FOAM_PRECISION PROPERTY STRINGS DP SP SPDP)
option(FOAM_ENABLE_LIBZ "Use zlib for compressed IO" ON)
option(FOAM_BUILD_APPS "Build applications" OFF)
# Static is the default on MSVC: the toolchain lacks MinGW's auto-import,
# so cross-DLL data references (OpenFOAM's many global registrars) would
# each need __declspec(dllimport) annotations. Static libs + /WHOLEARCHIVE
# keep the runTimeSelection mechanism working.
option(FOAM_STATIC_LIBS "Build static libs instead of DLLs" ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#------------------------------------------------------------------------------
# Layout
#------------------------------------------------------------------------------
set(FOAM_PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(FOAM_SRC_DIR "${FOAM_PROJECT_DIR}/src")
set(FOAM_APP_SRC_DIR "${FOAM_PROJECT_DIR}/applications")
set(FOAM_MODULES_DIR "${FOAM_PROJECT_DIR}/modules")
set(FOAM_PLUGINS_DIR "${FOAM_PROJECT_DIR}/plugins")
set(FOAM_LNINCLUDE_ROOT "${CMAKE_BINARY_DIR}/lnInclude")
set(FOAM_GEN_DIR "${CMAKE_BINARY_DIR}/generated")
set(FOAM_LIBBIN_DIR "${CMAKE_BINARY_DIR}/lib")
set(FOAM_IMPBIN_DIR "${CMAKE_BINARY_DIR}/imp")   # import libs
set(FOAM_APPBIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(FOAM_MPI_NAME "${FOAM_MPI}")
if(FOAM_MPI STREQUAL "dummy")
    set(FOAM_MPI_SUBDIR "dummy")
else()
    set(FOAM_MPI_SUBDIR "${FOAM_MPI}")
endif()

file(MAKE_DIRECTORY "${FOAM_LNINCLUDE_ROOT}" "${FOAM_GEN_DIR}")

# Vendored win_flex (winflexbison) for *.L lexer sources on Windows
set(FOAM_FLEX_EXE "${FOAM_PROJECT_DIR}/thirdparty/winflexbison/win_flex.exe")
set(FOAM_FLEX_INCLUDE_DIR "${FOAM_PROJECT_DIR}/thirdparty/winflexbison")

# Vendored lemon + m4 for *.ly/*.lyy/*.lyy-m4 expression-parser sources
set(FOAM_LEMON_EXE "${FOAM_PROJECT_DIR}/thirdparty/lemon/lemon.exe")
set(FOAM_LEMPAR "${FOAM_PROJECT_DIR}/wmake/etc/lempar.c")
set(FOAM_M4_EXE "${FOAM_PROJECT_DIR}/thirdparty/m4/m4.exe")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(FoamMacros)

#------------------------------------------------------------------------------
# Compiler configuration (MSVC)
#------------------------------------------------------------------------------
if(NOT MSVC)
    message(WARNING "This build is intended for MSVC/VS2022. Other toolchains are untested.")
endif()

add_compile_options(
    /MP                 # parallel compile
    /bigobj             # large object files (template-heavy TUs)
    /EHsc               # normal C++ exceptions
    /Zc:__cplusplus     # correct __cplusplus macro
    /Zc:throwingNew
    /utf-8
    /wd4018 /wd4068 /wd4100 /wd4127 /wd4146 /wd4244 /wd4250 /wd4251
    /wd4267 /wd4290 /wd4305 /wd4309 /wd4319 /wd4351 /wd4456 /wd4458
    /wd4482 /wd4503 /wd4512 /wd4522 /wd4661 /wd4800 /wd4910 /wd4996
    /wd4065 /wd4805 /wd4005
    /FImsvcCompat.h     # force-include the MSVC compatibility header
)

add_compile_definitions(
    _CRT_SECURE_NO_WARNINGS
    _CRT_NON_CONFORMING_WCSTOK
    _USE_MATH_DEFINES
    NOMINMAX
    WIN32_LEAN_AND_MEAN
    _WIN32_WINNT=0x0A00
    NOGDI               # avoid GDI Polygon()/ERROR clashes
    _WINDOWS
    # wmake GFLAGS equivalents
    OPENFOAM=2606
    WM_${FOAM_PRECISION}
    WM_LABEL_SIZE=${FOAM_LABEL_SIZE}
    WM_LITTLE_ENDIAN
    NoRepository
)

# Where the compat header lives
include_directories(BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/compat")

#------------------------------------------------------------------------------
# Locate the UCRT include dir (for merged-header shims that need the real
# C library headers, e.g. <string.h> colliding with Foam's string.H on a
# case-insensitive filesystem)
#------------------------------------------------------------------------------
find_path(FOAM_UCRT_INCLUDE_DIR
    NAMES string.h
    PATHS
        "$ENV{WindowsSdkDir}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/ucrt"
        "C:/Program Files (x86)/Windows Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/ucrt"
        "C:/Program Files/Windows Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/ucrt"
        "D:/Windows Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/ucrt"
    NO_DEFAULT_PATH
)
if(NOT FOAM_UCRT_INCLUDE_DIR)
    # Fallback: ask the compiler driver where its headers live
    get_filename_component(_cldir "${CMAKE_CXX_COMPILER}" DIRECTORY)
    get_filename_component(_msvc "${_cldir}/../../.." ABSOLUTE)
    find_path(FOAM_UCRT_INCLUDE_DIR NAMES string.h
        PATHS "${_msvc}/include")
endif()
file(TO_CMAKE_PATH "${FOAM_UCRT_INCLUDE_DIR}" FOAM_UCRT_INCLUDE_DIR)
message(STATUS "UCRT include dir: ${FOAM_UCRT_INCLUDE_DIR}")

# MSVC toolset include dir (for the same merged-header purpose)
get_filename_component(_cldir "${CMAKE_CXX_COMPILER}" DIRECTORY)
get_filename_component(_msvcroot "${_cldir}/../../.." ABSOLUTE)
find_path(FOAM_VC_INCLUDE_DIR NAMES cstdio
    PATHS "${_msvcroot}/include" NO_DEFAULT_PATH)
if(NOT FOAM_VC_INCLUDE_DIR)
    find_path(FOAM_VC_INCLUDE_DIR NAMES cstdio PATHS "${_msvcroot}/include")
endif()
file(TO_CMAKE_PATH "${FOAM_VC_INCLUDE_DIR}" FOAM_VC_INCLUDE_DIR)
message(STATUS "VC++ include dir: ${FOAM_VC_INCLUDE_DIR}")

# Windows SDK um/shared headers (windows.h etc.) for collision detection
find_path(FOAM_UM_INCLUDE_DIR NAMES windows.h
    PATHS
        "$ENV{WindowsSdkDir}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um"
        "C:/Program Files (x86)/Windows Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um"
        "C:/Program Files/Windows Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um"
        "D:/Windows Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um"
    NO_DEFAULT_PATH)
find_path(FOAM_SHARED_INCLUDE_DIR NAMES windef.h
    PATHS
        "$ENV{WindowsSdkDir}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/shared"
        "C:/Program Files (x86)/Windows Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/shared"
        "D:/Windows Kits/10/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/shared"
    NO_DEFAULT_PATH)

#------------------------------------------------------------------------------
# Version info -> foamConfig.Cver
#------------------------------------------------------------------------------
set(FOAM_VERSION_API 2606)
if(EXISTS "${FOAM_PROJECT_DIR}/META-INFO/api-info")
    file(STRINGS "${FOAM_PROJECT_DIR}/META-INFO/api-info" _api_lines)
    foreach(l ${_api_lines})
        if(l MATCHES "^api *= *([0-9]+)")
            set(FOAM_VERSION_API "${CMAKE_MATCH_1}")
        endif()
        if(l MATCHES "^patch *= *\"?([^\"]+)\"?")
            set(FOAM_VERSION_PATCH "${CMAKE_MATCH_1}")
        endif()
        if(l MATCHES "^version *= *\"?([^\"]+)\"?")
            set(FOAM_VERSION_STR "${CMAKE_MATCH_1}")
        endif()
    endforeach()
endif()
if(NOT FOAM_VERSION_PATCH)
    set(FOAM_VERSION_PATCH "0")
endif()
if(NOT FOAM_VERSION_STR)
    set(FOAM_VERSION_STR "v${FOAM_VERSION_API}")
endif()
set(FOAM_VERSION_BUILD "${FOAM_VERSION_STR}-${FOAM_VERSION_PATCH}")

#------------------------------------------------------------------------------
# Third-party: zlib
#------------------------------------------------------------------------------
if(FOAM_ENABLE_LIBZ)
    # Prefer vendored zlib (thirdparty/zlib), else system ZLIB
    set(_vendored_zlib "${FOAM_PROJECT_DIR}/thirdparty/zlib")
    if(EXISTS "${_vendored_zlib}/include/zlib.h"
       AND EXISTS "${_vendored_zlib}/lib/zlib1.lib")
        add_library(ZLIB::ZLIB SHARED IMPORTED GLOBAL)
        set_target_properties(ZLIB::ZLIB PROPERTIES
            IMPORTED_LOCATION "${_vendored_zlib}/bin/zlib1.dll"
            IMPORTED_IMPLIB "${_vendored_zlib}/lib/zlib1.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${_vendored_zlib}/include"
        )
        set(ZLIB_FOUND TRUE)
        message(STATUS "Using vendored zlib: ${_vendored_zlib}")
    else()
        find_package(ZLIB QUIET)
    endif()
    if(ZLIB_FOUND)
        set(HAVE_LIBZ_DEFINE "HAVE_LIBZ")
    endif()
endif()

#------------------------------------------------------------------------------
# Third-party: FFTW (prebuilt Windows binaries in thirdparty/fftw)
#------------------------------------------------------------------------------
set(FFTW_DIR "${FOAM_PROJECT_DIR}/thirdparty/fftw")
if(EXISTS "${FFTW_DIR}/fftw3.h" AND EXISTS "${FFTW_DIR}/libfftw3-3.lib")
    set(FFTW_INC_DIR "${FFTW_DIR}")
    set(FFTW_LIB_DIR "${FFTW_DIR}")
    add_library(fftw3 SHARED IMPORTED GLOBAL)
    set_target_properties(fftw3 PROPERTIES
        IMPORTED_LOCATION "${FFTW_LIB_DIR}/libfftw3-3.dll"
        IMPORTED_IMPLIB "${FFTW_LIB_DIR}/libfftw3-3.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${FFTW_INC_DIR}"
    )
    message(STATUS "Using vendored FFTW: ${FFTW_DIR}")
endif()

#------------------------------------------------------------------------------
# Third-party: Boost (header-only for SloanRenumber / CGAL consumers)
#------------------------------------------------------------------------------
set(BOOST_DIR "${FOAM_PROJECT_DIR}/thirdparty/boost")
if(EXISTS "${BOOST_DIR}/boost/config.hpp")
    set(BOOST_INC_DIR "${BOOST_DIR}")
    set(BOOST_LIB_DIR "")
    message(STATUS "Using vendored Boost headers: ${BOOST_DIR}")
endif()

#------------------------------------------------------------------------------
# Third-party: METIS (mesh decomposition) - optional
#------------------------------------------------------------------------------
set(METIS_INC_DIR "")
set(METIS_LIB_DIR "")
set(METIS_DIR "${FOAM_PROJECT_DIR}/thirdparty/metis")
if(EXISTS "${METIS_DIR}/include/metis.h" AND EXISTS "${METIS_DIR}/lib/metis.lib")
    set(METIS_INC_DIR "${METIS_DIR}/include")
    set(METIS_LIB_DIR "${METIS_DIR}/lib")
    if(NOT TARGET metis)
        add_library(metis UNKNOWN IMPORTED GLOBAL)
        set_target_properties(metis PROPERTIES
            IMPORTED_LOCATION "${METIS_LIB_DIR}/metis.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${METIS_INC_DIR}"
            # conda-forge metis carries pre-UCRT stdio refs (__iob_func etc)
            INTERFACE_LINK_LIBRARIES "legacy_stdio_definitions.lib")
    endif()
    message(STATUS "Using vendored METIS: ${METIS_DIR}")
endif()

#------------------------------------------------------------------------------
# Third-party: SCOTCH / PTSCOTCH (mesh decomposition) - optional
#------------------------------------------------------------------------------
set(SCOTCH_INC_DIR "")
set(SCOTCH_LIB_DIR "")
set(SCOTCH_DIR "${FOAM_PROJECT_DIR}/thirdparty/scotch")
if(EXISTS "${SCOTCH_DIR}/include/scotch.h" AND EXISTS "${SCOTCH_DIR}/lib/scotch.lib")
    set(SCOTCH_INC_DIR "${SCOTCH_DIR}/include")
    set(SCOTCH_LIB_DIR "${SCOTCH_DIR}/lib")
    if(NOT TARGET scotch)
        add_library(scotch UNKNOWN IMPORTED GLOBAL)
        set_target_properties(scotch PROPERTIES
            IMPORTED_LOCATION "${SCOTCH_LIB_DIR}/scotch.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${SCOTCH_INC_DIR}"
            # conda-forge splits error handling into separate archives
            INTERFACE_LINK_LIBRARIES
                "${SCOTCH_LIB_DIR}/scotcherr.lib;${SCOTCH_LIB_DIR}/scotcherrexit.lib")
    endif()
    message(STATUS "Using vendored SCOTCH: ${SCOTCH_DIR}")
endif()

set(PTSCOTCH_INC_DIR "")
set(PTSCOTCH_LIB_DIR "")
set(PTSCOTCH_DIR "${FOAM_PROJECT_DIR}/thirdparty/ptscotch")
if(EXISTS "${PTSCOTCH_DIR}/include/ptscotch.h" AND EXISTS "${PTSCOTCH_DIR}/lib/ptscotch.lib")
    set(PTSCOTCH_INC_DIR "${PTSCOTCH_DIR}/include")
    set(PTSCOTCH_LIB_DIR "${PTSCOTCH_DIR}/lib")
    if(NOT TARGET ptscotch)
        add_library(ptscotch UNKNOWN IMPORTED GLOBAL)
        set_target_properties(ptscotch PROPERTIES
            IMPORTED_LOCATION "${PTSCOTCH_LIB_DIR}/ptscotch.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${PTSCOTCH_INC_DIR}")
        # Error-handling archives; MPI binding is added after MPI detection
        set_target_properties(ptscotch PROPERTIES INTERFACE_LINK_LIBRARIES
            "${PTSCOTCH_LIB_DIR}/ptscotcherr.lib;${PTSCOTCH_LIB_DIR}/ptscotcherrexit.lib;scotch")
    endif()
    message(STATUS "Using vendored PTSCOTCH: ${PTSCOTCH_DIR}")
endif()

#------------------------------------------------------------------------------
# Third-party: CGAL (+ GMP/MPFR) - optional, header-only
#------------------------------------------------------------------------------
set(GMP_INC_DIR "")
set(GMP_LIB_DIR "")
set(GMP_DIR "${FOAM_PROJECT_DIR}/thirdparty/gmp")
if(EXISTS "${GMP_DIR}/include/gmp.h" AND EXISTS "${GMP_DIR}/lib/gmp.lib")
    set(GMP_INC_DIR "${GMP_DIR}/include")
    set(GMP_LIB_DIR "${GMP_DIR}/lib")
    if(NOT TARGET gmp)
        add_library(gmp SHARED IMPORTED GLOBAL)
        set_target_properties(gmp PROPERTIES
            IMPORTED_LOCATION "${GMP_DIR}/bin/gmp.dll"
            IMPORTED_IMPLIB "${GMP_DIR}/lib/gmp.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${GMP_INC_DIR}")
    endif()
endif()

set(MPFR_INC_DIR "")
set(MPFR_LIB_DIR "")
set(MPFR_DIR "${FOAM_PROJECT_DIR}/thirdparty/mpfr")
if(EXISTS "${MPFR_DIR}/include/mpfr.h" AND EXISTS "${MPFR_DIR}/lib/mpfr.lib")
    set(MPFR_INC_DIR "${MPFR_DIR}/include")
    set(MPFR_LIB_DIR "${MPFR_DIR}/lib")
    if(NOT TARGET mpfr)
        add_library(mpfr SHARED IMPORTED GLOBAL)
        set_target_properties(mpfr PROPERTIES
            IMPORTED_LOCATION "${MPFR_DIR}/bin/libmpfr-6.dll"
            IMPORTED_IMPLIB "${MPFR_DIR}/lib/mpfr.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${MPFR_INC_DIR}")
    endif()
endif()

set(CGAL_INC_DIR "")
set(CGAL_LIB_DIR "")
set(CGAL_DIR "${FOAM_PROJECT_DIR}/thirdparty/cgal")
if(EXISTS "${CGAL_DIR}/include/CGAL/version.h")
    set(CGAL_INC_DIR "${CGAL_DIR}/include")
    set(CGAL_LIB_DIR "${CGAL_DIR}/lib")
    message(STATUS "Using vendored CGAL: ${CGAL_DIR}")
endif()

#------------------------------------------------------------------------------
# Third-party: KaHIP (graph partitioning) - optional
#------------------------------------------------------------------------------
set(KAHIP_INC_DIR "")
set(KAHIP_LIB_DIR "")
set(KAHIP_DIR "${FOAM_PROJECT_DIR}/thirdparty/kahip")
if(EXISTS "${KAHIP_DIR}/include/kaHIP_interface.h" AND EXISTS "${KAHIP_DIR}/lib/kahip.lib")
    set(KAHIP_INC_DIR "${KAHIP_DIR}/include")
    set(KAHIP_LIB_DIR "${KAHIP_DIR}/lib")
    if(NOT TARGET kahip)
        add_library(kahip UNKNOWN IMPORTED GLOBAL)
        set_target_properties(kahip PROPERTIES
            IMPORTED_LOCATION "${KAHIP_LIB_DIR}/kahip.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${KAHIP_INC_DIR}")
    endif()
    message(STATUS "Using vendored KaHIP: ${KAHIP_DIR}")
endif()

#------------------------------------------------------------------------------
# Third-party: MGridGen (GAMG agglomeration) - optional, built in-tree
#------------------------------------------------------------------------------
# Ancient C code (Karypis/Moulitsas, metis4-era). Sources are vendored under
# thirdparty/mgridgen/src so the CRT/toolset always matches the main build;
# only drand48/srand48 needed a shim (src/IMlib/msvc_drand48.c).
set(MGRIDGEN_INC_DIR "")
set(MGRIDGEN_LIB_DIR "")
set(MGRIDGEN_DIR "${FOAM_PROJECT_DIR}/thirdparty/mgridgen")
if(EXISTS "${MGRIDGEN_DIR}/src/Lib/mgridgen.c" AND EXISTS "${MGRIDGEN_DIR}/include/mgridgen.h")
    set(MGRIDGEN_INC_DIR "${MGRIDGEN_DIR}/include")
    if(NOT TARGET mgrid)
        file(GLOB MGRIDGEN_SOURCES
            "${MGRIDGEN_DIR}/src/Lib/*.c"
            "${MGRIDGEN_DIR}/src/IMlib/*.c")
        add_library(mgrid STATIC ${MGRIDGEN_SOURCES})
        target_include_directories(mgrid
            PUBLIC "${MGRIDGEN_INC_DIR}"
            PRIVATE "${MGRIDGEN_DIR}/src/Lib" "${MGRIDGEN_DIR}/src/IMlib")
        set_target_properties(mgrid PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${FOAM_LIBBIN_DIR}")
    endif()
    message(STATUS "Using vendored MGridGen sources: ${MGRIDGEN_DIR}")
endif()

#------------------------------------------------------------------------------
# Third-party: MPI (MS-MPI) - optional
#------------------------------------------------------------------------------
set(FOAM_HAVE_MPI FALSE)
if(NOT FOAM_MPI STREQUAL "dummy")
    # Prefer vendored MS-MPI (thirdparty/msmpi: include/, lib/msmpi.lib)
    set(_msmpi_dir "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/msmpi")
    if(EXISTS "${_msmpi_dir}/include/mpi.h" AND EXISTS "${_msmpi_dir}/lib/msmpi.lib")
        if(NOT TARGET MPI::MPI_CXX)
            add_library(MPI::MPI_CXX UNKNOWN IMPORTED GLOBAL)
            set_target_properties(MPI::MPI_CXX PROPERTIES
                IMPORTED_LOCATION "${_msmpi_dir}/lib/msmpi.lib"
                INTERFACE_INCLUDE_DIRECTORIES "${_msmpi_dir}/include")
        endif()
        set(MPI_CXX_FOUND TRUE)
        message(STATUS "Using vendored MS-MPI: ${_msmpi_dir}")
    else()
        find_package(MPI QUIET COMPONENTS CXX)
    endif()
    if(MPI_CXX_FOUND)
        set(FOAM_HAVE_MPI TRUE)
        set(FOAM_MPI_NAME "msmpi")
        set(FOAM_MPI_SUBDIR "msmpi")
        # Deploy runtime binaries (mpiexec/smpd/msmpi.dll) next to the exes
        if(EXISTS "${_msmpi_dir}/bin/msmpi.dll")
            file(COPY "${_msmpi_dir}/bin/"
                 DESTINATION "${FOAM_APPBIN_DIR}/Release"
                 FILES_MATCHING PATTERN "*.dll" PATTERN "*.exe")
        endif()
        # ptscotch calls MPI_* - bind it now that MPI::MPI_CXX exists
        if(TARGET ptscotch)
            set_property(TARGET ptscotch APPEND PROPERTY
                INTERFACE_LINK_LIBRARIES "MPI::MPI_CXX")
        endif()
    else()
        message(WARNING "MPI requested but not found - falling back to dummy")
        set(FOAM_MPI "dummy")
        set(FOAM_MPI_NAME "dummy")
        set(FOAM_MPI_SUBDIR "dummy")
    endif()
endif()

#------------------------------------------------------------------------------
# Compile definitions for libz/mpi presence (mirrors Make/options logic)
#------------------------------------------------------------------------------
if(HAVE_LIBZ_DEFINE)
    add_compile_definitions(HAVE_LIBZ)
endif()

#------------------------------------------------------------------------------
# Sources
#------------------------------------------------------------------------------
add_subdirectory(src)

if(FOAM_BUILD_APPS)
    add_subdirectory(applications)
endif()

#------------------------------------------------------------------------------
# Summary
#------------------------------------------------------------------------------
message(STATUS "")
message(STATUS "OpenFOAM ${FOAM_VERSION_STR} (api ${FOAM_VERSION_API}) - Windows/MSVC")
message(STATUS "  MPI        : ${FOAM_MPI}")
message(STATUS "  Precision  : ${FOAM_PRECISION}  Label size: ${FOAM_LABEL_SIZE}")
message(STATUS "  zlib       : ${ZLIB_FOUND}")
message(STATUS "  Output     : ${FOAM_LIBBIN_DIR}")
message(STATUS "")
