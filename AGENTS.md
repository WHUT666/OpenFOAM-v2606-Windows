# OpenFOAM v2606 — Windows/MSVC port notes

## Build

Toolchain: VS2022 (MSVC 14.43), CMake ≥3.20, x64, C++17, DP/label32, dummy MPI.

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
  lnInclude. If you edit a `src/**` header, re-sync it (content differs
  → copy the file over).
- `build/bin/Release`, `build/lib/Release` — exe/lib output.

## Key conventions

- Executables link all enabled libs with `/WHOLEARCHIVE` +
  `/FORCE:MULTIPLE` to preserve runTimeSelection self-registration
  (upstream shared-lib semantics). Duplicate-registration warnings at
  startup are benign.
- App target dir comes AFTER library includes so `<CorrectPhi.H>`
  resolves to the library header, not the case-colliding local
  `correctPhi.H` fragment (Windows FS is case-insensitive).
- `FOAM_CONFIGURED_PROJECT_DIR` is baked into libOpenFOAM so
  `#includeEtc`/`etc/caseDicts` resolve without `WM_PROJECT_DIR`.
- `FOAM_STATIC_BUILD` makes `dlOpen` fall back to the process image
  (statically-linked "libs" entries succeed silently).
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
  `decomposePar`/`reconstructPar`, `foamToVTK`, `foamDictionary`,
  `foamFormatConvert`, `patchSummary`, `postProcess`
- `laplacianFoam` (flange, cyclicAMI+GAMG), `icoFoam` (cavity 0.5 s),
  `pisoFoam` (RAS cavity 0–10 s, k-ε+FOs), `simpleFoam` (pitzDaily,
  converged, streamlines), `potentialFoam`, `interFoam` (damBreak VOF),
  `snappyHexMesh` (motorBike, 3.8 M cells)

## Caution

- Run ONE MSBuild invocation at a time. Stale `MSBuild.exe` node-reuse
  processes lock `.obj`/`.tlog` files → `C1083 Permission denied`.
  Kill lingering MSBuild/cl before rebuilding.
