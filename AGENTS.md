# OpenFOAM v2606 — Windows/MSVC port notes

## Build

Toolchain: VS2022 (MSVC 14.43), CMake ≥3.20, x64, C++17, DP/label32, MS-MPI.

```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
MSBuild build\OpenFOAM.sln -p:Configuration=Release -m
```

Shared (DLL) build — every library becomes its own DLL:

```bat
cmake -S . -B build-shared -G "Visual Studio 17 2022" -A x64 -DFOAM_STATIC_LIBS=OFF
MSBuild build-shared\OpenFOAM.sln -p:Configuration=Release -m
```

`FOAM_STATIC_LIBS=ON` (default) = static `/WHOLEARCHIVE` model;
`OFF` = shared DLLs (`FOAM_SHARED_LIBS` defined, activates the
`<Lib>_API` import/export layer). Same sources, same annotations —
the macros expand empty in static mode.

Or per target:

```bat
MSBuild build\src\<lib>.vcxproj -p:Configuration=Release -p:Platform=x64 -m
MSBuild build\applications\<app>.vcxproj -p:Configuration=Release -p:Platform=x64 -m
```

All apps build by default (`FOAM_APP_<name>` defaults ON; disable one
with `-DFOAM_APP_<name>=OFF`, see `applications/CMakeLists.txt`).
Library deps build automatically as project references. App-local
helper libraries are registered via `foam_app_lib(<dir>)` and get the
same DLL/API treatment as src libs. Full shared builds should use
`/m:2` — higher parallelism has hit C1060 (compiler heap exhaustion)
on the largest TUs.

## Layout

- `cmake/wmake2cmake.py` — parses upstream `Make/files` + `Make/options`
  into `build/cmake-gen/*.cmake` fragments (sources/includes/defines/libs).
- `cmake/FoamMacros.cmake` — `foam_parse_dir`, `foam_add_library`,
  `foam_add_executable`, `foam_resolve_includes`, flex/m4/lemon generators.
- `cmake/compat/msvcCompat.h` — force-included (`/FI`) into every TU.
  Only aliases for POSIX names that are genuinely absent from UCRT
  (lstat, strcasecmp, strncasecmp, strtok_r, alloca...). Do NOT add
  `#define` for names like read/write/close — they rewrite C++ member
  calls (`os.write()` → `os._write()`).
- `cmake/compat/foamApi.h` — per-target `<Lib>_API` macros (also
  force-included via msvcCompat.h). Resolve to
  `__declspec(dllexport)` inside the owning lib (`<Lib>_EXPORTS` is
  auto-defined by CMake for SHARED targets), `dllimport` for
  consumers, empty under `FOAM_STATIC_LIBS`. Symbols declared by
  `OSspecific/` and `Pstream/` headers live in libOpenFOAM.dll —
  annotate with `OpenFOAM_API`. Maintenance tools:
  `cmake/annotateClass.py` (class-decl annotation),
  `cmake/harvestUnresolved.py` (classify LNK2019 symbols),
  `cmake/_bulk_annotate.py` (member-level data annotation).
- `cmake/genExportsDef.py` — PRE_LINK step per DLL: scans the
  target's `.obj` files with dumpbin and writes `foam_exports.def`
  with all strong symbols. Required because
  `WINDOWS_EXPORT_ALL_SYMBOLS` hit a link.exe .exp-generation bug on
  the `__imp___iob_func` shim, and because declspec alone cannot
  cover symbols pulled in via embedded OBJECT libraries.
- `build/lnInclude/` — copied headers replacing upstream symlinked
  lnInclude. Re-run CMake configuration after editing `src/**` headers to
  refresh the generated copies before building.
- `build/bin/Release`, `build/lib/Release` — exe/lib output.

## Key conventions

- Static mode: each library propagates `/WHOLEARCHIVE` through its
  CMake interface, preserving runTimeSelection registration for an
  executable's transitive dependency closure. Add optional static
  plugins to the application's `Make/options` `EXE_LIBS`.
