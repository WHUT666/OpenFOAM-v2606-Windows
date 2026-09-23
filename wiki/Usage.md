# Usage / 使用说明

**English** | [中文](#中文)

---

## English

### Environment

From a source build:

```bat
call etc\openfoam-env.bat              rem static tree (build\)
call etc\openfoam-env.bat build-shared rem shared DLL tree
```

From a release package: `call setenv.bat` in the package root.

Or set PATH manually:
`<bld>\bin\Release;<bld>\lib\Release;<repo>\thirdparty\fftw` (+ `gmp\bin`,
`mpfr\bin`, `msmpi\bin`, `zlib\bin` — `openfoam-env.bat` covers all).
`WM_PROJECT_DIR` is optional — a compile-time fallback is baked into
libOpenFOAM, so `#includeEtc`/`etc/caseDicts` resolve anyway.

### Hello world: cavity

```bat
mkdir run && cd run
xcopy /e /i %WM_PROJECT_DIR%\tutorials\incompressible\icoFoam\cavity\cavity cavity
blockMesh -case cavity
icoFoam -case cavity
checkMesh -case cavity
```

### MPI parallel

```bat
decomposePar -case cavity
mpiexec -n 4 icoFoam -case cavity -parallel
reconstructPar -case cavity
```

`mpiexec`/`msmpi.dll` come from the vendored SDK
(`thirdparty/msmpi/bin`, already on PATH via `openfoam-env.bat`).
Decomposition backends `metis`, `scotch`, `ptscotch`, `kahip` are all
real implementations — pick one in `system/decomposeParDict`.

### Runtime plugins (shared build)

```cpp
// system/controlDict
libs ("libutilityFunctionObjects");
```

`dlOpen` loads the DLL by name; a missing library warns gracefully
without aborting. In static builds `dlOpen` resolves against the
process image — the plugin must be in the executable's link closure.

### Useful tools

`checkMesh`, `topoSet`, `setFields`, `transformPoints`,
`patchSummary`, `foamDictionary`, `foamFormatConvert`, `foamToVTK`,
`foamToCcm`/`ccmToFoam`, `postProcess`, `surfaceFeatureExtract`,
`surfaceBooleanFeatures` (CGAL).

### Known limitations

- POSIX shell utilities (`foamJob`, `runParallel`, `foamLog`,
  `foamMonitor`, ...) don't run under cmd — use the app directly or
  the PowerShell/batch equivalents.
- `foamyHexMeshSurfaceSimplify` is not built (needs external
  `fastdualoctree_sgp` + OpenGL; upstream skips it the same way).
- Debug configuration is unverified; use Release (with PDBs — stack
  traces still resolve).
- MS-MPI is MPI-2: `MPI_Neighbor_alltoall` is emulated internally via
  pairwise `MPI_Sendrecv`. One-sided RMA (`MPI_Win`/
  `MPI_Fetch_and_op`) passive-target ops can hang — see [[Testing]].
- Windows is case-insensitive: case dictionaries relying on
  same-name-different-case files can't work.

### Troubleshooting

| Symptom | Fix |
|---|---|
| `error C1083 ... Permission denied` during build | stale MSBuild/cl nodes hold file locks — kill `MSBuild.exe`/`cl.exe`/`link.exe`/`mspdbsrv.exe` (or just use `etc\build-windows.bat`) |
| `C1060` compiler heap exhausted | lower parallelism to `-m:2` |
| exe can't find DLLs at runtime | `call etc\openfoam-env.bat <build-dir>` first, or add `lib\Release` to PATH |
| LNK2019 after editing a `src/` header | re-run CMake configure (lnInclude refresh), then Rebuild the affected target |
| `libs ("libX")` warns "cannot load" | expected if libX isn't in `lib\Release` — it's a warning, not a crash |

---

## 中文

[English](#english) | **中文**

### 环境变量

源码构建:

```bat
call etc\openfoam-env.bat              rem 静态构建目录(build\)
call etc\openfoam-env.bat build-shared rem 共享 DLL 构建目录
```

发布包:在包根目录 `call setenv.bat`。

或者手动设 PATH:
`<bld>\bin\Release;<bld>\lib\Release;<repo>\thirdparty\fftw`
(还有 `gmp\bin`、`mpfr\bin`、`msmpi\bin`、`zlib\bin` ——
`openfoam-env.bat` 一次全带上)。`WM_PROJECT_DIR` 可选 ——
libOpenFOAM 里烘焙了编译期回退路径,`#includeEtc`/`etc/caseDicts`
不设也能解析。

### 入门案例:cavity

```bat
mkdir run && cd run
xcopy /e /i %WM_PROJECT_DIR%\tutorials\incompressible\icoFoam\cavity\cavity cavity
blockMesh -case cavity
icoFoam -case cavity
checkMesh -case cavity
```

### MPI 并行

```bat
decomposePar -case cavity
mpiexec -n 4 icoFoam -case cavity -parallel
reconstructPar -case cavity
```

`mpiexec`/`msmpi.dll` 来自 vendored SDK(`thirdparty/msmpi/bin`,
`openfoam-env.bat` 已加进 PATH)。分解后端 `metis`、`scotch`、
`ptscotch`、`kahip` 均为真实实现 —— 在 `system/decomposeParDict`
里选一个即可。

### 运行时插件(共享构建)

```cpp
// system/controlDict
libs ("libutilityFunctionObjects");
```

`dlOpen` 按名字加载 DLL;库不存在时优雅告警、不崩溃。静态构建下
`dlOpen` 回退到进程镜像 —— 插件必须在可执行文件的链接闭包里。

### 常用工具

`checkMesh`、`topoSet`、`setFields`、`transformPoints`、
`patchSummary`、`foamDictionary`、`foamFormatConvert`、
`foamToVTK`、`foamToCcm`/`ccmToFoam`、`postProcess`、
`surfaceFeatureExtract`、`surfaceBooleanFeatures`(CGAL)。

### 已知限制

- POSIX shell 工具(`foamJob`、`runParallel`、`foamLog`、
  `foamMonitor` 等)在 cmd 下不可用 —— 直接调用应用或用
  PowerShell/批处理等价物。
- `foamyHexMeshSurfaceSimplify` 未构建(依赖外部
  `fastdualoctree_sgp` + OpenGL,上游同样跳过)。
- Debug 配置未验证;请用 Release(带 PDB —— 崩溃栈依然能解析到
  文件:行号)。
- MS-MPI 是 MPI-2:`MPI_Neighbor_alltoall` 内部用成对
  `MPI_Sendrecv` 模拟;单边 RMA(`MPI_Win`/`MPI_Fetch_and_op`)
  passive-target 可能挂起 —— 见 [[Testing]]。
- Windows 文件系统大小写不敏感:依赖同名不同大小写文件的案例
  字典无法工作。

### 故障排查

| 症状 | 处理 |
|---|---|
| 构建报 `C1083 ... Permission denied` | 残留 MSBuild/cl 进程持文件锁 —— 杀 `MSBuild.exe`/`cl.exe`/`link.exe`/`mspdbsrv.exe`(或直接用 `etc\build-windows.bat`) |
| `C1060` 编译器堆耗尽 | 并行度降到 `-m:2` |
| 运行时找不到 DLL | 先 `call etc\openfoam-env.bat <构建目录>`,或把 `lib\Release` 加进 PATH |
| 改了 `src/` 头文件后 LNK2019 | 重跑 CMake 配置(刷新 lnInclude),再 Rebuild 受影响目标 |
| `libs ("libX")` 警告无法加载 | libX 不在 `lib\Release` 时的正常告警,不会崩溃 |
