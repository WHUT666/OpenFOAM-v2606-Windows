@echo off
rem OpenFOAM-v2606 Windows (MSVC) environment
rem Usage: call <repo>\etc\openfoam-env.bat

set "WM_PROJECT_DIR=%~dp0.."
for %%i in ("%WM_PROJECT_DIR%") do set "WM_PROJECT_DIR=%%~fi"
set "PATH=%WM_PROJECT_DIR%\build\bin\Release;%WM_PROJECT_DIR%\thirdparty\fftw;%WM_PROJECT_DIR%\thirdparty\gmp\bin;%WM_PROJECT_DIR%\thirdparty\mpfr\bin;%WM_PROJECT_DIR%\thirdparty\msmpi\bin;%WM_PROJECT_DIR%\thirdparty\zlib\bin;%PATH%"
