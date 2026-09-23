# Build / 构建指南

**English** | [中文](#中文)

---

## English

### Requirements

| Component | Version |
|---|---|
| OS | Windows 10/11 x64 |
| Compiler | Visual Studio 2022, MSVC 14.43+ (Desktop C++ workload) |
| CMake | ≥ 3.20 |
| RAM | 32 GB recommended for full builds |
| Git | `git config --global core.longpaths true` recommended |

All third-party dependencies are vendored under `thirdparty/` — nothing
else to install. Python 3 (for `cmake/wmake2cmake.py`) must be on PATH.

### Quick start

```bat
git clone https://github.com/WHUT666/OpenFOAM-v2606-Windows.git
cd OpenFOAM-v2606-Windows

rem --- static build (default, FOAM_STATIC_LIBS=ON) ---
etc\build-windows.bat

rem --- or: shared DLL build ---
cmake -B build-shared -A x64 -DFOAM_MPI=msmpi -DFOAM_STATIC_LIBS=OFF
etc\build-windows.bat /d:build-shared
```

`build-windows.bat` locates MSBuild via `vswhere`, kills stale
MSBuild/cl/link nodes (file-lock prevention), configures if needed and
builds `Release` with `-m:2`. Pass target names to build a subset:

```bat
etc\build-windows.bat icoFoam decomposePar     rem selected targets
etc\build-windows.bat /c                        rem force re-configure
etc\build-windows.bat /d:build-shared icoFoam   rem other build tree
```

### Manual build

```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
MSBuild build\OpenFOAM.sln -p:Configuration=Release -m:2
```

Per target:

```bat
MSBuild build\src\<lib>.vcxproj -p:Configuration=Release -p:Platform=x64 -m:2
MSBuild build\applications\<app>.vcxproj -p:Configuration=Release -p:Platform=x64 -m:2
```

Library dependencies resolve automatically through project references.

### CMake options

| Option | Default | Meaning |
|---|---|---|
| `FOAM_STATIC_LIBS` | ON | ON = static `/WHOLEARCHIVE` model; OFF = one DLL per library |
| `FOAM_MPI` | msmpi | vendored MS-MPI SDK in `thirdparty/msmpi` |
| `FOAM_PRECISION` | DP | double precision |
| `FOAM_LABEL_SIZE` | 32 | label width |
| `FOAM_APP_<name>` | ON | per-application switch (`-DFOAM_APP_<name>=OFF` to skip one) |
| `FOAM_APP_TESTS` | ON | OFF skips the whole `applications/test/` tree (~320 targets) |
| `FOAM_PLUGINS` / `FOAM_MODULES` | ON | cfmesh, avalanche, ... submodule targets |
| `FOAM_ENABLE_PDB` | ON | `/Zi` + `/DEBUG` so crash stack traces resolve to file:line |

### Output layout

```
build\bin\Release\     executables (+ staged runtime DLLs)
build\lib\Release\     .lib / .dll (shared builds)
build\imp\Release\     import libraries (shared builds)
build\lnInclude\       generated header tree (per-library flat dirs)
```

### Packaging

```bat
powershell -File etc\package-windows.ps1
```

Produces `dist/OpenFOAM-v2606-Windows-x64/` (+ `.7z` if 7-Zip is on
PATH): `bin`, `lib`, `imp`, `include`, `etc`, `tutorials`,
`thirdparty` runtime subset and a `setenv.bat`. Options: `-BuildDir`,
`-OutDir`, `-Name`, `-NoArchive`, `-IncludePdb`.

CI does the same on `windows-2022` (`.github/workflows/windows-package.yml`):
trigger on `v*` tags or manual dispatch → shared-DLL build →
regression suite → package → in-package smoke test
(blockMesh/icoFoam/checkMesh/foamToCcm on cavity) → artifact/Release.

### Build gotchas

- **One MSBuild at a time.** Stale node-reuse processes lock
  `.obj`/`.tlog` files → `C1083 Permission denied`. `build-windows.bat`
  kills them for you; if building manually, kill `MSBuild.exe`,
  `cl.exe`, `link.exe`, `mspdbsrv.exe` first.
- **`-m:2` is the safe parallelism.** Bare `-m` multiplies project-level
  × `/MP` concurrency → `C1060` compiler heap exhaustion on the
  largest TUs.
- **After editing `src/**` headers, re-run CMake configure** — the
  `lnInclude` copies must be regenerated; incremental builds
  under-track them (stale .obj files cause misleading LNK2019s).
- MSBuild occasionally idles after finishing (node-reuse hang). Check
  error count, then kill the lingering process.

---

## 中文

[English](#english) | **中文**

### 环境要求

| 组件 | 版本 |
|---|---|
| 操作系统 | Windows 10/11 x64 |
| 编译器 | Visual Studio 2022,MSVC 14.43+(C++ 桌面开发工作负载) |
| CMake | ≥ 3.20 |
| 内存 | 全量构建建议 32 GB |
| Git | 建议 `git config --global core.longpaths true` |

第三方依赖全部 vendored 在 `thirdparty/`,无需额外安装。
`cmake/wmake2cmake.py` 需要 PATH 上有 Python 3。

### 快速构建

```bat
git clone https://github.com/WHUT666/OpenFOAM-v2606-Windows.git
cd OpenFOAM-v2606-Windows

rem --- 静态构建(默认,FOAM_STATIC_LIBS=ON)---
etc\build-windows.bat

rem --- 或:共享 DLL 构建 ---
cmake -B build-shared -A x64 -DFOAM_MPI=msmpi -DFOAM_STATIC_LIBS=OFF
etc\build-windows.bat /d:build-shared
```

`build-windows.bat` 通过 `vswhere` 定位 MSBuild,先清理残留的
MSBuild/cl/link 进程(防止文件锁),必要时自动配置,然后以 `-m:2`
构建 Release。可以只构建指定目标:

```bat
etc\build-windows.bat icoFoam decomposePar     rem 只构建指定目标
etc\build-windows.bat /c                        rem 强制重新配置
etc\build-windows.bat /d:build-shared icoFoam   rem 指定构建目录
```

### 手动构建

```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
MSBuild build\OpenFOAM.sln -p:Configuration=Release -m:2
```

单目标:

```bat
MSBuild build\src\<lib>.vcxproj -p:Configuration=Release -p:Platform=x64 -m:2
MSBuild build\applications\<app>.vcxproj -p:Configuration=Release -p:Platform=x64 -m:2
```

库依赖通过 project reference 自动构建。

### CMake 选项

| 选项 | 默认 | 说明 |
|---|---|---|
| `FOAM_STATIC_LIBS` | ON | ON = 静态 `/WHOLEARCHIVE` 模式;OFF = 每库一个 DLL |
| `FOAM_MPI` | msmpi | 使用 `thirdparty/msmpi` vendored SDK |
| `FOAM_PRECISION` | DP | 双精度 |
| `FOAM_LABEL_SIZE` | 32 | label 位宽 |
| `FOAM_APP_<name>` | ON | 单应用开关(`-DFOAM_APP_<name>=OFF` 跳过) |
| `FOAM_APP_TESTS` | ON | OFF 跳过整个 `applications/test/`(~320 个目标) |
| `FOAM_PLUGINS` / `FOAM_MODULES` | ON | cfmesh、avalanche 等子模块目标 |
| `FOAM_ENABLE_PDB` | ON | `/Zi` + `/DEBUG`,崩溃栈可解析到文件:行号 |

### 输出布局

```
build\bin\Release\     可执行文件(+ 随附运行时 DLL)
build\lib\Release\     .lib / .dll(共享构建)
build\imp\Release\     导入库(共享构建)
build\lnInclude\       生成的头文件树(每库一个平铺目录)
```

### 打包

```bat
powershell -File etc\package-windows.ps1
```

产出 `dist/OpenFOAM-v2606-Windows-x64/`(PATH 上有 7-Zip 时附带
`.7z`):`bin`、`lib`、`imp`、`include`、`etc`、`tutorials`、
`thirdparty` 运行时子集和 `setenv.bat`。选项:`-BuildDir`、
`-OutDir`、`-Name`、`-NoArchive`、`-IncludePdb`。

CI 在 `windows-2022` 上执行同样流程
(`.github/workflows/windows-package.yml`):推 `v*` tag 或手动触发
→ 共享 DLL 构建 → 回归测试 → 打包 → 包内冒烟测试(cavity 上跑
blockMesh/icoFoam/checkMesh/foamToCcm)→ artifact/Release。

### 构建注意事项

- **一次只跑一个 MSBuild。** 残留的 node-reuse 进程会锁
  `.obj`/`.tlog` 导致 `C1083 Permission denied`。
  `build-windows.bat` 会自动清理;手动构建时先杀 `MSBuild.exe`、
  `cl.exe`、`link.exe`、`mspdbsrv.exe`。
- **并行度用 `-m:2`。** 裸 `-m` 会让项目级并行 × `/MP` 叠乘,
  大 TU 上触发 `C1060` 编译器堆耗尽。
- **改完 `src/**` 头文件要重新跑 CMake 配置** —— `lnInclude`
  拷贝需要重新生成;增量构建对它的依赖追踪不完整(陈旧 .obj
  会产生误导性的 LNK2019)。
- MSBuild 偶尔在构建结束后挂起(node-reuse hang)。确认错误数后
  杀掉残留进程即可。
