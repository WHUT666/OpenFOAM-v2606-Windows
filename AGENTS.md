# OpenFOAM v2606 — Windows/MSVC port notes

## Build

Toolchain: VS2022 (MSVC 14.43), CMake ≥3.20, x64, C++17, DP/label32, MS-MPI.

```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
MSBuild build\OpenFOAM.sln -p:Configuration=Release -m
```

Or per target:

```bat
MSBuild build\src\<lib>.vcxproj -p:Configuration=Release -p:Platform=x64 -m
MSBuild build\applications\<app>.vcxproj -p:Configuration=Release -p:Platform=x64 -m
```

Apps are enabled via `FOAM_APP_<name>` CMake options (see
`applications/CMakeLists.txt`). Library deps build automatically as project
references.

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
- `build/lnInclude/` — copied headers replacing upstream symlinked
  lnInclude. Re-run CMake configuration after editing `src/**` headers to
  refresh the generated copies before building.
- `build/bin/Release`, `build/lib/Release` — exe/lib output.

## Key conventions

- Each static library propagates `/WHOLEARCHIVE` through its CMake interface,
  preserving runTimeSelection registration for an executable's transitive
  dependency closure without `/FORCE:MULTIPLE` or unrelated duplicate symbols.
  Add optional static plugins to the application's `Make/options` `EXE_LIBS`.
- App target dir comes AFTER library includes so `<CorrectPhi.H>`
  resolves to the library header, not the case-colliding local
  `correctPhi.H` fragment (Windows FS is case-insensitive).
- `FOAM_CONFIGURED_PROJECT_DIR` is baked into libOpenFOAM so
  `#includeEtc`/`etc/caseDicts` resolve without `WM_PROJECT_DIR`.
- `FOAM_STATIC_BUILD` makes `dlOpen` fall back to the process image. Any
  `libs` plugin must be present in the application's static dependency closure.
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

## Run

```bat
call etc\openfoam-env.bat
```

or set `PATH=E:\openfoam\build\bin\Release;E:\openfoam\thirdparty\fftw;%PATH%`.
`WM_PROJECT_DIR` is optional (compile-time fallback baked in).

## Verified runtime

- `blockMesh`, `topoSet`, `setFields`, `checkMesh`, `transformPoints`,
  `decomposePar`/`reconstructPar`, `redistributePar`, `foamToVTK`,
  `foamDictionary`, `foamFormatConvert`, `patchSummary`, `postProcess`
- `laplacianFoam` (flange, cyclicAMI+GAMG), `icoFoam` (cavity 0.5 s),
  `pisoFoam` (RAS cavity 0–10 s, k-ε+FOs), `simpleFoam` (pitzDaily,
  converged, streamlines), `potentialFoam`, `interFoam` (damBreak VOF),
  `snappyHexMesh` (motorBike, 3.8 M cells), `foamyHexMesh` (CGAL blob)

## Caution

- Run ONE MSBuild invocation at a time. Stale `MSBuild.exe` node-reuse
  processes lock `.obj`/`.tlog` files → `C1083 Permission denied`.
  Kill lingering MSBuild/cl before rebuilding.
- Parallelism: `-m:2` is the safe default (32 GB RAM). `-m` alone spawns
  too many concurrent cl.exe (project-level × /MP) → C1060 heap
  exhaustion / pagefile errors.

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
