# OpenFOAM-v2606-Windows

**English** | [中文](#中文)

---

## English

An unofficial **native Windows port of OpenFOAM v2606** — built with
Visual Studio 2022 (MSVC) through a CMake-generated solution, with real
MS-MPI parallelism and vendored third-party libraries.
**No WSL, no MinGW cross-compilation, no Cygwin.**

### Why this port exists

Upstream OpenFOAM offers two ways to run on Windows, and both have real
drawbacks:

| Official approach | Limitation |
|---|---|
| WSL / WSL2 | Runs Linux binaries inside a VM — no Visual Studio debugging, cross-filesystem I/O overhead, cut off from the Windows-native toolchain (MS-MPI, HPC tools, MSVC diagnostics) |
| MinGW cross-compiled binaries | Built on Linux — you cannot edit the source and rebuild on Windows; poor debugging experience |

**Windows users have never had an OpenFOAM they can open in Visual
Studio, edit, set breakpoints in, and rebuild.** This project fills
that gap.

### Goals

- **Native development**: CMake generates `.sln`/`.vcxproj` — open,
  build and debug any solver or library directly in VS2022
- **Real MPI**: vendored MS-MPI SDK;
  `mpiexec -n N <solver> -parallel` works end-to-end (not a dummy MPI)
- **Full feature parity**: solvers, mesh tools
  (blockMesh / snappyHexMesh / foamyHexMesh), parallel
  decomposition/reconstruction (decomposePar / redistributePar /
  reconstructPar), the CGAL geometry kernel and CCM format conversion
  are all verified running — not just "it compiles"
- **All dependencies vendored**: METIS / SCOTCH / PT-SCOTCH / KaHIP /
  CGAL / GMP / MPFR / FFTW / zlib / win_flex / lemon / m4 / MS-MPI —
  clone and build, no separate dependency hunt
- **Two linking models**: static (`/WHOLEARCHIVE`, preserves
  runTimeSelection self-registration) and shared DLL (per-library
  `<Lib>_API` annotations), from the same sources
- **Distributable**: CI produces an unzip-and-run
  `OpenFOAM-v2606-Windows-x64.7z`

### Non-goals

- Not a replacement for upstream Linux releases — this is a downstream
  port for Windows workflows, tracking v2606
- POSIX shell utilities (`foamJob`, `runParallel`, `foamLog`, ...) are
  not ported; use the batch/PowerShell equivalents
- Modules needing ADIOS2 / PETSc / ParaView / Python stay off until
  those dependencies are vendored

### Status at a glance

- **596 app targets + 135 libraries** build with zero compile/link
  errors
- `blockMesh → decomposePar → mpiexec -n N icoFoam -parallel →
  reconstructPar` verified end-to-end (9-rank MPI)
- `snappyHexMesh` motorBike (3.8 M cells, serial + MPI), `simpleFoam`
  pitzDaily, `pimpleFoam` movingCone, `interFoam` damBreak and more
  tutorials verified
- All four decomposition backends (metis / scotch / ptscotch / kahip)
  are real implementations
- **17 plugin DLLs + 19 plugin executables**: cfmesh meshers,
  avalanche finite-area solvers, research apps, all 15
  turbulence-community model libraries
- **253 `Test-*` apps pass** (2 known-difference, 1 MS-MPI hang,
  57 auto-detected skips); one-command regression via
  `etc/run-test-apps.ps1`, wired into CI
- Crash stack traces resolve to file:line via DbgHelp + PDB
  (`FOAM_ABORT=1` forces a trace on FatalError); exes link 8 MB stacks
- GitHub Actions builds the Windows package on `v*` tags, runs the
  regression suite and smoke-tests it (blockMesh / icoFoam /
  checkMesh / foamToCcm)

### Wiki pages

- [[Build|Build]] — requirements, CMake options, static vs shared,
  packaging
- [[Usage|Usage]] — environment setup, running solvers, MPI,
  troubleshooting
- [[Internals|Internals]] — DLL symbol export, MSVC compatibility
  fixes, lnInclude case-sensitivity handling
- [[Testing|Testing]] — test-suite runner, verified workflows

### Community

Questions, use reports and port discussion are welcome:

- QQ group: **581148231**
- QQ: **2358314123**

### Relationship to upstream

Based on [OpenFOAM v2606](https://gitlab.com/openfoam/core/openfoam)
(released by OpenCFD / Keysight Technologies), licensed GPL v3+.
Port changes are confined to:

- A new `cmake/` build system (wmake2cmake converter + MSVC compat
  layer) — the upstream wmake system is kept intact
- `src/OSspecific/MSwindows/` platform layer (incl. DbgHelp crash
  stack traces)
- `<Lib>_API` annotations for cross-DLL data symbols — the macros
  expand to nothing in static mode, zero impact on upstream
- Vendored Windows dependencies under `thirdparty/`

No upstream semantics are modified — every change aims at compiling
the same code under MSVC and producing the same runtime behaviour as
the GCC build.

---

## 中文

[English](#english) | **中文**

OpenFOAM v2606 的**非官方 Windows 原生移植** —— 用 Visual Studio 2022
(MSVC) 通过 CMake 生成的解决方案编译,真实 MS-MPI 并行,第三方依赖全部
内置。**不用 WSL,不用 MinGW 交叉编译,不用 Cygwin。**

### 为什么要做这个移植

OpenFOAM 官方对 Windows 的支持有两条路,但都有明显短板:

| 官方方案 | 问题 |
|---|---|
| WSL / WSL2 | 本质是跑 Linux 二进制;Windows 侧无法直接用 Visual Studio 调试,I/O 跨文件系统有性能损耗,与 Windows 原生工具链(MS-MPI、HPC 工具、VS 诊断)绝缘 |
| MinGW 交叉编译的预编译包 | 只能在 Linux 上交叉编译产出,不能在 Windows 上改源码重新构建;调试体验差 |

**Windows 用户一直没有一个"能在 Visual Studio 里打开、改代码、
打断点、重新编译"的 OpenFOAM。** 本项目补上这个缺口。

### 项目目标

- **原生开发体验**:CMake 生成 `.sln`/`.vcxproj`,在 VS2022 里直接
  打开、编译、调试任意求解器或库
- **真实并行能力**:vendored MS-MPI SDK,
  `mpiexec -n N <solver> -parallel` 端到端可用,而非 dummy MPI
- **完整功能对等**:求解器、网格工具(blockMesh / snappyHexMesh /
  foamyHexMesh)、并行分解与重构(decomposePar / redistributePar /
  reconstructPar)、CGAL 几何内核、CCM 格式转换等全部实际跑通,
  而非仅"能编译"
- **第三方依赖全内置**:METIS / SCOTCH / PT-SCOTCH / KaHIP / CGAL /
  GMP / MPFR / FFTW / zlib / win_flex / lemon / m4 / MS-MPI 全部
  vendored,clone 即可构建,不需要额外装依赖
- **两种链接模型**:静态(`/WHOLEARCHIVE`,保留 runTimeSelection
  自注册)与共享 DLL(逐库 `<Lib>_API` 标注)两种构建模式共用同一套
  源码
- **可分发**:CI 自动打包出解压即用的
  `OpenFOAM-v2606-Windows-x64.7z`

### 非目标

- 不替代上游 Linux 版本 —— 这是一个面向 Windows 工作流的下游移植,
  持续跟踪上游 v2606
- 不移植 POSIX shell 工具脚本(`foamJob`、`runParallel`、`foamLog`
  等);Windows 下用批处理 / PowerShell 等价物
- 暂不接需要 ADIOS2 / PETSc / ParaView / Python 的模块,对应依赖
  vendored 之前保持关闭

### 当前状态速览

- **596 个应用目标 + 135 个库**全部构建通过,零编译/链接错误
- `blockMesh → decomposePar → mpiexec -n N icoFoam -parallel →
  reconstructPar` 完整闭环(9 进程 MPI 验证)
- `snappyHexMesh` motorBike(380 万单元,串行 + MPI)、`simpleFoam`
  pitzDaily、`pimpleFoam` movingCone、`interFoam` damBreak 等教程
  案例实跑验证
- 四种分解后端(metis / scotch / ptscotch / kahip)均为真实实现
- **17 个插件 DLL + 19 个插件可执行文件**:cfmesh 网格器、
  avalanche 有限面积求解器、research 应用、全部 15 个
  turbulence-community 湍流模型库
- **253 个 `Test-*` 测试通过**(2 项已知平台差异、1 项 MS-MPI 挂起、
  57 项自动识别跳过),`etc/run-test-apps.ps1` 一键回归且已接入 CI
- 崩溃栈经 DbgHelp + PDB 解析到文件:行号(`FOAM_ABORT=1` 可让
  FatalError 强制打印);可执行文件统一 8 MB 栈
- GitHub Actions 推送 `v*` tag 即自动跑回归套件、产出 Windows
  安装包并做冒烟测试

### Wiki 页面导航

- [[构建指南|Build]] — 环境要求、CMake 配置、静态/共享两种构建、打包
- [[使用说明|Usage]] — 环境变量、运行求解器、MPI 并行、常见问题
- [[移植技术内幕|Internals]] — DLL 符号导出、MSVC 兼容性修复、
  lnInclude 大小写处理等
- [[测试与验证|Testing]] — 测试套件运行方式、已验证工作流清单

### 交流沟通

欢迎大家交流使用与移植问题:

- QQ 群:**581148231**
- QQ:**2358314123**

### 与上游的关系

本项目基于 [OpenFOAM v2606](https://gitlab.com/openfoam/core/openfoam)
(OpenCFD / Keysight Technologies 发布),遵循 GPL v3+ 许可。移植改动
集中在:

- 新增 `cmake/` 构建系统(wmake2cmake 转换器 + MSVC 兼容层),
  **不删除上游 wmake 体系**
- `src/OSspecific/MSwindows/` 平台层补全(含 DbgHelp 崩溃栈打印)
- 为跨 DLL 数据符号补 `<Lib>_API` 标注;静态模式下这些宏展开为空,
  对上游零影响
- `thirdparty/` 下 vendored Windows 依赖

上游源码语义不做功能性修改 —— 所有改动目标都是"让同一份代码在
MSVC 下正确编译并产生与 GCC 构建一致的运行时行为"。
