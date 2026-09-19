#!/usr/bin/env python3
"""
Repair a Windows checkout of OpenFOAM where the case-insensitive
filesystem collapsed files/dirs that differ only by case
(e.g. Instant.H / instant.H, LduMatrix/ vs lduMatrix/).

For every directory containing case-colliding children:
  1. move it aside,
  2. recreate it empty and enable NTFS per-directory case-sensitivity
     (fsutil only works on EMPTY directories, but then cl.exe and
     normal file I/O honour exact-case lookups),
  3. create the exact-case directory skeleton from the git index,
  4. move every staged file to the index path whose blob content it
     matches (preserving local edits); the case-twin whose content was
     lost during checkout is restored from the git object store,
  5. move untracked files back as-is.

Usage: python cmake/fixCaseCollisions.py [--apply]
Without --apply it only prints what it would do.
"""

import os
import shutil
import subprocess
import sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
APPLY = '--apply' in sys.argv
SCAN_PREFIXES = ('src/', 'applications/', 'modules/', 'plugins/', 'tutorials/')

_blob_cache = {}
_rebuilt = set()


def git(*args):
    return subprocess.run(['git', '-C', ROOT] + list(args), capture_output=True)


def blob(path):
    if path not in _blob_cache:
        _blob_cache[path] = git('show', 'HEAD:' + path).stdout
    return _blob_cache[path]


def set_case_sensitive(dirpath):
    subprocess.run(
        ['fsutil.exe', 'file', 'setCaseSensitiveInfo', dirpath, 'enable'],
        capture_output=True,
    )
    # fsutil exits 0 even when it fails (non-empty dir); verify by creating
    # two probe files that differ only by case.
    try:
        pa = os.path.join(dirpath, '.csprobeA')
        pb = os.path.join(dirpath, '.csprobea')
        with open(pa, 'w') as f:
            f.write('A')
        with open(pb, 'w') as f:
            f.write('a')
        ok = open(pa).read() == 'A' and open(pb).read() == 'a'
        os.remove(pa)
        os.remove(pb)
        return ok
    except OSError:
        return False


def main():
    os.chdir(ROOT)
    paths = [p for p in git('ls-files').stdout.decode('utf-8', 'replace').split()
             if p.startswith(SCAN_PREFIXES)]

    groups = defaultdict(list)
    for p in paths:
        groups[p.lower()].append(p)
    colliding = [sorted(v) for v in groups.values() if len(v) > 1]

    # Directories whose immediate children contain a fold-collision.
    cs_dirs = set()
    for g in colliding:
        for i in range(len(g)):
            for j in range(i + 1, len(g)):
                a, b = g[i].split('/'), g[j].split('/')
                for k in range(min(len(a), len(b))):
                    if a[k] != b[k]:
                        cs_dirs.add('/'.join(a[:k]))
                        break

    print('colliding path groups:', len(colliding))
    for g in colliding:
        for p in g:
            print('   ', p)
    print('directories needing case-sensitivity:', len(cs_dirs))
    for d in sorted(cs_dirs):
        print('   ', d + '/')
    if not APPLY:
        print('\n(dry-run; re-run with --apply)')
        return

    index_set = set(paths)
    for d in sorted(cs_dirs, key=lambda s: s.count('/')):
        repair_dir(d, index_set, cs_dirs)

    bad = []
    for g in colliding:
        for p in g:
            if not os.path.isfile(p):
                bad.append(('missing', p))
            elif open(p, 'rb').read() != blob(p):
                bad.append(('content-mismatch', p))
    if bad:
        print('UNRESOLVED:')
        for why, p in bad:
            print('   ', why, p)
        sys.exit(1)
    print('all colliding paths restored with exact-case contents')


def repair_dir(d, index_set, cs_dirs):
    if d in _rebuilt or not os.path.isdir(d):
        return
    _rebuilt.add(d)
    # Skip directories already case-sensitive (idempotent re-runs).
    try:
        pa, pb = d + '/.csprobeA', d + '/.csprobea'
        open(pa, 'w').write('A')
        open(pb, 'w').write('a')
        if open(pa).read() == 'A' and open(pb).read() == 'a':
            os.remove(pa)
            os.remove(pb)
            return
        os.remove(pa)
        os.remove(pb)
    except OSError:
        pass

    staging = d + '__casefix__'
    prefix = d + '/'
    sub_paths = [p for p in index_set if p.startswith(prefix)]
    if not sub_paths:
        return

    print('rebuilding', d)
    if os.path.isdir(staging):
        sys.exit('staging dir already exists: ' + staging)
    os.rename(d, staging)
    os.mkdir(d)
    if not set_case_sensitive(os.path.abspath(d)):
        print('WARNING: case-sensitivity NOT enabled on', d)

    # 1. exact-case directory skeleton for the whole subtree
    made = set()
    for p in sub_paths:
        comps = p.split('/')[:-1]
        cur = ''
        for c in comps:
            cur = cur + '/' + c if cur else c
            if cur not in made and not os.path.isdir(cur):
                os.mkdir(cur)
            if cur not in made and cur in cs_dirs and cur not in _rebuilt:
                _rebuilt.add(cur)
                if not set_case_sensitive(os.path.abspath(cur)):
                    print('WARNING: case-sensitivity NOT enabled on', cur)
            made.add(cur)

    # 2. distribute staged files by folded relative path
    rel_groups = defaultdict(list)
    for p in sub_paths:
        rel_groups[p[len(prefix):].lower()].append(p)

    for frel, ps in rel_groups.items():
        sfile = staging + '/' + frel
        if not os.path.isfile(sfile):
            for p in ps:
                write_blob(p)
            continue
        if len(ps) == 1:
            os.rename(sfile, ps[0])
            continue
        content = open(sfile, 'rb').read()
        owner = next((p for p in ps if content == blob(p)), None)
        if owner is None:
            owner = next(
                (p for p in ps
                 if p.split('/')[-1] == os.path.basename(sfile)), ps[0])
            print('   note: local edits kept for', owner)
        os.rename(sfile, owner)
        for p in ps:
            if p != owner:
                write_blob(p)

    # 3. move back anything left (untracked files)
    for root_, _dirs, files_ in os.walk(staging):
        for f in files_:
            src = os.path.join(root_, f)
            dest = os.path.join(
                ROOT, d, os.path.relpath(src, os.path.join(ROOT, staging)))
            os.makedirs(os.path.dirname(dest), exist_ok=True)
            os.rename(src, dest)
    shutil.rmtree(staging)


def write_blob(path):
    d = os.path.dirname(path)
    if d:
        os.makedirs(d, exist_ok=True)
    with open(path, 'wb') as f:
        f.write(blob(path))
    print('   restored', path)


if __name__ == '__main__':
    main()
