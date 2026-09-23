# OpenFOAM-v2606 Windows/MSVC 原生移植

> **English**: unofficial **native Windows port of OpenFOAM v2606** — built
> with Visual Studio 2022 (MSVC) via CMake-generated solutions, real MS-MPI
> parallelism and vendored third-party libraries. No WSL, no MinGW
> cross-compilation, no Cygwin.

## 项目目的

让 OpenFOAM v2606 能够在 Windows 上**原生开发和运行**:

- 使用 Visual Studio 2022 + CMake 生成 `.sln`/`.vcxproj` 直接编译调试
- 不依赖 WSL / MinGW 交叉编译 / Cygwin
- 真实 MS-MPI 并行(非 dummy MPI)
- 第三方依赖全部提供 Windows 版本(METIS / SCOTCH / PT-SCOTCH / CGAL /
  GMP / MPFR / Boost / FFTW / zlib / win_flex / lemon / m4)
- 保证功能正常:求解器、网格工具、并行分解/重构、CGAL 网格生成均实际跑通

## 当前进度

### 已完成

- ✅ **共享库(DLL)构建**:`FOAM_STATIC_LIBS=OFF` 时每个 OpenFOAM 库
  独立产出 DLL(134 个库),逐库 `<Lib>_API` 导入/导出标注
  (`cmake/compat/foamApi.h`),运行时选择表经
  `TableInsert/TableSet/TableErase` 成员函数封装跨 DLL 访问,
  模板静态成员用 `Foam_<Class>_defines_typeName` opt-out 保证单一属主;
  `cmake/genExportsDef.py` 从 obj 收割强符号生成 `.def` 导出表
- ✅ **全部应用默认可构建**:596 个应用目标全部启用
  (`FOAM_APP_<路径>` 默认 ON,可用 `-DFOAM_APP_<路径>=OFF` 关闭单个),
  共享构建产出 **596 个 exe + 135 个 DLL,零编译/链接错误**;
  应用级本地库(湍流模型、相系统、DSMC、conformalVoronoiMesh 等
  28 个)同样以 DLL 构建并被依赖应用正确链接
- ✅ **CCM 转换**:vendored `libccmio` 2.6.1(foam-extend 公开源码包,
  纯 C)+ ADF 底层以 MSVC 静态编译为 `ccmio.lib`,链入 `libccm.dll`;
  `foamToCcm`/`ccmToFoam` 双向转换经 cavity 案例验证
  (写出 .ccmg → 读回 polyMesh → checkMesh OK)
- ✅ **静态库构建保持可用**:`FOAM_STATIC_LIBS=ON`(默认)产出
  `/WHOLEARCHIVE` 静态库,保留 runTimeSelection 自注册语义,
  两种模式共用同一套源码与标注
- ✅ **构建系统**:完整 CMake 移植(`cmake/wmake2cmake.py` 从 `Make/files`
  + `Make/options` 自动生成目标,自动识别 MPI 需求、解析 wmake
  `mpi-rules`/`PFLAGS`/`PINC`/`PLIBS`),VS2022 x64 / C++17 / DP /
  label32,全量构建零编译错误
- ✅ **MPI**:vendored MS-MPI SDK(`thirdparty/msmpi`,含 mpiexec/smpd/
  msmpi.dll),`mpiexec -n N <solver> -parallel` 端到端可用;修复了
  MS-MPI 缺少 `MPI_Comm_create_group` 导致的子通信域集合调用错配
- ✅ **并行分解**:`decomposePar`/`redistributePar` 的 `metis`、`scotch`、
  `ptscotch`、`kahip` 后端均为真实实现(vendored 库)
- ✅ **CGAL**:vendored CGAL 6.x + GMP/MPFR;`foamyHexMesh`、`foamyQuadMesh`、
  `cellSizeAndAlignmentGrid`、`viewFactorsGen`、`surfaceBooleanFeatures`
  均可构建运行
