"""winCasefix: migrate case-colliding paths so the tree builds under
Windows/MSVC+CMake.

Strategy
--------
- Directory-level fold collisions (LduMatrix/ vs lduMatrix/ ...) ->
  rename the legacy/upper-case directory with a '_tpl' suffix and fix
  Make/files (+options) references.
- Same-directory .C file collisions (compiled sources that CMake must
  stat) -> rename with a '_tpl' suffix and fix the including header.
- Header files keep their names.  Poisoned quoted includes - where the
  includer's own directory contains a file that only differs by case
  (the quoted-include search hits the twin instead of the -I target) -
  are converted to angle-bracket includes, which skip the includer's
  directory and resolve exactly inside the case-sensitive lnInclude
  directories.

The script is idempotent: already-renamed paths are skipped.
"""

import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)
APPLY = '--apply' in sys.argv

# ---------------------------------------------------------------- dirs
DIR_RENAMES = [
    'src/OpenFOAM/matrices/LduMatrix',
    'src/OpenFOAM/meshes/primitiveMesh/PrimitivePatch',
    'src/finiteVolume/finiteVolume/gradSchemes/LeastSquaresGrad',
    'src/phaseSystemModels/multiphaseInter/phasesSystem/'
        'InterfaceCompositionModel',
    'src/phaseSystemModels/reactingEuler/multiphaseSystem/'
        'interfacialCompositionModels/interfaceCompositionModels/'
        'InterfaceCompositionModel',
    'src/thermophysicalModels/chemistryModel/chemistryModel/'
        'BasicChemistryModel',
    'applications/test/Tensor2D',
]

# ------------------------------------------------------- same-dir .C twins
# (path evaluated AFTER dir renames above)
FILE_RENAMES = [
    ('src/OpenFOAM/matrices/LduMatrix_tpl/LduMatrix/SolverPerformance.C',
     'src/OpenFOAM/matrices/LduMatrix_tpl/LduMatrix/SolverPerformance_tpl.C'),
    ('src/finiteVolume/finiteVolume/fvc/fvcDDt.C',
     'src/finiteVolume/finiteVolume/fvc/fvcDDt_tpl.C'),
    ('src/fvOptions/sources/derived/phaseLimitStabilization/'
     'PhaseLimitStabilization.C',
     'src/fvOptions/sources/derived/phaseLimitStabilization/'
     'PhaseLimitStabilization_tpl.C'),
]

# --------------------------------------------------- .C impl includes that
# point at a renamed .C file:  (header, old, new)
CIMPL_PATCHES = [
    ('src/OpenFOAM/matrices/LduMatrix_tpl/LduMatrix/SolverPerformance.H',
     'SolverPerformance.C', 'SolverPerformance_tpl.C'),
    ('src/finiteVolume/finiteVolume/fvc/fvcDDt.H',
     'fvcDDt.C', 'fvcDDt_tpl.C'),
    ('src/fvOptions/sources/derived/phaseLimitStabilization/'
     'PhaseLimitStabilization.H',
     'PhaseLimitStabilization.C', 'PhaseLimitStabilization_tpl.C'),
]

INC_RE = re.compile(r'#\s*include\s*"([^">]+)"')


def norm(p):
    return p.replace('\\', '/')


def rename_dir(src, dst):
    src_n, dst_n = norm(src), norm(dst)
    if not os.path.isdir(src_n):
        if os.path.isdir(dst_n):
            print('dir already renamed: %s' % dst_n)
            return
        print('MISSING DIR %s' % src_n)
        return
    print('dir rename: %s -> %s' % (src_n, dst_n))
    if APPLY:
        os.rename(src_n, dst_n)


def rename_file(src, dst):
    src_n, dst_n = norm(src), norm(dst)
    if not os.path.isfile(src_n):
        if os.path.isfile(dst_n):
            print('file already renamed: %s' % dst_n)
            return
        print('MISSING FILE %s' % src_n)
        return
    print('file rename: %s -> %s' % (src_n, dst_n))
    if APPLY:
        os.rename(src_n, dst_n)


