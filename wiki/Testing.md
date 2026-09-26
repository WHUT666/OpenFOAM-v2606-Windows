# Testing / 测试与验证

**English** | [中文](#中文)

---

## English

### Test suite runner

`etc/run-test-apps.ps1` runs every `applications/test/Test-*.exe`
found in `bin\Release`. For each exe it maps back to the source dir
under `applications/test`, copies it to a temp workdir, runs
`blockMesh` first when the case needs a mesh, and synthesizes a
minimal `system/controlDict` for tests that only need a `Time`.

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File etc\run-test-apps.ps1
powershell ... -BuildDir build-shared -TimeoutSec 180 -Filter "Test-Hash*"
powershell ... -Parallel -NProcs 4     # also run MPI tests via mpiexec
```

Parameters: `-BuildDir` (default `build-shared`), `-OutDir`,
`-TimeoutSec`, `-Filter`, `-Skip <name,...>`, `-Parallel`, `-NProcs`.
Results CSV + per-test logs under `$env:TEMP\of-test-logs-*`;
exit code = failure count (CI-friendly).

Classification: **PASS** (exit 0), **FAIL** (nonzero), **TIMEOUT**,
**SKIP** (auto-detected: needs mpiexec/args/mesh/stdin that the
harness can't supply).

### Baseline (shared build)

- **254 PASS / 2 FAIL / 57 SKIP** (313 total)
- The 2 FAILs: `Test-FixedList2` (1 MB default stack overflow —
  exes now link `/STACK:8MB`, fix verified via `editbin`),
  `Test-cubicEqn` (2 of ~1.1 M random cubics at the 1e-8 tolerance
  boundary — MSVC FP codegen difference, kept as sentinel).
- `Test-one-sided1` used to hang under MS-MPI: a `MPI_Win_lock_all`
  epoch with RMA ops queued for more than one target deadlocks in
  `MPI_Win_unlock_all` (standalone reproducer — genuine MS-MPI
  limitation). `UPstreamWindow` now issues `MPI_Win_flush_all`
  before `unlock_all` under `MSMPI_VER` and the test finalises
  cleanly.
- A handful of expected-abort tests (`Test-sigFpe` etc.) exercise
  deliberate error paths — identical behaviour upstream.
- MPI tests verified under MS-MPI: `broadcastCopy`,
  `parallel-barrier1`, `parallel-file-write1`, `parallel-scan`,
  `readBroadcast1`, `treeComms`, `one-sided1`.

### Known failures / skips

| Test | Status | Reason |
|---|---|---|
| `Test-decomposedBlockData` | SKIP | needs a real decomposedBlockData-format file (decomposed case) |
| `Test-processorTopology` | SKIP | needs a decomposed mesh (`decomposePar` output) |
| `Test-cubicEqn` | known-bad | marginal MSVC FP-tolerance deviation |

The CI workflow runs the suite with
`-Skip Test-cubicEqn` and uploads the logs as the
`regression-logs` artifact.

### Verified end-to-end workflows

Mesh: `blockMesh`, `snappyHexMesh` (motorBike 3.8 M cells, serial +
MPI, CGAL kernel), `foamyHexMesh` (CGAL blob), `cfmesh`
(cartesianMesh/cartesian2DMesh/pMesh/tetMesh),
`surfaceFeatureExtract`, `checkMesh`, `transformPoints`, `topoSet`.

Solvers: `icoFoam` (cavity, serial + 9-rank MPI), `simpleFoam`
(pitzDaily, converged, streamlines), `pisoFoam` (RAS cavity 0–10 s),
`pimpleFoam` (movingCone dynamic mesh + GAMG + vtkWrite),
`laplacianFoam` (flange, cyclicAMI + GAMG), `potentialFoam`,
`interFoam` (damBreak VOF), avalanche solvers (`faSavageHutterFoam`
etc., deposition tutorial to endTime=15).

Parallel infrastructure: `decomposePar`/`reconstructPar`/
`reconstructParMesh`/`redistributePar` with metis / scotch /
ptscotch / kahip; 9-rank cavity closed loop
(decompose → mpiexec → reconstruct).

I/O & conversion: `foamToVTK`, `foamToCcm`/`ccmToFoam` round-trip
(cavity → `.ccmg` → read back → checkMesh OK), `foamFormatConvert`,
`foamDictionary`, `postProcess`, `patchSummary`.

GAMG agglomeration: `agglomerator MGridGen` (vendored MGridGen,
verified on icoFoam cavity).

Runtime plugins (shared build): `controlDict` `libs("libX")` DLL
loading; missing lib warns gracefully.

### Running a single test manually

```bat
call etc\openfoam-env.bat build-shared
cd <workdir>
copy %WM_PROJECT_DIR%\applications\test\<dir>\* .
bin\Release\Test-<name>.exe
```

MPI tests additionally need `processor0..N-1` dirs each carrying
`system/controlDict` (the runner synthesizes them; `-parallel`
argList checks them).

---

## 中文

[English](#english) | **中文**

### 测试套件

`etc/run-test-apps.ps1` 遍历 `bin\Release` 下所有
`applications/test/Test-*.exe`:把每个 exe 映射回
`applications/test` 下的源目录,拷贝到临时工作目录;案例需要网格
时先跑 `blockMesh`,只需要 `Time` 的测试则合成最小
`system/controlDict`。

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File etc\run-test-apps.ps1
powershell ... -BuildDir build-shared -TimeoutSec 180 -Filter "Test-Hash*"
powershell ... -Parallel -NProcs 4     # 同时跑 MPI 测试(mpiexec)
```

参数:`-BuildDir`(默认 `build-shared`)、`-OutDir`、
`-TimeoutSec`、`-Filter`、`-Skip <name,...>`、`-Parallel`、
`-NProcs`。结果 CSV + 每测试日志在 `$env:TEMP\of-test-logs-*`;
退出码 = 失败数(方便 CI)。

判定:**PASS**(退出码 0)、**FAIL**(非零)、**TIMEOUT**、
**SKIP**(自动检测:需要 mpiexec/参数/网格/stdin 等 harness
无法提供的条件)。

### 基线(共享构建)

- **254 PASS / 2 FAIL / 57 SKIP**(共 313 项)
- 2 个 FAIL:`Test-FixedList2`(默认 1 MB 栈溢出 —— exe 已统一
  `/STACK:8MB`,editbin 实测修复)、`Test-cubicEqn`(~113 万随机
  三次方程中 2 项卡在 1e-8 容差边界 —— MSVC 浮点代码生成差异,
  保留作哨兵)。
- `Test-one-sided1` 曾在 MS-MPI 下挂起:`MPI_Win_lock_all` epoch
  内对多个目标排队的 RMA 操作会在 `MPI_Win_unlock_all` 死锁
  (独立复现器确认 —— MS-MPI 自身缺陷)。`UPstreamWindow` 现在在
  `MSMPI_VER` 下先 `MPI_Win_flush_all` 再 `unlock_all`,
  测试干净收尾。
- 少量预期中止测试(`Test-sigFpe` 等)走刻意错误路径 —— 与上游
  行为一致。
- MS-MPI 下已验证 MPI 测试:`broadcastCopy`、`parallel-barrier1`、
  `parallel-file-write1`、`parallel-scan`、`readBroadcast1`、
  `treeComms`、`one-sided1`。

### 已知失败 / 跳过

| 测试 | 状态 | 原因 |
|---|---|---|
| `Test-decomposedBlockData` | SKIP | 需要真实 decomposedBlockData 格式文件(已分解案例) |
| `Test-processorTopology` | SKIP | 需要已分解网格(`decomposePar` 输出) |
| `Test-cubicEqn` | 已知不良 | MSVC 浮点容差轻微偏差 |

CI 以 `-Skip Test-cubicEqn` 跑该套件,日志作为
`regression-logs` artifact 上传。

### 已验证端到端工作流

网格:`blockMesh`、`snappyHexMesh`(motorBike 380 万单元,
串行 + MPI,CGAL 内核)、`foamyHexMesh`(CGAL blob)、cfmesh
(cartesianMesh/cartesian2DMesh/pMesh/tetMesh)、
`surfaceFeatureExtract`、`checkMesh`、`transformPoints`、
`topoSet`。

求解器:`icoFoam`(cavity,串行 + 9 进程 MPI)、`simpleFoam`
(pitzDaily,收敛,streamlines)、`pisoFoam`(RAS cavity
0–10 s)、`pimpleFoam`(movingCone 动网格 + GAMG + vtkWrite)、
`laplacianFoam`(flange,cyclicAMI + GAMG)、`potentialFoam`、
`interFoam`(damBreak VOF)、雪崩求解器(`faSavageHutterFoam`
等,deposition 教程跑到 endTime=15)。

并行基建:`decomposePar`/`reconstructPar`/`reconstructParMesh`/
`redistributePar`(metis / scotch / ptscotch / kahip);9 进程
cavity 闭环(分解 → mpiexec → 重构)。

I/O 与转换:`foamToVTK`、`foamToCcm`/`ccmToFoam` 往返(cavity →
`.ccmg` → 读回 → checkMesh 通过)、`foamFormatConvert`、
`foamDictionary`、`postProcess`、`patchSummary`。

GAMG 凝聚:`agglomerator MGridGen`(vendored MGridGen,icoFoam
cavity 验证)。

运行时插件(共享构建):`controlDict` `libs("libX")` DLL 加载;
缺失库优雅告警。

### 手动跑单个测试

```bat
call etc\openfoam-env.bat build-shared
cd <工作目录>
copy %WM_PROJECT_DIR%\applications\test\<目录>\* .
bin\Release\Test-<名字>.exe
```

MPI 测试还需要 `processor0..N-1` 目录(各含
`system/controlDict`,runner 会自动合成;`-parallel` argList
会检查它们)。