- ✅ **运行时插件**:共享构建下 `controlDict` 的 `libs(...)` 动态加载
  DLL 插件可用
- ✅ **兼容性修复**:空 compound 类型名污染 token 表导致的并行反序列化
  bug、`argv[0]` 反斜杠/`.exe` 后缀、conda-forge metis 的 pre-UCRT stdio
  shim(`__iob_func`)、MSVC 名称限定与模板显式实例化等
- ✅ **社区插件/模块接入**:`plugins/` + `modules/` 走同一条
  wmake2cmake 管线,已产出 **17 个插件 DLL + 19 个插件 exe**:
  cfmesh(meshLibrary + cartesianMesh/cartesian2DMesh/pMesh/tetMesh/
  zipUpMesh)、avalanche(faAvalanche + 3 个有限面积求解器 + 3 个工具)、
  research(porousPimpleFoam/aniPorousPimpleFoam/projectionFoam/
  preciseFoam1-5)、turbulence-community 全部 15 个湍流模型库
  (SAH/PDA/CND/GammaSST/SpalartAllmarasRC/EllipticBlending/
  MachineLearning/dynamicSmagorinsky/WallModelledLES/HelicalForce)。
  需 ADIOS2/PETSc/ParaView/VTK/Python 的模块及 OpenQBMM 暂保持关闭
- ✅ **回归测试套件**:`etc/run-test-apps.ps1` 一键跑全部
  `Test-*.exe`(自动映射源目录、合成 controlDict、`-Parallel` 走
  mpiexec);基线 **253 PASS / 2 FAIL / 1 TIMEOUT / 57 SKIP**
  (SKIP 为需真实算例/参数,MPI 通信原语 6 项实测通过)
- ✅ **崩溃堆栈诊断**:`src/OSspecific/MSwindows/printStack` 经
  DbgHelp + `CaptureStackBackTrace` 输出 demangle 后的函数名 +
  源文件:行号(配 `FOAM_ENABLE_PDB` 的 `/Zi`/`/DEBUG`);
  `FOAM_ABORT=1` 下 FatalError 实测打出完整调用链;
  可执行文件统一 `/STACK:8MB` 对齐 Linux 默认栈

### 已验证工作流(共享 DLL 构建)

- `blockMesh → decomposePar → mpiexec -n N icoFoam -parallel →
  reconstructPar` 完整闭环(9 进程 MPI 验证通过)
- `snappyHexMesh` 串行与 MPI 并行(CGAL 几何内核),`simpleFoam`
  (湍流+functionObject+streamlines 收敛)、`pimpleFoam`
  (movingCone 动网格+GAMG+vtkWrite 全程)
- `interFoam` damBreak(VoF+湍流+相系统 DLL 全链路)、
  `surfaceFeatureExtract + snappyHexMesh` flange 案例
  (19186 单元,零网格质量错误)
- `redistributePar`(metis / scotch / ptscotch / kahip)及
  `redistributePar -reconstruct`;`setFields`、`checkMesh`、
  `foamDictionary`、`reconstructParMesh`
- `controlDict` `libs ("libutilityFunctionObjects");` 动态插件加载;
  不存在的库优雅告警不崩溃
- GitHub Actions 自动打包(`.github/workflows/windows-package.yml`):
  推送 `v*` tag 或手动触发即构建共享 DLL 全套,
  **构建后自动跑回归套件**(`Test-*`,跳过 2 个已知平台差异项)并上传
  日志 artifact,产出 `OpenFOAM-v2606-Windows-x64.7z`(bin+lib+imp+
  include+etc+tutorials+第三方运行时,默认不含 Test-*/PDB),
  并在包内冒烟验证 blockMesh/icoFoam/checkMesh/foamToCcm 后上传
  artifact/Release
- avalanche deposition 教程端到端跑通(t=0→15 s 瞬态,releaseArea
  映射 → faSavageHutterFoam → 正常收敛退出)
