#------------------------------------------------------------------------------
# seed-cyclic-stubs.ps1 - seed import libs for cyclic shared-lib groups
#
# OpenQBMM's phase-system libs are mutually referential (ELF allows cyclic
# DT_NEEDED; MSVC does not). The hub->spoke edges are emitted as raw
# import-lib paths (FOAM_LINK_AS_FILE_<hub> in modules/CMakeLists.txt), so
# the hub's link needs the spoke import libs BEFORE the spoke DLLs exist.
#
# For each cyclic spoke this script:
#   1. compiles the spoke's objects        (MSBuild -t:ClCompile)
#   2. scans them into an exports .def      (cmake/genExportsDef.py)
#   3. creates a stub import lib            (lib /DEF)
# The real .lib overwrites the stub when the spoke links for real.
#
# Run once after configuring a FRESH shared build tree, before building
# the OpenQBMM multiphase / velocityDistributionTransport groups:
#
#   powershell -File etc\seed-cyclic-stubs.ps1 -BuildDir build-shared
#
#------------------------------------------------------------------------------

param(
    [string]$BuildDir = "build-shared",
    [string]$Config   = "Release",
    [string[]]$Spokes = @(
        "pbeInterfacialModels",
        "pbeTurbulenceModels",
        "pdEulerianInterfacialModels",
        "pdTurbulenceModels"
    )
)

$ErrorActionPreference = "Stop"
$root   = Split-Path $PSScriptRoot
$bld    = Join-Path $root $BuildDir
$impdir = Join-Path $bld "imp\$Config"

# Locate MSBuild + lib.exe + dumpbin via vswhere
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vs = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
$msbuild  = Join-Path $vs "MSBuild\Current\Bin\MSBuild.exe"
$vcbin    = Get-ChildItem (Join-Path $vs "VC\Tools\MSVC\*\bin\Hostx64\x64") | Select-Object -Last 1
$lib      = Join-Path $vcbin "lib.exe"
$dumpbin  = Join-Path $vcbin "dumpbin.exe"
$python   = (Get-Command python).Source

foreach($s in $Spokes){
    $implib = Join-Path $impdir "$s.lib"
    $proj   = Join-Path $bld "modules\$s.vcxproj"
    $objdir = Join-Path $bld "modules\$s.dir\$Config"
    $def    = Join-Path $objdir "foam_exports.def"

    if(Test-Path $implib){
        Write-Host "[$s] import lib exists - skip"
        continue
    }
    if(-not (Test-Path $proj)){
        Write-Host "[$s] no vcxproj (target not configured) - skip"
        continue
    }

    Write-Host "[$s] compiling objects for stub..."
    & $msbuild $proj -t:ClCompile -p:Configuration=$Config -p:Platform=x64 `
        -m:2 -v:m -nr:false | Select-Object -Last 3

    Write-Host "[$s] scanning exports..."
    & $python (Join-Path $root "cmake\genExportsDef.py") `
        --project $proj --config $Config --objdir $objdir `
        --dumpbin $dumpbin --out $def

    # Import lib must bind to the real DLL name (lib<s>.dll), not the
    # .def basename - write a wrapper def with an explicit LIBRARY line.
    $stubdef = Join-Path $objdir "stub_exports.def"
    "LIBRARY lib$s`nEXPORTS" | Out-File -Encoding ascii $stubdef
    Get-Content $def | Select-Object -Skip 1 | Add-Content $stubdef

    Write-Host "[$s] creating stub import lib -> $implib"
    & $lib /DEF:$stubdef /OUT:$implib /MACHINE:X64 | Select-Object -Last 2
}

Write-Host "Done. Cyclic hubs can now link; real libs replace stubs as spokes build."
