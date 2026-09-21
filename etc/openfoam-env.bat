@echo off
rem OpenFOAM-v2606 Windows (MSVC) environment
rem Usage: call <repo>\etc\openfoam-env.bat [build-dir-name]
rem   default build dir: build  (static, FOAM_STATIC_LIBS=ON)
rem   shared DLL build : call openfoam-env.bat build-shared

set "WM_PROJECT_DIR=%~dp0.."
for %%i in ("%WM_PROJECT_DIR%") do set "WM_PROJECT_DIR=%%~fi"

if "%~1"=="" (set "FOAM_BUILD_DIR=build") else (set "FOAM_BUILD_DIR=%~1")

set "PATH=%WM_PROJECT_DIR%\%FOAM_BUILD_DIR%\bin\Release;%WM_PROJECT_DIR%\%FOAM_BUILD_DIR%\lib\Release;%WM_PROJECT_DIR%\thirdparty\fftw;%WM_PROJECT_DIR%\thirdparty\gmp\bin;%WM_PROJECT_DIR%\thirdparty\mpfr\bin;%WM_PROJECT_DIR%\thirdparty\msmpi\bin;%WM_PROJECT_DIR%\thirdparty\zlib\bin;%PATH%"