- cfmesh `cartesianMesh` 真实网格生成教程验证通过
- `foamToCcm -mesh`(cavity → `meshExport-*.ccmg`)→ `ccmToFoam`
  读回 → `checkMesh` 网格 OK(882 点/400 单元,patch 名称保留)
- 测试应用:`Test-dummyLib`(WM_* 编译期值正确)、`Test-volField`、
  `Test-dimField1`、`Test-fvc2D`、`Test-parallel-comm1`、
  `Test-processorTopology`(9 进程,MPI-2 Sendrecv 邻居交换回退)
- 静态回归:`FOAM_STATIC_LIBS=ON` 下 OpenFOAM/finiteVolume/icoFoam
  编译、链接、运行一致通过

### 已知限制

- `foamyHexMeshSurfaceSimplify` 跳过(缺 `fastdualoctree_sgp` + OpenGL,
  上游同样跳过)
- POSIX shell 工具脚本(`foamJob`、`runParallel`、`foamLog` 等)在 cmd
  下不可用
- Debug 配置未验证;MSBuild 增量构建对生成头(`lnInclude`)变更的
  依赖追踪不完整,头文件改动后建议对受影响目标用 Rebuild

## 交流沟通

欢迎大家交流使用与移植问题:

- QQ 群:**581148231**
- QQ:**2358314123**

## 快速开始(Windows)

```bat
git clone https://github.com/WHUT666/OpenFOAM-v2606-Windows.git
cd OpenFOAM-v2606-Windows

rem --- 静态构建(默认,FOAM_STATIC_LIBS=ON)---
etc\build-windows.bat
call etc\openfoam-env.bat

rem --- 共享 DLL 构建 ---
cmake -B build-shared -A x64 -DFOAM_MPI=msmpi -DFOAM_STATIC_LIBS=OFF
etc\build-windows.bat /d:build-shared
call etc\openfoam-env.bat build-shared

blockMesh -help
mpiexec -n 2 icoFoam -case <case> -parallel
```

应用默认全部构建(596 个目标);关闭单个应用:
`cmake -B <build> -DFOAM_APP_<路径>=OFF`;关闭全部 test 应用:
`-DFOAM_APP_TESTS=OFF`。MSBuild 全量构建建议
`/m:2` 并行度(更高并行可能触发 C1060 编译器内存耗尽)。

本地打包(与 CI 相同逻辑):`powershell -File etc/package-windows.ps1`
→ `dist/OpenFOAM-v2606-Windows-x64(.7z)`,解压后 `setenv.bat` 即用。

详细构建约定、坑位记录与调试须知见 [AGENTS.md](AGENTS.md)。

---
以下是上游 OpenFOAM 原始 README:

<table align="center"><tr><td align="center" width="9999">

<a href="https://www.openfoam.com/">
    <img src="https://www.openfoam.com/themes/bs4esi/img/openfoam-logo.png?v20210416" alt="OpenFOAM logo" title="OpenFOAM" align="center" height="60" />
</a>

<h4 align="center">Welcome to the Official OpenFOAM&reg; Repository!</a></h4>

<h4 align="center">The Industry-Leading Open-Source Fluid Simulation Software</a></h4>

<p align="center">
  <a href="#installation">Installation</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#license">License</a> •
  <a href="#trademark">Trademark</a> •
  <a href="#useful-links">Useful Links</a>
</p>
</td></tr></table>

