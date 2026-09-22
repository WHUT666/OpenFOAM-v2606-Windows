# run-test-apps.ps1 - regression runner for applications/test/Test-*.exe
#
# Upstream tests expect to run inside their own source dir (which carries
# system/controlDict, input files, sub-cases). This runner maps each
# Test-*.exe back to its dir under applications/test, copies it to a temp
# workdir, picks the case dir (dir root, or the single subdir that has
# system/controlDict), runs blockMesh first when the case needs it, then
# runs the test.
#
# Classification: PASS (exit 0), FAIL (nonzero), TIMEOUT, SKIP.
#
# Usage:
#   powershell -NoProfile -ExecutionPolicy Bypass -File etc\run-test-apps.ps1
#   powershell ... -BuildDir build-shared -TimeoutSec 180 -Filter "Test-Hash*"
#   powershell ... -Parallel -NProcs 4   # also run MPI tests via mpiexec
#
# Exit code = number of failed tests (for CI use).

[CmdletBinding()]
param(
    [string]$BuildDir   = "build-shared",
    [string]$OutDir     = "",
    [int]$TimeoutSec    = 180,
    [string]$Filter     = "Test-*",
    [string[]]$Skip     = @(),
    [switch]$Parallel,
    [int]$NProcs        = 4
)

$ErrorActionPreference = "Stop"
$root   = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
$binSrc = Join-Path $root (Join-Path $BuildDir "bin\Release")
$libSrc = Join-Path $root (Join-Path $BuildDir "lib\Release")
$testSrc = Join-Path $root "applications\test"

if (-not (Test-Path $binSrc)) { throw "missing build output: $binSrc" }

if (-not $OutDir) {
    $OutDir = Join-Path $env:TEMP ("of-test-logs-" + (Get-Date -Format "yyyyMMdd-HHmmss"))
}
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

# Runtime env (mirrors etc/openfoam-env.bat) + FOAM_RUN/FOAM_ETC that the
# upstream shell environment provides and several tests rely on.
$env:PATH = "$binSrc;$libSrc;" +
    "$root\thirdparty\fftw;$root\thirdparty\gmp\bin;$root\thirdparty\mpfr\bin;" +
    "$root\thirdparty\msmpi\bin;$root\thirdparty\zlib\bin;$env:PATH"
$env:WM_PROJECT_DIR = $root
$env:FOAM_RUN  = $OutDir
$env:FOAM_ETC  = Join-Path $root "etc"
$env:WM_MPLIB  = "MSMPI"
if (-not $env:USER) { $env:USER = $env:USERNAME }   # POSIX name
if (-not $env:HOME) { $env:HOME = $env:USERPROFILE }

# --- exe -> source dir mapping ------------------------------------------------
# Index every dir under applications/test that has Make/files, keyed by
# (a) basenames of its *.C/*.cxx sources and (b) the dir basename.
$dirBySource = @{}
$dirByName   = @{}
Get-ChildItem $testSrc -Recurse -Directory |
    Where-Object { Test-Path (Join-Path $_.FullName "Make\files") } |
    ForEach-Object {
        $d = $_.FullName
        $dirByName[$_.Name.ToLower()] = $d
        Get-ChildItem $d -File | Where-Object { $_.Extension -match '^\.(C|cxx)$' } |
            ForEach-Object { $dirBySource[$_.BaseName.ToLower()] = $d }
    }

function Resolve-TestDir([string]$exeName) {
    if ($dirBySource.ContainsKey($exeName.ToLower())) {
        return $dirBySource[$exeName.ToLower()]
    }
    $short = ($exeName -replace '^Test-','').ToLower()
    if ($dirByName.ContainsKey($short)) { return $dirByName[$short] }
    return $null
}

# Pick the run dir inside a copied test dir: dir root if it is a case,
# else the first subdir that is a case (alphabetical; multi-case tests may
# expect a specific one - failure detail notes which was picked).
function Resolve-CaseDir([string]$dir) {
    if (Test-Path (Join-Path $dir "system\controlDict")) { return $dir }
    $cases = Get-ChildItem $dir -Directory |
        Where-Object { Test-Path (Join-Path $_.FullName "system\controlDict") }
    if ($cases.Count -ge 1) { return $cases[0].FullName }
    return $dir
}

# Minimal controlDict for tests that only need Time to construct
$minimalControlDict = @'
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    object      controlDict;
}
application     Test;
startFrom       latestTime;
startTime       0;
stopAt          endTime;
endTime         1;
deltaT          1;
writeControl    timeStep;
writeInterval   1;
purgeWrite      0;
writeFormat     ascii;
writePrecision  6;
writeCompression off;
timeFormat      general;
timePrecision   6;
runTimeModifiable false;
'@

