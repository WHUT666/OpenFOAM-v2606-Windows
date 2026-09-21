#------------------------------------------------------------------------------
# package-windows.ps1 - assemble the distributable OpenFOAM Windows package
#
# Usage:
#   powershell -File etc/package-windows.ps1 [-BuildDir build-shared] [-OutDir dist]
#
# Layout produced under <OutDir>/<Name>:
#   bin/         executables + staged runtime DLLs
#   lib/         OpenFOAM DLLs
#   imp/         import libs (dev) + static third-party libs (ccmio, mgrid)
#   include/     lnInclude tree, one flat dir per library
#   etc/         controlDict & friends
#   thirdparty/  runtime-only subset (fftw, zlib, gmp, mpfr, msmpi)
#   tutorials/
#   setenv.bat   package-local environment (WM_PROJECT_DIR/PATH/FOAM_ETC)
#   *.md         README / LICENSE
#
# Then <OutDir>/<Name>.7z is created with 7-Zip if available.
#------------------------------------------------------------------------------

[CmdletBinding()]
param(
    [string]$BuildDir = "build-shared",
    [string]$OutDir   = "dist",
    [string]$Name     = "OpenFOAM-v2606-Windows-x64",
    [switch]$NoArchive
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
Push-Location $root
try {

$pkg    = Join-Path $OutDir $Name
$binSrc = Join-Path $BuildDir "bin\Release"
$libSrc = Join-Path $BuildDir "lib\Release"
$impSrc = Join-Path $BuildDir "imp\Release"
$incSrc = Join-Path $BuildDir "lnInclude"

foreach ($d in @($binSrc, $libSrc)) {
    if (-not (Test-Path $d)) { throw "missing build output: $d" }
}

$exeCount = (Get-ChildItem "$binSrc\*.exe" -ErrorAction SilentlyContinue).Count
$dllCount = (Get-ChildItem "$libSrc\*.dll" -ErrorAction SilentlyContinue).Count
if ($exeCount -lt 50 -or $dllCount -lt 50) {
    throw "suspicious build output: $exeCount exes / $dllCount dlls - aborting"
}
Write-Host "==> packaging $exeCount exes, $dllCount dlls"

if (Test-Path $pkg) { Remove-Item $pkg -Recurse -Force }
New-Item -ItemType Directory -Force -Path $pkg | Out-Null

# --- payloads ----------------------------------------------------------------
Copy-Item $binSrc  (Join-Path $pkg "bin") -Recurse
Copy-Item $libSrc  (Join-Path $pkg "lib") -Recurse
if (Test-Path $impSrc) { Copy-Item $impSrc (Join-Path $pkg "imp") -Recurse }
if (Test-Path $incSrc) { Copy-Item $incSrc (Join-Path $pkg "include") -Recurse }
Copy-Item "etc"        (Join-Path $pkg "etc") -Recurse
Copy-Item "tutorials"  (Join-Path $pkg "tutorials") -Recurse

foreach ($doc in @("README.md", "LICENSE.md")) {
    if (Test-Path $doc) { Copy-Item $doc (Join-Path $pkg $doc) }
}

# static third-party libs needed to re-link OpenFOAM apps -> imp/
foreach ($s in @("ccmio.lib", "mgrid.lib")) {
    $f = Join-Path $libSrc $s
    if (Test-Path $f) { Copy-Item $f (Join-Path $pkg "imp") -Force }
}

# --- third-party runtime subset (DLLs/exes needed on PATH) -------------------
$tp = Join-Path $pkg "thirdparty"
foreach ($sub in @(
        @{ src = "thirdparty\fftw";     dst = "fftw";   pat = "*.dll" },
        @{ src = "thirdparty\zlib\bin"; dst = "zlib\bin"; pat = "*.dll" })) {
    $dstDir = Join-Path $tp $sub.dst
    New-Item -ItemType Directory -Force -Path $dstDir | Out-Null
    Copy-Item (Join-Path $sub.src $sub.pat) $dstDir -ErrorAction SilentlyContinue
}
foreach ($dir in @("gmp", "mpfr", "msmpi")) {
    Copy-Item "thirdparty\$dir" (Join-Path $tp $dir) -Recurse
}

# --- package-local environment script -----------------------------------------
@'
@echo off
rem OpenFOAM-v2606 Windows - package environment
set "WM_PROJECT_DIR=%~dp0"
if "%WM_PROJECT_DIR:~-1%"=="\" set "WM_PROJECT_DIR=%WM_PROJECT_DIR:~0,-1%"
set "FOAM_ETC=%WM_PROJECT_DIR%\etc"
set "PATH=%WM_PROJECT_DIR%\bin;%WM_PROJECT_DIR%\lib;%WM_PROJECT_DIR%\thirdparty\fftw;%WM_PROJECT_DIR%\thirdparty\zlib\bin;%WM_PROJECT_DIR%\thirdparty\gmp\bin;%WM_PROJECT_DIR%\thirdparty\mpfr\bin;%WM_PROJECT_DIR%\thirdparty\msmpi\bin;%PATH%"
'@ | Set-Content -Encoding ASCII (Join-Path $pkg "setenv.bat")

@'
OpenFOAM v2606 - Windows x64 (MSVC, shared DLLs, MS-MPI)
========================================================

Run "setenv.bat" (or call it from your shell), then use the solvers
directly, e.g.:

    setenv.bat
    blockMesh -case tutorials\incompressible\icoFoam\cavity\cavity
    icoFoam   -case tutorials\incompressible\icoFoam\cavity\cavity
    mpiexec -n 4 icoFoam -parallel -case <case>

Contents: bin (executables), lib (DLLs), imp (import libs),
          include (headers, one dir per library), etc, thirdparty
          runtime DLLs, tutorials.
'@ | Set-Content -Encoding ASCII (Join-Path $pkg "PACKAGE-INFO.txt")

# --- archive ------------------------------------------------------------------
if (-not $NoArchive) {
    $sevenZip = @("C:\Program Files\7-Zip\7z.exe",
                  (Get-Command 7z -ErrorAction SilentlyContinue).Source) |
                Where-Object { $_ -and (Test-Path $_) } | Select-Object -First 1
    $archive = "$OutDir\$Name.7z"
    if ($sevenZip) {
        if (Test-Path $archive) { Remove-Item $archive }
        & $sevenZip a -t7z -mx=5 "$archive" "$pkg" | Out-Null
    } else {
        $archive = "$OutDir\$Name.zip"
        if (Test-Path $archive) { Remove-Item $archive }
        Compress-Archive -Path $pkg -DestinationPath $archive -CompressionLevel Optimal
    }
    Write-Host "==> archive: $archive"
}

Write-Host "==> package: $pkg"
}
finally { Pop-Location }
