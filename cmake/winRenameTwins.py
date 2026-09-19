#!/usr/bin/env python3
"""Rename case-fold twin files for MSVC.

MSVC dedups #include'd files by a case-INSENSITIVE canonical path, so two
headers in the same directory (or flattened into the same lnInclude dir)
that differ only in case can never both be included in one TU: the second
is silently skipped. NTFS per-directory case-sensitivity does NOT help.

Fix: rename one side of every fold-twin pair (capital-CamelCase template
side gets a '_tpl' suffix, matching the earlier LduMatrix_tpl directory
convention), then patch every literal '#include "X"'/'#include <X>' and
Make/files entry that referenced the old basename.

Usage: winRenameTwins.py [--dry-run]
"""
import os, re, sys, collections

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
APPLY = '--apply' in sys.argv

SKIP_DIRS = {'.git', 'build', 'doc', 'tutorials'}
LN_SKIP = {'lnInclude', 'Make', 'config', 'noLink'}
LN_EXTS = ('.C', '.H', '.h', '.hh', '.tcc', '.hpp', '.tpp', '.hxx', '.txx')
INC_RE = re.compile(r'(#\s*include\s*["<])([^">]+)([">])')

# --- collect twin groups ---------------------------------------------------
# group key -> set of exact basenames; 'src' covers lib flattening, same-dir
# covers everything else (applications, tests, etc.)
twin_names = set()          # exact basenames that are one side of a twin pair

# same-directory twins: walk whole tree
for dp, dn, fn in os.walk(ROOT):
    dn[:] = [d for d in dn if d not in SKIP_DIRS]
    g = collections.defaultdict(set)
    for f in fn:
        g[f.lower()].add(f)
    for k, v in g.items():
        if len(v) > 1:
            twin_names.update(v)

# lnInclude-flattened twins: for each src/<Lib> root simulate the flat copy
src_root = os.path.join(ROOT, 'src')
for lib in sorted(os.listdir(src_root)):
    libdir = os.path.join(src_root, lib)
    if not os.path.isdir(libdir):
        continue
    g = collections.defaultdict(set)
    for dp, dn, fn in os.walk(libdir):
        dn[:] = [d for d in dn if d not in LN_SKIP]
        for f in fn:
            if os.path.splitext(f)[1] in LN_EXTS:
                g[f.lower()].add(f)
    for k, v in g.items():
        if len(v) > 1:
            twin_names.update(v)

# --- choose which side to rename -------------------------------------------
def new_name(old):
    stem, ext = os.path.splitext(old)
    return stem + '_tpl' + ext

renames = {}  # old basename -> new basename
for name in sorted(twin_names):
    if name[:1].isupper():
        renames[name] = new_name(name)

# groups where no member starts uppercase (e.g. fvcDDt.H/fvcDdt.H):
# rename the member with the most capital letters
all_groups = []
for dp, dn, fn in os.walk(ROOT):
    dn[:] = [d for d in dn if d not in SKIP_DIRS]
    g = collections.defaultdict(set)
    for f in fn:
        g[f.lower()].add(f)
    for k, v in g.items():
        if len(v) > 1:
            all_groups.append((dp, v))
for lib in sorted(os.listdir(src_root)):
    libdir = os.path.join(src_root, lib)
    if not os.path.isdir(libdir):
        continue
    g = collections.defaultdict(set)
    for dp, dn, fn in os.walk(libdir):
        dn[:] = [d for d in dn if d not in LN_SKIP]
        for f in fn:
            if os.path.splitext(f)[1] in LN_EXTS:
                g[f.lower()].add(f)
    for k, v in g.items():
        if len(v) > 1:
            all_groups.append((libdir, v))
for where, v in all_groups:
    if v & set(renames):
        continue
    pick = max(v, key=lambda s: sum(c.isupper() for c in s))
    renames[pick] = new_name(pick)
    print("NONCAP group", where, sorted(v), "-> rename", pick)

print(f"{len(renames)} files to rename")

# --- apply renames ----------------------------------------------------------
for dp, dn, fn in os.walk(ROOT):
    dn[:] = [d for d in dn if d not in SKIP_DIRS]
    for f in fn:
        if f in renames:
            old = os.path.join(dp, f)
            new = os.path.join(dp, renames[f])
            rel = os.path.relpath(old, ROOT)
            if os.path.exists(new):
                print("  TARGET EXISTS", rel)
                continue
            print("  RENAME", rel, '->', renames[f])
            if APPLY:
                os.rename(old, new)

# --- patch references -------------------------------------------------------
base_re = re.compile(
    r'\b(' + '|'.join(re.escape(k) for k in sorted(renames, key=len, reverse=True)) + r')\b'
) if renames else None

patched_files = 0
patched_lines = 0
if base_re:
    for dp, dn, fn in os.walk(ROOT):
        dn[:] = [d for d in dn if d not in SKIP_DIRS]
        for f in fn:
            fp = os.path.join(dp, f)
            if not f.endswith(('.C', '.H', '.h', '.hh', '.hpp', '.cxx', '.cpp',
                               '.I', '.L', '.Cver', '.cmake', 'files', 'options',
                               '.txt')) and f != 'files':
                continue
            try:
                text = open(fp, encoding='utf-8', errors='strict').read()
            except (UnicodeDecodeError, OSError):
                continue
            out_lines = []
            changed = 0
            for line in text.split('\n'):
                nl = line
                m = INC_RE.search(line)
                if m:
                    tgt = m.group(2)
                    head, sep, base = tgt.rpartition('/')
                    nb = base_re.sub(lambda mm: renames[mm.group(1)], base)
                    if nb != base:
                        nl = line[:m.start(2)] + (head + sep if sep else '') + nb + line[m.end(2):]
                elif os.path.basename(fp) in ('files', 'options'):
                    nl = base_re.sub(lambda mm: renames[mm.group(1)], line)
                if nl != line:
                    changed += 1
                out_lines.append(nl)
            if changed:
                patched_files += 1
                patched_lines += changed
                print(f"  PATCH {os.path.relpath(fp, ROOT)} ({changed})")
                if APPLY:
                    open(fp, 'w', encoding='utf-8').write('\n'.join(out_lines))

print(f"{patched_files} files patched, {patched_lines} lines")
print("APPLIED" if APPLY else "DRY-RUN (pass --apply to apply)")