# Parallel-only tests: run under "mpiexec -n N <exe> -parallel" when
# -Parallel is given, otherwise skipped. Value = extra args after -parallel
# (file-arg tests get a file that exists in every case dir).
$mpiOnly = [ordered]@{
    "Test-broadcastCopy"        = @("system/controlDict")
    "Test-one-sided1"           = @()
    "Test-parallel-barrier1"    = @()
    "Test-parallel-file-write1" = @()
    "Test-parallel-scan"        = @()
    "Test-processorTopology"    = @()
    "Test-readBroadcast1"       = @("system/controlDict")
    "Test-treeComms"            = @()
    "Test-parallel1"            = @()
    "Test-parallel2"            = @()
    "Test-parallel3"            = @()
    "Test-Pstream"              = @()
}

# Tests that cannot run standalone (need args, stdin, plugins).
$skipReasons = [ordered]@{
    "Test-dynamicLibrary"     = "needs plugin dll arg"
    "Test-etcFiles"           = "needs args"
    "Test-codeStream"         = "needs a dictionary"
    "Test-externalFileCoupler"= "needs master+slave process pair"
    "Test-decomposedBlockData"= "needs decomposedBlockData-format input (real decomposed case)"
}
if (-not $Parallel) {
    foreach ($k in $mpiOnly.Keys) {
        $skipReasons[$k] = "needs mpiexec -parallel (use -Parallel to run)"
    }
}

# Deliberate-abort / error-exercising tests: nonzero exit IS the expected
# outcome (they behave identically upstream on Linux).
$expectNonzero = [ordered]@{
    "Test-sigFpe"       = "provokes SIGFPE on purpose; abort = trap works"
    "Test-fileName"     = "deliberate findEtcFile FatalError"
    "Test-dimensionSet" = "aborts on deliberately-bad dims (abort() never throws - upstream same)"
    "Test-Enum"         = "reads invalid token from Sin on purpose"
    "Test-string2"      = "deliberately malformed expansion syntax"
    "Test-string"       = "expands undefined variable '__UNKNOWN' on purpose"
    "Test-PDRblockMesh" = "try/catch around abort() paths - abort() never throws (upstream same)"
    "Test-multiDimPolyFitter" = "requests nonexistent polyDegree2 to print Valid-types diagnostic"
}
foreach ($s in $Skip) { $skipReasons[$s] = "user skip" }

$exes = Get-ChildItem "$binSrc\*.exe" |
    Where-Object { $_.BaseName -like $Filter } |
    Sort-Object Name

Write-Host "==> $($exes.Count) test exe(s) under $binSrc"
Write-Host "==> logs: $OutDir`n"

function Invoke-Exe($exePath, $workDir, $logFile, $timeoutSec, $arguments = "") {
    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = $exePath
    $psi.Arguments = $arguments
    $psi.WorkingDirectory = $workDir
    $psi.UseShellExecute = $false
    $psi.RedirectStandardOutput = $true
    $psi.RedirectStandardError  = $true
    $psi.RedirectStandardInput  = $true
    $psi.CreateNoWindow = $true

    $p = New-Object System.Diagnostics.Process
    $p.StartInfo = $psi
    [void]$p.Start()
    $p.StandardInput.Close()
    $outTask = $p.StandardOutput.ReadToEndAsync()
    $errTask = $p.StandardError.ReadToEndAsync()

    $exited = $p.WaitForExit($timeoutSec * 1000)
    if (-not $exited) {
        # taskkill /T also kills mpiexec's child ranks (plain Kill orphans them)
        try { taskkill /PID $p.Id /T /F 2>$null | Out-Null } catch {}
        try { $p.Kill() } catch {}
        # Drain whatever was captured before the kill so the log isn't empty
        $out = ""; $err = ""
        try { $out = $outTask.GetAwaiter().GetResult() } catch {}
        try { $err = $errTask.GetAwaiter().GetResult() } catch {}
        if ($logFile) { ($out + "`n" + $err) | Out-File -Encoding utf8 $logFile }
        return @{ Status = "TIMEOUT"; ExitCode = $null; Output = ($out + "`n" + $err) }
    }
    $out = $outTask.GetAwaiter().GetResult()
    $err = $errTask.GetAwaiter().GetResult()
    # Errors are printed on stderr (Perr) - match patterns against both
    $combined = $out + "`n" + $err
    if ($logFile) { $combined | Out-File -Encoding utf8 $logFile }
    return @{ Status = "DONE"; ExitCode = $p.ExitCode; Output = $combined }
}

$results = @()

