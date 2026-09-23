# Submodule port patches

The `plugins/*` submodules track upstream repos we do not own, so the
Windows/MSVC porting fixes are kept as uncommitted working-tree changes
plus these patch snapshots. Reapply after `git submodule update`:

```sh
git -C plugins/avalanche apply ../../patches/plugins/avalanche.patch
git -C plugins/cfmesh apply ../../patches/plugins/cfmesh.patch
git -C plugins/research apply ../../patches/plugins/research.patch
cd plugins/turbulence-community
for d in EllipticBlending HelicitySpalartAllmaras SpalartAllmarasRC \
         dynamicSmagorinsky gammaSST kOmegaSSTPDA libWallModelledLES; do
  git -C "$d" apply "../../../patches/plugins/turbulence-community/$d.patch"
done
```

See AGENTS.md "Modules / plugins" for what each patch fixes.
