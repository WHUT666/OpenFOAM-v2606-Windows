#!/usr/bin/env python3
"""Annotate C++ class/struct declarations with a DLL import/export macro.

Rewrites `class Foo` / `struct Foo` declaration headers (definitions AND
forward declarations) into `class <API> Foo`, so consumers compile with
__declspec(dllimport) while the owning library compiles with dllexport.

Skips:
  - `template<class T>` parameter keywords
  - `enum class Foo`
  - `friend class Foo` (friend decls must not carry dllimport)
  - `class Outer::Inner` nested-class qualified defs (already imported via Outer)
  - sites already annotated with an _API macro

Usage:
    annotateClass.py <root> <ClassName>=<LIB>_API [<Class2>=<LIB2>_API ...]
"""

import os
import re
import sys


def annotate_file(path, classmap):
    with open(path, 'r', encoding='utf-8', errors='replace') as f:
        lines = f.readlines()

    changed = 0
    out = []
    for i, line in enumerate(lines):
        m = re.search(r'\b(class|struct)\s+([A-Za-z_]\w*)\b', line)
        if m and m.group(2) in classmap:
            kw, name = m.group(1), m.group(2)
            before = line[:m.start()]
            after = line[m.end():]
            prev_text = ' '.join(lines[max(0, i-3):i+1])
            # exclusions
            if kw == 'class' and re.search(r'\benum\s+$', before):
                pass  # enum class
            elif re.search(r'\bfriend\s*$', before):
                pass
            elif re.search(r'\b(template|typedef)\b[^;{]*$', before):
                pass  # template<class T> or typedef class X
            elif re.search(r'\w\s*::\s*$', before):
                pass  # qualified nested-class definition
            elif re.match(r'\s*::', after):
                pass  # out-of-line nested-class def: 'class Outer::Inner'
            elif re.search(r'\w+_API\s*$', before):
                pass  # already annotated
            elif kw == 'class' and re.search(r'<\s*$', before):
                pass  # template<..., class X> split across lines
            else:
                line = (before + f'{kw} {classmap[name]} {name}' + after)
                changed += 1
        out.append(line)

    if changed:
        with open(path, 'w', encoding='utf-8', newline='') as f:
            f.writelines(out)
    return changed


def main():
    root = sys.argv[1]
    classmap = {}
    for arg in sys.argv[2:]:
        name, _, api = arg.partition('=')
        if name and api:
            classmap[name] = api
    if not classmap:
        sys.exit('no ClassName=LIB_API pairs given')

    total = 0
    files = 0
    for dirpath, _dirs, fnames in os.walk(root):
        for fn in fnames:
            if not fn.endswith(('.H', '.h', '.hpp', '.C', '.cpp', '.cxx')):
                continue
            p = os.path.join(dirpath, fn)
            n = annotate_file(p, classmap)
            if n:
                print(f'{p}: {n}')
                files += 1
                total += n
    print(f'== {total} annotations in {files} files')


if __name__ == '__main__':
    main()