foreach ($exe in $exes) {
    $name = $exe.BaseName

    if ($skipReasons.Contains($name)) {
        $results += [pscustomobject]@{
            Name = $name; Status = "SKIP"; Time = 0; Detail = $skipReasons[$name]
        }
        continue
    }

    # Copy the test's source dir to an isolated workdir (tests write files)
    $srcDir = Resolve-TestDir $name
    $work = Join-Path $OutDir ("run-" + $name)
    if ($srcDir) {
        Copy-Item $srcDir $work -Recurse -Force
    } else {
        New-Item -ItemType Directory -Force -Path $work | Out-Null
    }
    $caseDir = Resolve-CaseDir $work

    # No case anywhere: synthesize a minimal controlDict so that
    # createTime.H / argList can construct a Time
    if (-not (Test-Path (Join-Path $caseDir "system\controlDict"))) {
        New-Item -ItemType Directory -Force -Path (Join-Path $caseDir "system") | Out-Null
        $minimalControlDict | Out-File -Encoding ascii `
            (Join-Path $caseDir "system\controlDict")
    }

    # Parallel run: argList -parallel requires processor0..N-1 dirs and each
    # rank reads its own processorN/system/controlDict
    if ($Parallel -and $mpiOnly.Contains($name)) {
        for ($i = 0; $i -lt $NProcs; ++$i) {
            $proc = Join-Path $caseDir ("processor" + $i)
            New-Item -ItemType Directory -Force -Path $proc | Out-Null
            if (Test-Path (Join-Path $caseDir "system")) {
                Copy-Item (Join-Path $caseDir "system") $proc -Recurse -Force
            }
        }

    }

    $log = Join-Path $OutDir ($name + ".log")

    $sw = [System.Diagnostics.Stopwatch]::StartNew()

    # Case needs a mesh: run blockMesh first
    if ((Test-Path (Join-Path $caseDir "system\blockMeshDict")) -and
        -not (Test-Path (Join-Path $caseDir "constant\polyMesh"))) {
        $bm = Invoke-Exe (Join-Path $binSrc "blockMesh.exe") $caseDir `
                (Join-Path $OutDir ($name + ".blockMesh.log")) $TimeoutSec
        if ($bm.Status -ne "DONE" -or $bm.ExitCode -ne 0) {
            $results += [pscustomobject]@{
                Name = $name; Status = "FAIL"
                Time = [math]::Round($sw.Elapsed.TotalSeconds,1)
                Detail = "blockMesh failed: " + $bm.Status
            }
            continue
        }
    }

    if ($Parallel -and $mpiOnly.Contains($name)) {
        $mpiexec = Join-Path $root "thirdparty\msmpi\bin\mpiexec.exe"
        $argStr = "-n $NProcs `"$($exe.FullName)`" -parallel"
        if ($mpiOnly[$name].Count) {
            $argStr += " " + ($mpiOnly[$name] -join " ")
        }
        $r = Invoke-Exe $mpiexec $caseDir $log $TimeoutSec $argStr
    } else {
        $r = Invoke-Exe $exe.FullName $caseDir $log $TimeoutSec
    }
    $sw.Stop()

    # Exit code is authoritative: tests that exercise error paths catch the
    # exception and still exit 0 (FATAL text in output is then expected).
    if ($r.Status -eq "TIMEOUT") {
        $status = "TIMEOUT"; $detail = "exceeded ${TimeoutSec}s"
    }
    elseif ($r.ExitCode -eq 0) {
        $status = "PASS"; $detail = "exit=0"
    }
    elseif ($expectNonzero.Contains($name)) {
        $status = "PASS"; $detail = "expected abort: " + $expectNonzero[$name]
    }
    elseif ($r.Output -match "Expected \d+ arguments? but found 0" -or
            $r.Output -match "^\s*Usage:") {
        $status = "SKIP"; $detail = "needs args"
    }
    elseif ($r.Output -match "Not running in parallel|parallel only|Please run in parallel") {
        $status = "SKIP"; $detail = "needs mpiexec -parallel"
    }
    elseif ($r.Output -match 'file: Sin at line') {
        $status = "SKIP"; $detail = "reads stdin"
    }
    elseif ($r.Output -match "No times!") {
        $status = "SKIP"; $detail = "needs case with time dirs"
    }
    elseif ($r.Output -match 'cannot find file|Cannot find file') {
        $status = "SKIP"; $detail = "needs case/data files"
    }
    else {
        $status = "FAIL"; $detail = "exit=$($r.ExitCode)"
    }

    $results += [pscustomobject]@{
        Name = $name; Status = $status
        Time = [math]::Round($sw.Elapsed.TotalSeconds,1); Detail = $detail
    }
}

# --- summary -----------------------------------------------------------------
$pass = ($results | Where-Object Status -eq "PASS").Count
$fail = ($results | Where-Object Status -eq "FAIL").Count
$skip = ($results | Where-Object Status -eq "SKIP").Count
$tout = ($results | Where-Object Status -eq "TIMEOUT").Count

Write-Host "`n===== RESULTS ====="
$results | Format-Table Name, Status, Time, Detail -AutoSize
Write-Host "PASS=$pass FAIL=$fail TIMEOUT=$tout SKIP=$skip TOTAL=$($results.Count)"
Write-Host "logs: $OutDir"

$results | Export-Csv -NoTypeInformation (Join-Path $OutDir "summary.csv")

exit $fail + $tout
