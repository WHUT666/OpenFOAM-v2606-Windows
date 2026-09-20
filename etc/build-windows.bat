@echo off
rem OpenFOAM-v2606 Windows build helper
rem
rem   Usage: build-windows.bat [target ...] [/c]
rem
rem     no args        configure (if needed) + build ALL_BUILD Release
rem     target names   build only those targets (e.g. icoFoam decomposePar)
rem     /c             force a fresh cmake configure first
rem
rem Serializes MSBuild: kills stale build nodes that otherwise lock
rem .obj/.tlog files, then runs a single build invocation.
rem -m:2 is the safe parallelism (32 GB RAM): bare -m spawns too many
rem concurrent cl.exe (project-level x /MP) -> C1060 heap exhaustion.

setlocal EnableDelayedExpansion
set "PROJ=%~dp0.."
for %%i in ("%PROJ%") do set "PROJ=%%~fi"
set "BLD=%PROJ%\build"

rem --- locate MSBuild -------------------------------------------------------
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo [build-windows] vswhere not found - is Visual Studio installed? 1>&2
    exit /b 1
)
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe"`) do set "MSBUILD=%%i"
if not defined MSBUILD (
    echo [build-windows] MSBuild.exe not found 1>&2
    exit /b 1
)

rem --- kill stale build nodes (file-lock prevention) ------------------------
taskkill /F /IM MSBuild.exe /T >nul 2>&1
taskkill /F /IM cl.exe /T >nul 2>&1
taskkill /F /IM link.exe /T >nul 2>&1
taskkill /F /IM mspdbsrv.exe /T >nul 2>&1

rem --- configure ------------------------------------------------------------
if /i "%~1"=="/c" (
    set "FORCECFG=1"
    shift
)
if not exist "%BLD%\OpenFOAM.sln" set "FORCECFG=1"
if defined FORCECFG (
    if not exist "%BLD%" mkdir "%BLD%"
    pushd "%BLD%"
    cmake .. -A x64 -DFOAM_MPI=msmpi || (popd & exit /b 1)
    popd
)

rem --- build ----------------------------------------------------------------
if "%~1"=="" (
    "%MSBUILD%" "%BLD%\OpenFOAM.sln" -t:ALL_BUILD -p:Configuration=Release -m:2 -clp:Summary
) else (
    set "TGTS="
    :tgtloop
    if not "%~1"=="" (
        set "TGTS=!TGTS!-t:%~1 "
        shift
        goto tgtloop
    )
    "%MSBUILD%" "%BLD%\OpenFOAM.sln" !TGTS! -p:Configuration=Release -m:2 -clp:Summary
)
endlocal