def find_poisoned():
    """Quoted includes whose own directory holds a case-fold twin."""
    src_files = []
    for base in ('src', 'applications'):
        for root, dirs, files in os.walk(base):
            if '__casefix__' in root or 'lnInclude' in root:
                continue
            for fn in files:
                if os.path.splitext(fn)[1] in (
                        '.C', '.H', '.h', '.cxx', '.cpp', '.L', '.Cver'):
                    src_files.append(os.path.join(root, fn))
    dirmap = {}
    for p in src_files:
        dirmap.setdefault(os.path.dirname(p), set()).add(
            os.path.basename(p))
    poisoned = []
    for p in src_files:
        names = dirmap[os.path.dirname(p)]
        try:
            text = open(p, encoding='utf-8', errors='replace').read()
        except OSError:
            continue
        for m in INC_RE.finditer(text):
            inc = m.group(1)
            if '/' in inc:
                continue
            lw = inc.lower()
            if any(n.lower() == lw and n != inc for n in names):
                poisoned.append((norm(p), inc))
    return sorted(set(poisoned))


def patch_angle_includes(pairs):
    """Turn poisoned #include "X" into #include <X>."""
    n_files = n_inc = 0
    byfile = {}
    for p, inc in pairs:
        byfile.setdefault(p, set()).add(inc)
    for p, incs in byfile.items():
        if not os.path.isfile(p):
            print('MISSING SRC %s' % p)
            continue
        text = open(p, encoding='utf-8', errors='replace').read()
        changed = False

        def sub(m, incs=incs):
            nonlocal n_inc, changed
            if m.group(1) in incs:
                n_inc += 1
                changed = True
                return '#include <%s>' % m.group(1)
            return m.group(0)

        new = INC_RE.sub(sub, text)
        if changed:
            n_files += 1
            if APPLY:
                open(p, 'w', encoding='utf-8', newline='').write(new)
    print('angle-include patches: %d includes in %d files'
          % (n_inc, n_files))


def patch_cimpl():
    for p, old, new in CIMPL_PATCHES:
        p = norm(p)
        if not os.path.isfile(p):
            print('MISSING IMPL-HDR %s' % p)
            continue
        text = open(p, encoding='utf-8', errors='replace').read()
        pat = '#include "%s"' % old
        if pat in text:
            print('cimpl patch: %s  "%s" -> "%s"' % (p, old, new))
            if APPLY:
                open(p, 'w', encoding='utf-8', newline='').write(
                    text.replace(pat, '#include "%s"' % new))
        elif '#include "%s"' % new in text:
            print('cimpl already patched: %s' % p)


def patch_make_files():
    """Fix Make/files (+options) path references for renamed dirs/files."""
    renames = []
    for d in DIR_RENAMES:
        base = os.path.basename(norm(d))
        renames.append((base, base + '_tpl'))
    for src, dst in FILE_RENAMES:
        renames.append((os.path.basename(norm(src)),
                        os.path.basename(norm(dst))))
    n = 0
    for root, dirs, files in os.walk('.'):
        if '__casefix__' in root or 'lnInclude' in root:
            continue
        for fn in files:
            if fn not in ('files', 'options'):
                continue
            p = os.path.join(root, fn)
            try:
                text = open(p, encoding='utf-8', errors='replace').read()
            except OSError:
                continue
            new = text
            for old, new_name in renames:
                new = re.sub(r'(?<![\w.])' + re.escape(old) + r'(?![\w.])',
                             new_name, new)
            if new != text:
                n += 1
                print('Make patch: %s' % p)
                if APPLY:
                    open(p, 'w', encoding='utf-8', newline='').write(new)
    print('Make files patched: %d' % n)


def main():
    for d in DIR_RENAMES:
        rename_dir(d, norm(d) + '_tpl')
    for src, dst in FILE_RENAMES:
        rename_file(src, dst)
    patch_cimpl()

    poisoned = find_poisoned()
    # .C impl includes handled above must not also become angle includes
    cimpl = {norm(p) for p, _, _ in CIMPL_PATCHES}
    poisoned = [x for x in poisoned
                if not (x[0] in cimpl and x[1].endswith('.C'))]
    print('poisoned include sites: %d' % len(poisoned))
    for p, inc in poisoned:
        print('  %s  "%s"' % (p, inc))
    patch_angle_includes(poisoned)
    patch_make_files()
    print('done. apply=%s' % APPLY)


if __name__ == '__main__':
    main()