[OpenFOAM&reg;](https://www.openfoam.com/) is the industry-leading,
free, (forever) open-source, general-purpose computational fluid dynamics (CFD) software,
[developed, maintained and released by Keysight Technologies](http://www.openfoam.com/history/).

OpenFOAM&reg; has [an extensive range of features](http://www.openfoam.com/documentation) to solve anything from complex fluid flows involving chemical reactions, turbulence and heat transfer, to acoustics, solid mechanics and electromagnetics. For this reason, OpenFOAM&reg; developed a large user base across most areas of engineering and science, from both commercial and academic organisations.

OpenFOAM&reg; is professionally released every six months to include
customer sponsored developments and contributions from the community -
individual and group contributors, integrations, e.g. from extend-project and
OpenFOAM Foundation Ltd., as well as
[OpenFOAM&reg; Governance guided activities](https://www.openfoam.com/governance/).

## Installation

You can build OpenFOAM&reg; in different ways for Windows, macOS, Linux and Unix-like operating systems. Please follow the links below that suits your needs:

<details open>
  <summary><strong>Pre-compiled operating-system packages</strong></summary>

- **Best for**: Users who want a stable, easy-to-update installation. This is generally the most straightforward way to get a functional environment on these systems.
- **Method**: This involves downloading and installing the pre-compiled operating-system packages using the native package manager of the operating system.
- **Instructions**:
  - [Debian/Ubuntu](https://gitlab.com/openfoam/core/openfoam/-/wikis/precompiled/debian)
  - [openSUSE](https://gitlab.com/openfoam/core/openfoam/-/wikis/precompiled/suse)
  - [Rocky/Fedora/CentOS/RedHat](https://gitlab.com/openfoam/core/openfoam/-/wikis/precompiled/redhat)
  - Windows
    - [Windows Subsystem for Linux (WSL/WSL2)](https://gitlab.com/openfoam/core/openfoam/-/wikis/precompiled/windows#windows-subsystem-for-linux)<sup>[What is WSL?](https://learn.microsoft.com/en-us/windows/wsl/about)</sup>
    - [Native Windows executables with cross-compilation](https://gitlab.com/openfoam/core/openfoam/-/wikis/precompiled/windows#native-windows)
</details>

<details>
  <summary><strong>Containers (Docker/Singularity)</strong></summary>

- **Best for**: Those who need a self-contained or specific version of OpenFOAM&reg; without modifying their host system.
- **Method**: This involves using pre-assembled Docker<sup>[What is Docker?](https://docs.docker.com/get-started/)</sup> images for Windows/macOS/Linux or Apptainer<sup>[What is Apptainer?](https://apptainer.org/docs/user/latest/introduction.html)</sup> images to run OpenFOAM&reg; within a virtualized, isolated container.
- **Instructions**:
  - [Docker](https://gitlab.com/openfoam/core/openfoam/-/wikis/precompiled/docker)
  - [Apptainer](https://gitlab.com/openfoam/core/openfoam/-/wikis/precompiled/apptainer)

</details>

<details>
  <summary><strong>Source-code compilation</strong></summary>

- **Best for**: Advanced users/developers that requires the latest features, experimental branches, or custom builds with specific optimizations or libraries.
- **Method**: Download the source code and compile it yourself. This is typically done on Linux or within a WSL environment, and it requires all necessary dependencies to be manually installed.
- **Instructions**:
  - [Guide for building from the source](https://gitlab.com/openfoam/core/openfoam/-/wikis/building)
  - See the following table for the specifics:

| Location    | README    | Requirements | Build |
|-------------|-----------|--------------|-------|
| [OpenFOAM&reg;][repo openfoam] | [README][link openfoam-readme] | [System requirements][link openfoam-require] | [Build][link openfoam-build] |
| [ThirdParty][repo third] | [README][link third-readme] | [System requirements][link third-require] | [Build][link third-build] |


If you need to modify the versions or locations of ThirdParty
software, please read how the
[OpenFOAM&reg; configuration][wiki-config] is structured.
</details>

### How do I know which version I am currently using?

The value of the `$WM_PROJECT_DIR` or even `$WM_PROJECT_VERSION` are
not guaranteed to have any correspondence to the OpenFOAM&reg; release
(API) value. If OpenFOAM&reg; has already been compiled, the build-time
information is embedded into each application. For example, as
displayed from `blockMesh -help`:
```
Using: OpenFOAM-com (2012) - visit www.openfoam.com
Build: b830beb5ea-20210429 (patch=210414)
Arch:  LSB;label=32;scalar=64
```
This output contains all of the more interesting information that we need:

| item                  | value         |
|-----------------------|---------------|
| version               | com  (eg, local development branch) |
| api                   | 2012          |
| commit                | b830beb5ea    |
| author date           | 20210429      |
| patch-level           | (20)210414    |
| label/scalar size     | 32/64 bits    |

The Arch information may also include the `solveScalar` size
if different than the `scalar` size.

As can be seen in this example, the git build information is
supplemented by the date when the last change was authored, which can
be helpful when the repository contains local changes. If you simply
wish to know the current API and patch levels directly, the
`wmake -build-info` provides the relevant information even
when OpenFOAM&reg; has not yet been compiled:
```
$ wmake -build-info
make
    api = 2012
    patch = 210414
    branch = master
    build = 308af39136-20210426
```
Similar information is available with `foamEtcFile`, using the
`-show-api` or `-show-patch` options. For example,
```
$ foamEtcFile -show-api
2012

$ foamEtcFile -show-patch
210414
```
This output will generally be the easiest to parse for scripts.
The `$FOAM_API` convenience environment variable may not reflect the
patching changes made within the currently active environment and
should be used with caution.

### ThirdParty directory

OpenFOAM&reg; normally ships with a directory of 3rd-party software and
build scripts for some 3rd-party software that is either necessary or
at least highly useful for OpenFOAM&reg;, but which are not necessarily
readily available on every operating system or cluster installation.

These 3rd-party sources are normally located in a directory parallel
to the OpenFOAM&reg; directory. For example,
```
/path/parent
|-- OpenFOAM-v2606
\-- ThirdParty-v2606
```
There are, however, many cases where this simple convention is inadequate:

* When no additional 3rd party software is actually required (ie, the
  operating system or cluster installation provides it)

* When we have changed the OpenFOAM&reg; directory name to some arbitrary
  directory name, e.g. openfoam-sandbox2412, etc..

* When we would like any additional 3rd party software to be located
  inside of the OpenFOAM&reg; directory to ensure that the installation is
  encapsulated within a single directory structure. This can be
  necessary for cluster installations, or may simply be a convenient
  means of performing a software rollout for individual workstations.

* When we have many different OpenFOAM&reg; directories for testing or
  developing various different features but wish to use or reuse the
  same 3rd party software for them all.

The solution for these problems is a newer, more intelligent discovery
when locating the ThirdParty directory with the following precedence:

1. PROJECT/ThirdParty
   * for single-directory installations
2. PREFIX/ThirdParty-VERSION
   * this corresponds to the traditional approach
3. PREFIX/ThirdParty-vAPI
   * allows for an updated value of VERSION, *eg*, `v2606-myCustom`,
     without requiring a renamed ThirdParty. The API value would still
     be `2606` and the original `ThirdParty-v2606/` would be found.
4. PREFIX/ThirdParty-API
   * same as the previous example, but using an unadorned API value.
5. PREFIX/ThirdParty-common
   * permits maximum reuse for various versions, for experienced
     users who are aware of potential version incompatibilities

If none of these directories are found to be suitable, it reverts to
using PROJECT/ThirdParty as a dummy location (even if the directory
does not exist). This is a safe fallback value since it is within the
OpenFOAM&reg; directory structure and can be trusted to have no negative
side-effects. In the above, the following notation has been used:

| name          | value         | meaning       |
|---------------|---------------|---------------|
| PROJECT       | `$WM_PROJECT_DIR`     | The OpenFOAM&reg; directory |
| PREFIX        | `dirname $WM_PROJECT_DIR` | The OpenFOAM&reg; parent directory |
| API           | `foamEtcFiles -show-api` |  The api or release version |
| VERSION       | `$WM_PROJECT_VERSION` | The version we have chosen |

To reduce the potential of false positive matches (perhaps some other
software also uses ThirdParty-xxx for its naming), the directory test
is accompanied by a OpenFOAM&reg;-specific sanity test. The OpenFOAM&reg;
ThirdParty directory will contain either an `Allwmake` file or a
`platforms/` directory.

## How to use

You can start using OpenFOAM&reg; by launching a terminal<sup>[What is Linux terminal?](https://ubuntu.com/tutorials/command-line-for-beginners#1-overview),[What is Windows terminal?](https://learn.microsoft.com/en-us/windows/terminal/)</sup> window and loading the OpenFOAM&reg; environment in the terminal.

In its simplest form, simply source the appropriate `etc/bashrc` or `etc/cshrc` file to load the environment and start using OpenFOAM&reg; tools such as `blockMesh`:

```bash
source <absolute path of the installation>/OpenFOAM-v2606/etc/bashrc

cd $FOAM_TUTORIALS/incompressible/simpleFoam/pitzDaily

blockMesh
```

For more usage details see the [quickstart guide](https://doc.openfoam.com/2312/quickstart/).

## License

OpenFOAM&reg; is free and open-source software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.  See the file [LICENSE.md](./LICENSE.md) in this directory or
[http://www.gnu.org/licenses/](http://www.gnu.org/licenses), for a
description of the GNU General Public License terms under which you
may redistribute files.

## Trademark

OpenCFD Ltd grants use of its OpenFOAM&reg; trademark by Third Parties on a
licence basis. Keysight Technologies Ltd and OpenFOAM Foundation Ltd are currently
permitted to use the Name and agreed Domain Name. For information on
trademark use, please refer to the
[trademark policy guidelines][link trademark].

Please [contact Keysight Technologies](http://www.openfoam.com/contact) if you have
any questions about the use of the OpenFOAM&reg; trademark.

Violations of the Trademark are monitored, and will be duly prosecuted.

<!-- OpenFOAM -->

[link trademark]: https://www.openfoam.com/opencfd-limited-trade-mark-policy

[repo openfoam]: https://gitlab.com/openfoam/core/openfoam/
[repo third]: https://gitlab.com/openfoam/core/thirdparty-common/

[link openfoam-readme]: https://gitlab.com/openfoam/core/openfoam/blob/develop/README.md
[link openfoam-issues]: https://gitlab.com/openfoam/core/openfoam/blob/develop/doc/BuildIssues.md
[link openfoam-build]: https://gitlab.com/openfoam/core/openfoam/blob/develop/doc/Build.md
[link openfoam-require]: https://gitlab.com/openfoam/core/openfoam/blob/develop/doc/Requirements.md
[link third-readme]: https://gitlab.com/openfoam/core/thirdparty-common/blob/develop/README.md
[link third-build]: https://gitlab.com/openfoam/core/thirdparty-common/blob/develop/BUILD.md
[link third-require]: https://gitlab.com/openfoam/core/thirdparty-common/blob/develop/Requirements.md

[wiki-config]: https://gitlab.com/openfoam/core/openfoam/-/wikis/configuring

## Useful links

- [Source-code packs](https://dl.openfoam.com/source/)
- [Documentation](http://www.openfoam.com/documentation)
- [Reporting bugs/issues/feature requests](http://www.openfoam.com/code/bug-reporting.php)
- [Issue tracker](https://gitlab.com/openfoam/core/openfoam/-/issues)
- [Code wiki](https://gitlab.com/openfoam/core/openfoam/-/wikis/)
- [General wiki](http://wiki.openfoam.com/)
- [C++ source code guide](https://api.openfoam.com/2506/)
- [Governance](http://www.openfoam.com/governance/), [Governance Projects](https://www.openfoam.com/governance/projects)
- [Contact Keysight Technologies](http://www.openfoam.com/contact/)

Copyright 2016-2025 OpenCFD Ltd
Copyright 2026 Keysight Technologies

<!----------------------------------------------------------------------------->
