# Internals / 移植技术内幕

**English** | [中文](#中文)

How the port works under the hood. For the full gory details see
`AGENTS.md` in the repo root — this page is the guided tour.

---

## English

### Build system

`cmake/wmake2cmake.py` parses upstream `Make/files` + `Make/options`
into `build/cmake-gen/*.cmake` fragments (sources / includes / defines /
libs); `cmake/FoamMacros.cmake` provides `foam_parse_dir`,
`foam_add_library`, `foam_add_executable`, `foam_resolve_includes` and
the flex/m4/lemon generators. The upstream wmake files stay the single
source of truth — the CMake build is generated *from* them.

### Two linking models, one source tree

- **Static** (`FOAM_STATIC_LIBS=ON`, default): every library
  propagates `/WHOLEARCHIVE` through its CMake interface, so the whole
  archive — including runTimeSelection self-registration objects — is
  pulled into each executable's transitive dependency closure.
- **Shared** (`OFF`): each library becomes its own DLL. `FOAM_SHARED_LIBS`
  activates the `<Lib>_API` import/export layer; the same annotations
  expand to nothing in static mode.

Shared-mode deps link `PUBLIC` so import libs propagate transitively
(mirroring ELF `DT_NEEDED` — upstream `Make/options` under-declares
direct deps). `/FORCE:MULTIPLE` is global because the export scanner
re-exports COMDAT template instantiations that consumers also emit
locally — identical code, safe to fold.

### Symbol export machinery

MSVC auto-imports functions via linker thunks but **never data** —
every cross-DLL data declaration needs explicit `__declspec`.

- `cmake/compat/foamApi.h` defines a `<Lib>_API` macro per target
  (force-included via `msvcCompat.h`). Inside the owning lib
  (`<Lib>_EXPORTS` auto-defined by CMake) it expands to `dllexport`;
  for consumers, `dllimport`; empty under `FOAM_STATIC_LIBS`.
- `cmake/genExportsDef.py` runs PRE_LINK per DLL: it scans the
  target's `.obj` files with dumpbin and writes `foam_exports.def` of
  all strong symbols. Needed because `WINDOWS_EXPORT_ALL_SYMBOLS` hit
  a link.exe .exp bug, and declspec can't cover symbols pulled in via
  embedded OBJECT libraries.
- Ownership annotations:
  - `ClassNameApi` / `TypeNameApi` / `NamespaceNameApi` carry declspec
    for `typeName`/`debug` statics.
  - `declareRunTimeSelectionTable{,New}Api` /
    `declareMemberFunctionSelectionTableApi` annotate table singletons;
    access goes through `TableInsert`/`TableSet`/`TableErase` member
    functions (MSVC emits bare refs to extern-template static *data*
    through nested adder templates).
  - `Foam_<Tpl>_defines_typeName` opt-out: a header emits the plain
    decl when the flag is set (the owning .C defines it before
    includes), `<Lib>_API` otherwise.
  - Closed template-spec sets (Function1/PatchFunction1) use
    `<Lib>_TEMPLATE_IMPORT` in headers + `<Lib>_TEMPLATE_EXPORT`
    (`template class`) in the owner TU — the only MSVC-legal way to
    import statics of a specialization.

### lnInclude without symlinks

`build/lnInclude/` is generated as **copied** headers (no symlinks on
Windows). Each `lnInclude/<lib>` dir is made NTFS case-sensitive at
configure time (`SetFileInformationByHandle`/`FileCaseSensitiveInfo`,
no elevation), so Foam `string.H`/`Time.H`/`wchar.H` cannot shadow
`<string.h>`/`<time.h>`/`<wchar.h>`. If the flag can't be set the
generator falls back to merged shim headers — that path poisons
system-header lookup order and is treated as unsupported.

### MSVC language quirks fixed

- No key-function vtable optimisation: any TU seeing a complete
  `GeometricField` must also see the complete patch-field type.
- Injected class names (`Field`, `fvPatchField`, `flux`,
  `pointPatchField`) shadow same-named templates in derived classes —
  qualified with `Foam::`.
- Inherited `IOobject`/`IOdictionary` via multiple base paths → C2385,
  fixed with `Foam::` qualification.
- `min`/`max`/`log`/`component` calls hitting members → `Foam::`-
  qualified.
- `msvcCompat.h` aliases only POSIX names genuinely absent from UCRT
  (`lstat`, `strcasecmp`, `strtok_r`, `alloca`, ...). It deliberately
  does NOT define `read`/`write`/`close` — those would rewrite C++
  member calls (`os.write()` → `os._write()`).

### Platform layer

`src/OSspecific/MSwindows/` — including `printStack.cxx`:
`CaptureStackBackTrace` + DbgHelp (`SymFromAddr`, `SymGetLineFromAddr64`,
`UnDecorateSymbolName`), `try_lock`-guarded (DbgHelp is
single-threaded), falls back to module+offset. `FOAM_ENABLE_PDB` keeps
file:line resolution.

### MPI

MS-MPI is MPI-2: `MPI_Neighbor_alltoall` doesn't exist — emulated with
pairwise `MPI_Sendrecv`. A missing `MPI_Comm_create_group` collective
mismatch was also fixed. Passive-target RMA (`MPI_Win` +
`MPI_Fetch_and_op`) can hang on process exit — an MS-MPI limitation,
not a port bug.

### Runtime fixes worth knowing

- `argv[0]` backslash / `.exe` suffix handling.
- Empty compound `typeName` polluting the token table → parallel
  deserialisation bug (fixed).
- conda-forge `metis.lib` is pre-UCRT — `cmake/compat/legacyStdioShim.c`
  supplies `__imp___iob_func`.
- Executables link `/STACK:8388608` (8 MB) matching the Linux default —
  Windows' 1 MB overflows on OpenFOAM's large automatic objects.

### Third-party stack (all vendored)

| Package | Form |
|---|---|
| MS-MPI | SDK: headers + `msmpi.lib`/`msmpi.dll` + `mpiexec`/`smpd` |
| METIS, SCOTCH, PT-SCOTCH | conda-forge int32 libs |
| KaHIP | MSVC-built `kahip.lib` |
| MGridGen | serial sources compiled in-tree as `mgrid` |
| CGAL 6.x, GMP, MPFR | vendored; powers foamyHexMesh & friends |
| libccmio 2.6.1 | in-tree `ccmio` static target (foam-extend drop) |
| FFTW, zlib, win_flex, lemon, m4 | vendored binaries/sources |

Dummy stubs (`src/dummyThirdParty/{kahipDecomp,MGridGen}`) remain as
configure-time fallbacks when vendored deps are absent.

---

## 中文

[English](#english) | **中文**

移植的内部工作原理。完整细节见仓库根目录 `AGENTS.md`,本页是导览版。

### 构建系统

`cmake/wmake2cmake.py` 把上游 `Make/files` + `Make/options` 解析成
`build/cmake-gen/*.cmake` 片段(源文件/头文件路径/宏/库);
`cmake/FoamMacros.cmake` 提供 `foam_parse_dir`、`foam_add_library`、
`foam_add_executable`、`foam_resolve_includes` 和 flex/m4/lemon
生成器。上游 wmake 文件仍是唯一事实来源 —— CMake 构建是从它们
*生成*的。

### 两种链接模型,同一套源码

- **静态**(`FOAM_STATIC_LIBS=ON`,默认):每个库通过 CMake
  interface 传播 `/WHOLEARCHIVE`,整个归档(含 runTimeSelection
  自注册对象)被拉进可执行文件的传递依赖闭包。
- **共享**(`OFF`):每个库独立产出 DLL。`FOAM_SHARED_LIBS` 激活
  `<Lib>_API` 导入/导出层;同一套标注在静态模式下展开为空。

共享模式下库依赖以 `PUBLIC` 链接,导入库可传递(等价 ELF
`DT_NEEDED` —— 上游 `Make/options` 有欠声明直接依赖的情况)。
全局 `/FORCE:MULTIPLE`:导出扫描器会重复导出消费方本地也实例化
的 COMDAT 模板实例 —— 代码相同,折叠安全。

### 符号导出机制

MSVC 通过链接器 thunk 自动导入函数,但**数据符号永远不自动导入**
—— 每个跨 DLL 数据声明都要显式 `__declspec`。

- `cmake/compat/foamApi.h` 为每个目标定义 `<Lib>_API` 宏(经
  `msvcCompat.h` 强制包含)。属主库内(CMake 自动定义
  `<Lib>_EXPORTS`)展开为 `dllexport`;消费方为 `dllimport`;
  静态模式为空。
- `cmake/genExportsDef.py` 在每个 DLL 的 PRE_LINK 阶段运行:用
  dumpbin 扫描 `.obj` 收割强符号生成 `foam_exports.def`。必须这样
  是因为 `WINDOWS_EXPORT_ALL_SYMBOLS` 触发了 link.exe 的 .exp 生成
  bug,且 declspec 覆盖不了经 OBJECT 库嵌入的符号。
- 属主标注:
  - `ClassNameApi`/`TypeNameApi`/`NamespaceNameApi` 为
    `typeName`/`debug` 静态成员带 declspec;
  - `declareRunTimeSelectionTable{,New}Api`/
    `declareMemberFunctionSelectionTableApi` 标注选择表单例;
    访问走 `TableInsert`/`TableSet`/`TableErase` 成员函数(MSVC
    对嵌套 adder 模板里的 extern-template 静态*数据*成员会发出裸
    引用);
  - `Foam_<Tpl>_defines_typeName` opt-out:头文件在宏已定义时输出
    裸声明(属主 .C 在 include 之前定义它),否则 `<Lib>_API`;
  - 封闭模板特化集合(Function1/PatchFunction1 家族)用
    `<Lib>_TEMPLATE_IMPORT`(头文件)+ `<Lib>_TEMPLATE_EXPORT`
    (`template class`,属主 TU)—— MSVC 下导入特化静态成员的
    唯一合法方式。

### 无符号链接的 lnInclude

`build/lnInclude/` 用**拷贝**头文件实现(Windows 没有可靠的符号
链接)。每个 `lnInclude/<lib>` 目录在配置期被设为 NTFS 大小写敏感
(`SetFileInformationByHandle`/`FileCaseSensitiveInfo`,无需提权),
使 Foam 的 `string.H`/`Time.H`/`wchar.H` 不会遮蔽
`<string.h>`/`<time.h>`/`<wchar.h>`。设不了该标志时生成器回退到
合并 shim 头 —— 这条路会污染系统头文件查找顺序,视为不支持。

### 修复过的 MSVC 语言怪癖

- 无 key-function vtable 优化:看到完整 `GeometricField` 的 TU 必须
  同时看到完整 patch-field 类型。
- 注入类名(`Field`、`fvPatchField`、`flux`、`pointPatchField`)
  在派生类里遮蔽同名模板 —— 用 `Foam::` 限定。
- 多基类路径继承的 `IOobject`/`IOdictionary` → C2385,加 `Foam::`。
- `min`/`max`/`log`/`component` 撞成员名 → `Foam::` 限定。
- `msvcCompat.h` 只为 UCRT 真正缺的 POSIX 名做别名(`lstat`、
  `strcasecmp`、`strtok_r`、`alloca`……)。刻意不定义
  `read`/`write`/`close` —— 那会改写 C++ 成员调用
  (`os.write()` → `os._write()`)。

### 平台层

`src/OSspecific/MSwindows/` —— 含 `printStack.cxx`:
`CaptureStackBackTrace` + DbgHelp(`SymFromAddr`、
`SymGetLineFromAddr64`、`UnDecorateSymbolName`),`try_lock` 保护
(DbgHelp 单线程),争用时回退模块+偏移。`FOAM_ENABLE_PDB` 保持
文件:行号解析。

### MPI

MS-MPI 是 MPI-2:没有 `MPI_Neighbor_alltoall`,用成对
`MPI_Sendrecv` 模拟;另修复了缺失 `MPI_Comm_create_group` 导致的
集合调用错配。Passive-target RMA(`MPI_Win` +
`MPI_Fetch_and_op`)可能在进程退出时挂起 —— 这是 MS-MPI 的限制,
不是移植 bug。

### 值得知道的运行时修复

- `argv[0]` 反斜杠 / `.exe` 后缀处理。
- 空 compound `typeName` 污染 token 表 → 并行反序列化 bug(已修)。
- conda-forge `metis.lib` 是 pre-UCRT 时代产物 ——
  `cmake/compat/legacyStdioShim.c` 提供 `__imp___iob_func`。
- 可执行文件链接 `/STACK:8388608`(8 MB,对齐 Linux 默认)——
  Windows 默认 1 MB 装不下 OpenFOAM 的大型自动对象。

### 第三方依赖(全部 vendored)

| 包 | 形式 |
|---|---|
| MS-MPI | SDK:头文件 + `msmpi.lib`/`msmpi.dll` + `mpiexec`/`smpd` |
| METIS、SCOTCH、PT-SCOTCH | conda-forge int32 库 |
| KaHIP | MSVC 构建的 `kahip.lib` |
| MGridGen | 串行源码树内编译为 `mgrid` |
| CGAL 6.x、GMP、MPFR | vendored;支撑 foamyHexMesh 系列 |
| libccmio 2.6.1 | 树内 `ccmio` 静态目标(foam-extend 公开包) |
| FFTW、zlib、win_flex、lemon、m4 | vendored 二进制/源码 |

`src/dummyThirdParty/{kahipDecomp,MGridGen}` 的 dummy stub 保留为
vendored 依赖缺席时的配置期回退。