- Shared mode: library deps link `PUBLIC` so import libs propagate
  transitively (mirrors ELF DT_NEEDED; upstream `Make/options`
  under-declares direct deps — e.g. conformalVoronoiMesh uses
  finiteVolume via dynamicMesh but never links -lfiniteVolume).
  `/FORCE:MULTIPLE` is set globally: genExportsDef re-exports COMDAT
  template instantiations (e.g. `HashSet<label>` members forced by
  dllexport'd classes) that consumers also instantiate locally —
  identical code, safe to fold. All *data* symbols are explicitly
  owned via API annotations, never duplicated.
- DLL data-sympol ownership rules (MSVC auto-imports functions via
  linker thunks but never data — every cross-DLL data decl needs the
  owner macro):
  - `ClassNameApi(<Lib>_API, "name")` / `TypeNameApi(...)` /
    `NamespaceNameApi(...)` carry declspec for `typeName`/`debug`
    statics.
  - `declareRunTimeSelectionTable{,New}Api(<Lib>_API, ...)` and
    `declareMemberFunctionSelectionTableApi` annotate table
    singletons. Table-pointer access goes through
    `TableInsert`/`TableSet`/`TableErase` member functions — MSVC
    emits bare refs (no `__imp_`) to extern-template static *data*
    members through nested adder templates, while function refs
    always resolve.
  - Template classes whose spec definitions live in other TUs use
    the opt-out pattern: header does
    `#if defined(Foam_X_defines_typeName)` → undecorated decl, else
    `<Lib>_API`; the owning .C defines `Foam_X_defines_typeName`
    *before* any include.
  - Closed instance sets (Function1/PatchFunction1 families) use
    `<Lib>_TEMPLATE_IMPORT` in the header + `<Lib>_TEMPLATE_EXPORT`
    (`template class`) in the owner TU — the only MSVC-legal way to
    import statics of a template specialization (declspec on
    `template<>` member decls is C2720; annotating the primary
    template's member breaks downstream spec defs, C2491).
  - Class-level `<Lib>_API` only for classes whose vtable crosses
    DLL boundaries. Beware: exporting a class forces instantiation
    of lazy members — `UList<token>` needed constexpr guards on
    `expr()`/`fill_uniform`/`operator<`, and dllexport classes
    instantiated over dllimport-member bases hit C2487.
- App target dir comes AFTER library includes so `<CorrectPhi.H>`
  resolves to the library header, not the case-colliding local
  `correctPhi.H` fragment (Windows FS is case-insensitive).
- `FOAM_CONFIGURED_PROJECT_DIR` is baked into libOpenFOAM so
  `#includeEtc`/`etc/caseDicts` resolve without `WM_PROJECT_DIR`.
- `FOAM_STATIC_BUILD` makes `dlOpen` fall back to the process image.
  Any `libs` plugin must be present in the application's static
  dependency closure. In shared builds `dlOpen` loads the DLL
  normally — `controlDict` `libs("libX")` verified working.
- MSVC has no key-function vtable optimisation: any TU that sees a
  complete `GeometricField` must also see the complete patch-field
  type (include `volFields.H`/`surfaceFields.H`, not just `*Fwd.H`).
- Injected class names (`Field`, `fvPatchField`, `flux`,
  `pointPatchField`) shadow same-named templates inside derived
  classes — qualify with `Foam::` where MSVC reports ambiguity.
- Inherited `IOobject`/`IOdictionary` from multiple base paths needs
  `Foam::` qualification (C2385).
- `min`/`max`/`log`/`component` calls can hit members or bool data
  members on MSVC — qualify with `Foam::`.
- The `Foam_<Tpl>_defines_typeName` opt-out is class-wide: a TU that
  defines SOME specs of `<Tpl>` AND consumes others loses dllimport on
  all of them → LNK2019 for the consumed statics. MSVC cannot re-import
  single specialisations (C2720/C2370 on `template<> extern` declspec),
  so such mixed TUs must also `defineTemplateTypeNameAndDebug` the
  consumed specs locally (e.g. `Test-volField`), or drop the flag when
  the spec def is `#ifdef`'d out (e.g. `Test-dimField1`).
- Never annotate a NON-template class with a spec flag macro
  (`Foam_<Tpl>_typeName_API`) — its single DLL-side definition then
  disappears for flag-carrying consumers (`fvPatchFieldBase` bug). Use
  the fixed `<Lib>_API` for concrete classes.
- Windows filename collisions that actually bite: `correctPhi.H` (app
  fragment) vs `CorrectPhi.H` (library header) — library includes use
  `finiteVolume/CorrectPhi.H` qualified paths; pointer files
  (`#include "X"` forwarding headers) must contain real includes, not
  raw relative paths.
- MS-MPI is MPI-2: no `MPI_Neighbor_alltoall` — emulated with pairwise
  `MPI_Sendrecv` over the neighbour list (see
  `applications/test/processorTopology`).
- MSBuild occasionally idles after the last target finishes (node-reuse
  hang). Verify completion by error count and a missing-output sweep,
  then kill the lingering MSBuild.exe.

## Run

```bat
call etc\openfoam-env.bat              rem static build tree (build\)
call etc\openfoam-env.bat build-shared rem shared build tree
```

or set `PATH=<bld>\bin\Release;<bld>\lib\Release;<repo>\thirdparty\fftw;%PATH%`
(`lib\Release` holds the DLLs in shared builds; harmless in static).
`WM_PROJECT_DIR` is optional (compile-time fallback baked in).

## Verified runtime

- `blockMesh`, `topoSet`, `setFields`, `checkMesh`, `transformPoints`,
  `decomposePar`/`reconstructPar`/`reconstructParMesh`,
  `redistributePar`, `foamToVTK`, `foamDictionary`,
  `foamFormatConvert`, `patchSummary`, `postProcess`
- `laplacianFoam` (flange, cyclicAMI+GAMG), `icoFoam` (cavity 0.5 s,
  serial + 9-rank MPI), `pisoFoam` (RAS cavity 0–10 s, k-ε+FOs),
  `simpleFoam` (pitzDaily, converged, streamlines), `pimpleFoam`
  (movingCone, dynamic mesh + GAMG + vtkWrite), `potentialFoam`,
  `interFoam` (damBreak VOF), `snappyHexMesh` (motorBike 3.8 M cells,
  serial + MPI, CGAL kernel), `foamyHexMesh` (CGAL blob)
- Shared-build extras: `controlDict` `libs("libutilityFunctionObjects")`
  plugin load; missing lib warns gracefully.
- Static regression (`FOAM_STATIC_LIBS=ON`): libOpenFOAM,
  libfiniteVolume, icoFoam compile + link + run cavity end-to-end.

## Caution

- Run ONE MSBuild invocation at a time. Stale `MSBuild.exe` node-reuse
  processes lock `.obj`/`.tlog` files → `C1083 Permission denied`.
  Kill lingering MSBuild/cl before rebuilding.
- Parallelism: `-m:2` is the safe default (32 GB RAM). `-m` alone spawns
  too many concurrent cl.exe (project-level × /MP) → C1060 heap
  exhaustion / pagefile errors.
- MSBuild incremental builds under-track `lnInclude` header changes
  (the /MP tlog shard merge drops header deps after compile errors).
  After editing `src/**` headers, Rebuild the affected targets rather
  than relying on incremental — stale .obj files produce misleading
  bare-reference LNK2019s.
- Changing any annotation in a widely-included header (foamApi.h,
  runTimeSelectionTables.H, className.H) effectively requires a full
  rebuild — plan accordingly.

## MPI / decomposition / CGAL

- `FOAM_MPI=msmpi` (default). Vendored SDK in `thirdparty/msmpi`
  (headers, `msmpi.lib`, `msmpi.dll`, `mpiexec.exe`, `smpd.exe`).
  `mpiexec -n N <solver> -parallel` verified end-to-end on
  `icoFoam/cavity` (decomposePar → parallel solve → reconstructPar).
- Decomposition: real `metis`, `scotch`, `ptscotch`, `kahip` backends.
  METIS/SCOTCH/PT-SCOTCH are vendored conda-forge int32 libs
  (`thirdparty/{metis,scotch,ptscotch}`); KaHIP is vendored as a
  prebuilt MSVC `kahip.lib` + `kaHIP_interface.h`
  (`thirdparty/kahip`, built from KaHIP sources with MSVC).
  `metis.lib` needs `cmake/compat/legacyStdioShim.c` (provides
  `__imp___iob_func` for its VS2013-era CRT).
- MGridGen GAMG agglomeration: serial MGridGen sources are vendored
  under `thirdparty/mgridgen` and compiled in-tree as the `mgrid`
  static target (MSVC shim `msvc_drand48.c` provides `drand48`/
  `srand48`; internal `errexit` renamed to `mgridgen_errexit` to
  avoid symbol clashes). `src/fvAgglomerationMethods/MGridGenGamgAgglomeration`
  builds the real `MGridGenGAMGAgglomeration` lib, force-linked into
  `finiteVolume` so `agglomerator MGridGen` registers in every solver.
  Verified on `icoFoam` cavity (GAMG + `agglomerator MGridGen`).
  Requires `minSize`/`maxSize`/`nProcConsistencyIter` entries in the
  solver dict.
- CGAL 6.x + GMP/MPFR vendored (`thirdparty/{cgal,gmp,mpfr}`). CGAL
  apps enabled: `foamyHexMesh` (verified on `mesh/foamyHexMesh/blob`),
  `foamyQuadMesh`, `cv2DMesh`, `cellSizeAndAlignmentGrid`,
  `foamyHexMeshBackgroundMesh`, `viewFactorsGen`,
  `surfaceBooleanFeatures`.
  `foamyHexMeshSurfaceSimplify` is skipped — needs external
  `fastdualoctree_sgp` + OpenGL (upstream skips it identically).
- Runtime DLLs (`msmpi.dll`, `mpir.dll`, `libmpfr-6.dll`, fftw) are
  staged next to the exes; `etc/openfoam-env.bat` sets all PATHs.

## Remaining optional components

- `kahip` and `mgridgen` are real implementations (see above); the
  `src/dummyThirdParty/{kahipDecomp,MGridGen}` stubs remain as
  configure-time fallbacks when the vendored dependencies are absent.
- `foamyHexMeshSurfaceSimplify` remains disabled because the external
  `fastdualoctree_sgp` and OpenGL dependency is unavailable; upstream also
  skips this application when that optional dependency is absent.
