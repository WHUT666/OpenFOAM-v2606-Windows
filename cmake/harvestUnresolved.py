#!/usr/bin/env python3
"""Harvest unresolved symbols (LNK2001/2019/1120) from an MSBuild log.

Groups them per failing target and classifies:
  data   - ?x@Y@@<digit>...   (static members, globals - need dllimport)
  vtable - ??_7...            (need class-level dllimport)
  func   - everything else (template fns etc - missing defs or link deps)

Usage: harvestUnresolved.py <msbuild.log>
Writes unres_{data,func,vt,bytarget}.txt next to the log.
"""

import collections
import os
import re
import sys

txt = open(sys.argv[1], encoding='utf-8', errors='replace').read()
per_target = collections.defaultdict(set)
cur = 'unknown'
for line in txt.splitlines():
    m = re.search(r'build-shared\\src\\(\w+)\.vcxproj', line)
    if m:
        cur = m.group(1)
    if 'error LNK' not in line and 'LNK2' not in line:
        continue
    for q in re.findall(r'"([^"]+)"', line):
        if q.startswith('?'):
            per_target[cur].add(q)
    # mangled names also appear unquoted in parentheses: (??6Foam@@...)
    for q in re.findall(r'\((\?[^()\s]+)\)', line):
        per_target[cur].add(q)

data, func, other = set(), set(), set()
for syms in per_target.values():
    for s in syms:
        if s.startswith('??_7'):
            other.add(s)
        elif s.startswith('??'):
            func.add(s)
        elif re.search(r'@@[0-9_]', s):
            data.add(s)
        else:
            func.add(s)

outdir = os.path.dirname(os.path.abspath(sys.argv[1]))
open(os.path.join(outdir, 'unres_data.txt'), 'w').write('\n'.join(sorted(data)))
open(os.path.join(outdir, 'unres_func.txt'), 'w').write('\n'.join(sorted(func)))
open(os.path.join(outdir, 'unres_vt.txt'), 'w').write('\n'.join(sorted(other)))
open(os.path.join(outdir, 'unres_bytarget.txt'), 'w').write(
    '\n'.join(f'{k}: {len(v)}' for k, v in sorted(per_target.items())))

print('targets with errors:', ', '.join(sorted(per_target.keys())))
print('unique unresolved:', len(data) + len(func) + len(other),
      '| data:', len(data), '| func:', len(func), '| vtable:', len(other))
