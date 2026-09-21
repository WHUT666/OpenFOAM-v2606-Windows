#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
wmake2cmake.py - Convert OpenFOAM wmake build files to CMake fragments.

Reads <dir>/Make/files and <dir>/Make/options (a restricted subset of GNU make
syntax) and emits a .cmake file describing the target:

    FOAM_SOURCES        - absolute paths of source files
    FOAM_TARGET_NAME    - library/executable name (without lib prefix)
    FOAM_TARGET_TYPE    - LIB | EXE | LIBO
    FOAM_TARGET_OUTDIR  - FOAM_LIBBIN | FOAM_APPBIN | ...
    FOAM_INCLUDES       - include directories (absolute, or @TOKEN@ markers)
    FOAM_DEFINES        - -D compile definitions
    FOAM_CFLAGS         - other -X compile flags (rare)
    FOAM_LIBS           - library link names (-lfoo -> foo, @TOKEN@ paths)
    FOAM_LIBDIRS        - -L library directories
    FOAM_FLEX_SOURCES   - .L sources needing flex (skipped if no flex)

Usage:
    wmake2cmake.py --dir <srcDir> --out <file.cmake> [--vars K=V ...]

The script also flattens headers into a per-library "lnInclude" directory
when invoked with --lninclude <outDir>/<incName>.
"""

import argparse
import os
import re
import sys
import shutil

# ---------------------------------------------------------------------------
# Minimal GNU-make flavour evaluator
# ---------------------------------------------------------------------------

class MakeEval:
    """Evaluate the restricted make syntax used in Make/files and
    Make/options: variable assignment, $(var) expansion, ifeq/ifneq/ifdef,
    and a few $(func ...) expansions."""

    def __init__(self, variables=None):
        self.vars = dict(variables or {})

    # -- helpers ----------------------------------------------------------

    @staticmethod
    def _split_args(s):
        """Split make function arguments on commas at top paren level."""
        args, depth, cur = [], 0, ''
        for ch in s:
            if ch in '($':
                depth += 1
            elif ch in ')':
                depth -= 1
            if ch == ',' and depth == 0:
                args.append(cur)
                cur = ''
            else:
                cur += ch
        args.append(cur)
        return args

    def expand(self, text, depth=0):
        """Expand $(...) and ${...} references."""
        if depth > 20:
            return text
        out = []
        i = 0
        n = len(text)
        while i < n:
            ch = text[i]
            if ch == '$' and i + 1 < n and text[i + 1] in '({':
                close = ')' if text[i + 1] == '(' else '}'
                j = i + 2
                d = 1
                while j < n and d:
                    if text[j] in '({':
                        d += 1
                    elif text[j] in ')}':
                        if text[j] == close:
                            d -= 1
                        elif d == 0:
                            break
                    j += 1
                inner = text[i + 2:j - 1] if j <= n else text[i + 2:]
                out.append(self._expand_ref(inner, depth))
                i = j
            elif ch == '$' and i + 1 < n and text[i + 1] == '$':
                out.append('$')
                i += 2
            elif ch == '$' and i + 1 < n and text[i + 1].isalnum():
                # $(x) short form: $x
                out.append(self.vars.get(text[i + 1], ''))
                i += 2
            else:
                out.append(ch)
                i += 1
        return ''.join(out)

    def _expand_ref(self, inner, depth):
        # Function calls: $(func args...)
        m = re.match(r'([a-zA-Z][a-zA-Z0-9_-]*)\s+(.*)$', inner, re.S)
        if m and m.group(1) in (
            'findstring', 'strip', 'notdir', 'basename', 'dir', 'subst',
            'patsubst', 'filter', 'filter-out', 'addprefix', 'addsuffix',
            'firstword', 'word', 'words', 'if', 'shell', 'wildcard',
            'foreach', 'realpath', 'abspath', 'suffix', 'join',
        ):
            func = m.group(1)
            argstr = m.group(2)
            args = [self.expand(a, depth + 1) for a in self._split_args(argstr)]
            return self._call(func, args, depth)
        # Substitution reference $(var:pat=rep)
        m = re.match(r'([a-zA-Z_][a-zA-Z0-9_]*)(?::(.*))?$', inner, re.S)
        if m:
            name = m.group(1)
            val = self.vars.get(name, '')
            # Make variables are lazily expanded - re-expand nested refs
            val = self.expand(val, depth + 1)
            if m.group(2):
                val = self._subst_ref(val, m.group(2))
            return val
        return self.expand(self.vars.get(inner.strip(), ''), depth + 1)

    def _subst_ref(self, val, spec):
        # a=b pattern substitution (with %)
        if '=' in spec:
            pat, rep = spec.split('=', 1)
            pat, rep = self.expand(pat), self.expand(rep)
            if '%' in pat:
                rx = re.escape(pat).replace('%', '(.*)')
                return re.sub('^' + rx + '$', lambda mm: rep.replace('%', mm.group(1)), val)
            return val.replace(pat, rep)
        return val

    def _call(self, func, args, depth):
        a = args + [''] * 4
        if func == 'findstring':
            return a[1] if a[0] in a[1] else ''
        if func == 'strip':
            return ' '.join(a[0].split())
        if func == 'notdir':
            return ' '.join(w.split('/')[-1] for w in a[0].split())
        if func == 'basename':
            return ' '.join(os.path.splitext(w)[0] for w in a[0].split())
        if func == 'dir':
            return ' '.join((w.rsplit('/', 1)[0] + '/') if '/' in w else './'
                            for w in a[0].split())
        if func == 'subst':
            return a[2].replace(a[0], a[1])
        if func == 'patsubst':
            pat = re.escape(a[0]).replace('%', '(.*)')
            rx = re.compile('^' + pat + '$')
            def rep(w):
                mm = rx.match(w)
                if not mm:
                    return w
                return a[1].replace('%', mm.group(1) if mm.groups() else '')
            return ' '.join(rep(w) for w in a[2].split())
        if func == 'filter':
            pats = a[0].split()
            return ' '.join(w for w in a[1].split()
                            if any(self._pat_match(p, w) for p in pats))
        if func == 'filter-out':
            pats = a[0].split()
            return ' '.join(w for w in a[1].split()
                            if not any(self._pat_match(p, w) for p in pats))
        if func == 'addprefix':
            return ' '.join(a[0] + w for w in a[1].split())
        if func == 'addsuffix':
            return ' '.join(w + a[0] for w in a[1].split())
        if func == 'join':
            l1, l2 = a[0].split(), a[1].split()
            n = max(len(l1), len(l2))
            l1 += [''] * (n - len(l1))
            l2 += [''] * (n - len(l2))
            return ' '.join(x + y for x, y in zip(l1, l2))
        if func == 'firstword':
            w = a[0].split()
            return w[0] if w else ''
        if func == 'word':
            try:
                return a[1].split()[int(a[0]) - 1]
            except (ValueError, IndexError):
                return ''
        if func == 'words':
            return str(len(a[0].split()))
        if func == 'if':
            return a[1] if a[0].strip() else a[2]
        if func in ('wildcard', 'realpath', 'abspath'):
            return a[0]
        if func == 'suffix':
            return ' '.join(os.path.splitext(w)[1] for w in a[0].split()
                            if os.path.splitext(w)[1])
        if func == 'foreach':
            # $(foreach v,list,text)
            var, lst, body = a[0], a[1].split(), a[2]
            old = self.vars.get(var)
            res = []
            for w in lst:
                self.vars[var] = w
                res.append(self.expand(body, depth + 1))
            if old is None:
                self.vars.pop(var, None)
            else:
                self.vars[var] = old
            return ' '.join(res)
        if func == 'shell':
            return ''  # never execute shell during conversion
        return ''

    @staticmethod
    def _pat_match(pat, word):
        rx = re.escape(pat).replace('%', '.*')
        return re.match('^' + rx + '$', word) is not None


# ---------------------------------------------------------------------------
# Preprocessing
# ---------------------------------------------------------------------------

_COMMENT_RE = re.compile(r'/\*.*?\*/', re.S)


def _logical_lines(text):
    """Join backslash-continued lines; strip /* */ and # comments.
    Returns list of (lineno, text)."""
    text = _COMMENT_RE.sub('', text)
    lines = []
    buf = ''
    start = 0
    for idx, raw in enumerate(text.splitlines(), 1):
        line = raw
        # '#' starts a comment only when not inside a directive we handle
        stripped = line.strip()
        if not stripped.startswith('#if') and not stripped.startswith('#el') \
                and not stripped.startswith('#endif') \
                and not stripped.startswith('#define') \
                and not stripped.startswith('#undef') \
                and not stripped.startswith('#include'):
            line = line.split('#', 1)[0] if '#' in line else line
        line = line.rstrip()
        if not buf:
            start = idx
        if line.endswith('\\'):
            buf += line[:-1] + ' '
            continue
        buf += line
        lines.append((start, buf))
        buf = ''
    if buf:
        lines.append((start, buf))
    return lines


_ASSIGN_RE = re.compile(
    r'^\s*([A-Za-z_][A-Za-z0-9_]*)\s*(:=|\?=|\+=|=)\s*(.*)$')


def _emit_lines(lines, ev):
    """Run lines through make-style conditionals + cpp #if guards.
    Returns list of expanded text lines."""
    out = []
    # stack of (parent_active, this_active, in_else)
    cond = []
    active = True

    def is_active():
        return all(c[1] for c in cond)

    for lineno, raw in lines:
        line = raw.strip()
        if not line:
            continue

        # --- preprocessor directives (rare, cpp runs over Make/files) ---
        if line.startswith('#ifdef'):
            name = line.split(None, 1)[1].strip()
            cond.append((active, active and name in ev.vars, False))
            active = is_active()
            continue
        if line.startswith('#ifndef'):
            name = line.split(None, 1)[1].strip()
            cond.append((active, active and name not in ev.vars, False))
            active = is_active()
            continue
        if line.startswith('#else') or line == '#else':
            if cond:
                p, cur, _ = cond.pop()
                cond.append((p, p and not cur, True))
            continue
        if line.startswith('#endif'):
            if cond:
                cond.pop()
            continue

        # --- make conditionals ---
        m = re.match(r'ifeq\s*\((.*)\)\s*$', line)
        if m:
            a = ev._split_args(m.group(1))
            lhs = ev.expand(a[0]).strip()
            rhs = ev.expand(a[1]).strip() if len(a) > 1 else ''
            cond.append((active, active and lhs == rhs, False))
            active = is_active()
            continue
        m = re.match(r'ifneq\s*\((.*)\)\s*$', line)
        if m:
            a = ev._split_args(m.group(1))
            lhs = ev.expand(a[0]).strip()
            rhs = ev.expand(a[1]).strip() if len(a) > 1 else ''
            cond.append((active, active and lhs != rhs, False))
            active = is_active()
            continue
        m = re.match(r'ifdef\s+([A-Za-z_][A-Za-z0-9_]*)', line)
        if m:
            name = m.group(1)
            cond.append((active, active and bool(ev.vars.get(name)), False))
            active = is_active()
            continue
        m = re.match(r'ifndef\s+([A-Za-z_][A-Za-z0-9_]*)', line)
        if m:
            name = m.group(1)
            cond.append((active, active and not ev.vars.get(name), False))
            active = is_active()
            continue
        if re.match(r'else\b', line):
            if cond:
                p, cur, _ = cond.pop()
                cond.append((p, p and not cur, True))
            continue
        if re.match(r'endif\b', line):
            if cond:
                cond.pop()
            continue
        if line.startswith('include ') or line.startswith('-include ') \
                or line.startswith('sinclude '):
            # includes reference wmake rule files - nothing useful inside
            continue
        if line.startswith(('define ', 'endef', 'export ', 'unexport ',
                            'override ', 'vpath', '.PHONY')):
            continue

        if not is_active():
            continue

        # --- variable assignment ---
        m = _ASSIGN_RE.match(line)
        if m:
            name, op, val = m.group(1), m.group(2), m.group(3)
            val = val.strip()
            if op == '=':
                ev.vars[name] = val          # recursive: expand on use
            elif op == ':=':
                ev.vars[name] = ev.expand(val)
            elif op == '+=':
                prev = ev.vars.get(name, '')
                ev.vars[name] = (prev + ' ' + val).strip() if prev else val
            elif op == '?=':
                if name not in ev.vars or not ev.vars[name]:
                    ev.vars[name] = val
            continue

        out.append(line)

    return out


# ---------------------------------------------------------------------------
# Parse Make/files and Make/options
# ---------------------------------------------------------------------------

_SOURCE_EXTS = ('.C', '.c', '.cc', '.cpp', '.cxx', '.Cver')
_FLEX_EXTS = ('.L',)
_LEMON_EXTS = ('.lyy-m4', '.lyy', '.ly')


def parse_files(path, ev):
    """Return (sources, flex_sources, lemon_sources, lib_name, exe_name, outdir_marker)."""
    with open(path, 'r', encoding='utf-8', errors='replace') as f:
        lines = _logical_lines(f.read())

    emitted = _emit_lines(lines, ev)
    sources, flex, lemon = [], [], []
    lib = exe = None

    for line in emitted:
        # LIB = path/libName   (handled as assignment too, but re-check
        # because assignment parser stored it; extract targets here)
        for tok in line.split():
            tok = ev.expand(tok).strip()
            if not tok:
                continue
            ext = os.path.splitext(tok)[1]
            if ext in _FLEX_EXTS:
                flex.append(tok)
            elif tok.endswith('.lyy-m4') or ext in _LEMON_EXTS:
                lemon.append(tok)
            elif ext in _SOURCE_EXTS:
                sources.append(tok)
            elif ext == '.o':
                # object-file link (e.g. libOSspecific.o) - record as lib
                pass

    libv = ev.vars.get('LIB', '')
    if libv:
        libv = ev.expand(libv)
        m = re.search(r'(FOAM_LIBBIN|FOAM_USER_LIBBIN|FOAM_MPI_LIBBIN|'
                      r'FOAM_LIBBIN_DUMMY)/?.*lib([A-Za-z0-9_+.-]+)', libv)
        if m:
            lib = m.group(2)
            outdir_marker = m.group(1)
        else:
            lib = os.path.basename(libv)
            if lib.startswith('lib'):
                lib = lib[3:]
            outdir_marker = 'FOAM_LIBBIN'
    else:
        outdir_marker = 'FOAM_APPBIN'

    exev = ev.vars.get('EXE', ev.vars.get('SEXE', ''))
    if exev:
        exev = ev.expand(exev)
        exe = os.path.basename(exev)

    return sources, flex, lemon, lib, exe, outdir_marker


_FLAG_SWITCHES = {
    'OPENFOAM', 'WM_DP', 'WM_SP', 'WM_SPDP', 'WM_LABEL_SIZE',
    'FOAM_USE_WINDOWS_PSAPI', 'FOAM_COMPILE_STRICT',
}


def parse_options(path, ev):
    """Parse Make/options -> (includes, defines, cflags, libs, libdirs)."""
    if not os.path.isfile(path):
        return [], [], [], [], []

    with open(path, 'r', encoding='utf-8', errors='replace') as f:
        lines = _logical_lines(f.read())

    # Evaluate assignments and conditionals first
    _emit_lines(lines, ev)

    includes, defines, cflags, libs, libdirs = [], [], [], [], []

    def consume(flagstr, inc, dfn, cfl):
        for tok in ev.expand(flagstr).split():
            if tok.startswith('-I'):
                inc.append(tok[2:])
            elif tok.startswith('-D'):
                # Make-level \" quoting -> plain ": the cmake writer
                # re-escapes " -> \" (double-escaping breaks MSVC)
                dfn.append(tok[2:].replace('\\"', '"'))
            elif tok.startswith('-U'):
                dfn.append('__UNDEF__' + tok[2:])
            elif tok.startswith('-L'):
                libdirs.append(tok[2:])
            elif tok.startswith('-l'):
                libs.append(tok[2:])
            elif tok.startswith('-isystem'):
                pass
            elif tok == '-pthread':
                pass
            elif tok.startswith('-'):
                cfl.append(tok)
            elif tok.endswith('.o') or tok.endswith('.obj'):
                libs.append(tok)          # explicit object/archive path
            elif tok:
                libs.append(tok)

    consume(ev.vars.get('EXE_INC', ''), includes, defines, cflags)
    # PFLAGS/PINC land inside EXE_INC already via expansion

    for var in ('LIB_LIBS', 'EXE_LIBS', 'LIBO_LIBS', 'PROJECT_LIBS'):
        consume(ev.vars.get(var, ''), includes, defines, cflags)

    return includes, defines, cflags, libs, libdirs


# ---------------------------------------------------------------------------
# lnInclude generation
# ---------------------------------------------------------------------------

_LNINCLUDE_EXTS = ('.C', '.H', '.h', '.hh', '.tcc', '.hpp', '.tpp',
                   '.hxx', '.txx')
_LNINCLUDE_SKIP = {'lnInclude', 'Make', 'config', 'noLink'}

# Names that collide with standard C/C++ headers on case-insensitive
# filesystems (Windows NTFS): 'string.H' matches <string.h> etc.
_STDLIB_HEADER_NAMES = {
    'assert.h', 'ctype.h', 'errno.h', 'fenv.h', 'float.h', 'inttypes.h',
    'iso646.h', 'limits.h', 'locale.h', 'math.h', 'setjmp.h', 'signal.h',
    'stdarg.h', 'stdatomic.h', 'stdbool.h', 'stddef.h', 'stdint.h',
    'stdio.h', 'stdlib.h', 'string.h', 'tgmath.h', 'threads.h', 'time.h',
    'uchar.h', 'wchar.h', 'wctype.h', 'malloc.h', 'memory.h', 'io.h',
    'direct.h', 'process.h', 'conio.h', 'dos.h', 'fcntl.h', 'share.h',
    'sys/stat.h', 'search.h', 'mbstring.h', 'tchar.h', 'corecrt.h',
    'algorithm', 'array', 'atomic', 'bit', 'bitset', 'cassert', 'cctype',
    'cerrno', 'cfenv', 'cfloat', 'charconv', 'chrono', 'cinttypes',
    'climits', 'clocale', 'cmath', 'codecvt', 'compare', 'complex',
    'concepts', 'condition_variable', 'coroutine', 'csetjmp', 'csignal',
    'cstdarg', 'cstddef', 'cstdint', 'cstdio', 'cstdlib', 'cstring',
    'ctime', 'cuchar', 'cwchar', 'cwctype', 'deque', 'exception',
    'execution', 'filesystem', 'forward_list', 'fstream', 'functional',
    'future', 'initializer_list', 'iomanip', 'ios', 'iosfwd', 'iostream',
    'istream', 'iterator', 'latch', 'limits', 'list', 'locale', 'map',
    'memory', 'memory_resource', 'mutex', 'new', 'numbers', 'numeric',
    'optional', 'ostream', 'queue', 'random', 'ranges', 'ratio', 'regex',
    'scoped_allocator', 'semaphore', 'set', 'shared_mutex',
    'source_location', 'span', 'sstream', 'stack', 'stdexcept',
    'stop_token', 'streambuf', 'string', 'string_view', 'syncstream',
    'system_error', 'thread', 'tuple', 'typeindex', 'typeinfo',
    'type_traits', 'unordered_map', 'unordered_set', 'utility',
    'valarray', 'variant', 'vector', 'version', 'format', 'print',
    'expected', 'stacktrace', 'flat_map', 'flat_set', 'generator',
    'mdspan', 'spanstream',
}


def _collect_system_header_names(dirs):
    """Lowercase basenames of all files in the given system include dirs."""
    names = set()
    for d in dirs:
        if not d or not os.path.isdir(d):
            continue
        for fn in os.listdir(d):
            if os.path.isfile(os.path.join(d, fn)):
                names.add(fn.lower())
    return names


def _is_case_sensitive(dirpath):
    """Functional probe: create two files differing only by case and
    check they coexist. Works regardless of locale/output language."""
    try:
        pa = os.path.join(dirpath, '.foamCsProbeA')
        pb = os.path.join(dirpath, '.foamCsProbea')
        with open(pa, 'w') as fh:
            fh.write('A')
        with open(pb, 'w') as fh:
            fh.write('a')
        ok = (os.path.exists(pa) and os.path.exists(pb)
              and open(pa).read() == 'A' and open(pb).read() == 'a')
        for p in (pa, pb):
            try:
                os.remove(p)
            except OSError:
                pass
        return ok
    except OSError:
        return False


def _set_case_sensitive(dirpath):
    """Try to enable per-directory case sensitivity (Windows 10 1803+).
    Returns True on success. This makes 'string.H' and 'string.h'
    distinct files inside the directory, which is the cleanest fix
    for Foam-header vs C-header basename collisions.

    NOTE: fsutil only succeeds on EMPTY directories."""
    if os.name != 'nt':
        return True
    import subprocess
    try:
        subprocess.run(
            ['fsutil.exe', 'file', 'setCaseSensitiveInfo', dirpath,
             'enable'],
            capture_output=True, timeout=30)
    except Exception:
        pass
    return _is_case_sensitive(dirpath)


def make_lninclude(srcdir, outdir, sys_includes=()):
    """Copy header/template files into <outdir> (flat), mirroring
    wmakeLnInclude.

    Foam headers like string.H/Time.H collide with C headers on
    case-insensitive filesystems. Prefer making the output directory
    case-sensitive (cleanest: <string.h> then cannot match string.H).
    If that fails, emit merged shim headers that pull in BOTH the real
    system header and the Foam header via absolute paths.

    Returns (count, mode) with mode in {'case-sensitive','merged'}."""
    # fsutil can only enable case-sensitivity on an EMPTY directory.
    # lnInclude output is fully regenerated each configure: always wipe
    # so stale files (e.g. pre-rename copies) can never poison lookups.
    if os.path.isdir(outdir):
        shutil.rmtree(outdir)
    os.makedirs(outdir, exist_ok=True)
    case_ok = _set_case_sensitive(outdir)

    sysdirs = [d for d in sys_includes if d]
    sysnames = _collect_system_header_names(sysdirs)
    seen = {}
    collisions = []
    count = 0
    for root, dirs, files in os.walk(srcdir):
        dirs[:] = [d for d in dirs if d not in _LNINCLUDE_SKIP]
        for fn in files:
            ext = os.path.splitext(fn)[1]
            if ext not in _LNINCLUDE_EXTS:
                continue
            src = os.path.join(root, fn)
            dst = os.path.join(outdir, fn)
            try:
                low = fn.lower()
                if low in seen and seen[low] != src:
                    collisions.append((seen[low], src))
                else:
                    seen[low] = src
                if not case_ok and low in sysnames:
                    real = None
                    for sd in sysdirs:
                        cand = os.path.join(sd, low)
                        if os.path.isfile(cand):
                            real = cand
                            break
                    if real is None:
                        real = low
                    merged = (
                        '/* Merged shim generated by wmake2cmake.py:\n'
                        ' * this basename collides with a system header\n'
                        ' * on case-insensitive filesystems. Pull in the\n'
                        ' * real system header first, then the Foam\n'
                        ' * header. */\n'
                        '#pragma once\n'
                        '#include "%s"\n'
                        '#include "%s"\n'
                        % (os.path.normpath(real).replace('\\', '/'),
                           src.replace('\\', '/'))
                    )
                    prev = None
                    if os.path.exists(dst):
                        with open(dst, 'r', encoding='utf-8',
                                  errors='replace') as fh:
                            prev = fh.read()
                    if prev != merged:
                        with open(dst, 'w', encoding='utf-8',
                                  newline='\n') as fh:
                            fh.write(merged)
                elif not os.path.exists(dst):
                    shutil.copyfile(src, dst)
                elif low in sysnames:
                    # Colliding name: a stale merged shim may exist -
                    # compare content, not just timestamps
                    with open(src, 'rb') as fh:
                        sdata = fh.read()
                    with open(dst, 'rb') as fh:
                        ddata = fh.read()
                    if sdata != ddata:
                        shutil.copyfile(src, dst)
                elif (os.path.getmtime(src) > os.path.getmtime(dst)
                      or os.path.getsize(src) != os.path.getsize(dst)):
                    shutil.copyfile(src, dst)
                count += 1
            except OSError:
                pass
    if collisions and not case_ok:
        sys.stderr.write(
            'wmake2cmake: %d case-colliding file pairs could NOT be '
            'written to %s (case-sensitivity unavailable):\n'
            % (len(collisions), outdir))
        for a, b in collisions[:20]:
            sys.stderr.write('    %s  <->  %s\n' % (a, b))
    return count, ('case-sensitive' if case_ok else 'merged')


# ---------------------------------------------------------------------------
# Emit cmake fragment
# ---------------------------------------------------------------------------

def _norm(base, p):
    """Normalise a path token relative to base dir."""
    p = p.strip().strip('"')
    if not p:
        return p
    # keep @TOKENS@ as-is
    if p.startswith('@'):
        return p
    if not os.path.isabs(p):
        p = os.path.join(base, p)
    return os.path.normpath(p).replace('\\', '/')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--dir', required=True, help='target source dir')
    ap.add_argument('--out', required=True, help='output .cmake file')
    ap.add_argument('--lninclude', help='generate lnInclude into this dir')
    ap.add_argument('--sysinclude', action='append', default=[],
                    help='system include dir (merged-header resolution); '
                         'may be given multiple times')
    ap.add_argument('--var', action='append', default=[],
                    help='extra make vars K=V')
    args = ap.parse_args()

    srcdir = os.path.abspath(args.dir).replace('\\', '/')
    files_path = os.path.join(srcdir, 'Make', 'files')
    options_path = os.path.join(srcdir, 'Make', 'options')

    # Predefined wmake variables
    proj = os.environ.get('FOAM_PROJECT_DIR', '')
    if not proj:
        # walk up until we find src/
        d = srcdir
        while d and d != os.path.dirname(d):
            if os.path.isdir(os.path.join(d, 'src')) and \
               os.path.isdir(os.path.join(d, 'wmake')):
                proj = d
                break
            d = os.path.dirname(d)
    libsrc = os.path.join(proj, 'src').replace('\\', '/')

    predefined = {
        'WM_PROJECT_DIR': proj.replace('\\', '/'),
        'LIB_SRC': libsrc,
        'FOAM_SRC': libsrc,
        'FOAM_LIBBIN': '@FOAM_LIBBIN@',
        'FOAM_USER_LIBBIN': '@FOAM_USER_LIBBIN@',
        'FOAM_MPI_LIBBIN': '@FOAM_MPI_LIBBIN@',
        'FOAM_APPBIN': '@FOAM_APPBIN@',
        'FOAM_USER_APPBIN': '@FOAM_USER_APPBIN@',
        'FOAM_SOLVERS': os.path.join(proj, 'applications/solvers'),
        'FOAM_UTILITIES': os.path.join(proj, 'applications/utilities'),
        'FOAM_MODULES': os.path.join(proj, 'modules'),
        'OBJECTS_DIR': '@OBJECTS_DIR@',
        'GENERAL_RULES': os.path.join(proj, 'wmake/rules/General'),
        'WM_OSTYPE': 'windows',
        'WM_MPLIB': os.environ.get('FOAM_MPLIB', 'dummy'),
        'WM_OPTIONS': '',
        'WM_ARCH': 'windows',
        'WM_COMPILER': os.environ.get('FOAM_COMPILER', 'msvc'),
        'EXT_SO': '.dll',
        'WM_PRECISION_OPTION': os.environ.get('FOAM_PRECISION', 'DP'),
        'PRECISION': os.environ.get('FOAM_PRECISION', 'DP'),
        'SCALAR_SIZE': (
            '32' if os.environ.get('FOAM_PRECISION', 'DP') == 'SP' else '64'
        ),
        'WM_COMPILE_OPTION': os.environ.get('FOAM_COMPILE_OPTION', 'Opt'),
        'WM_LABEL_SIZE': os.environ.get('FOAM_LABEL_SIZE', '32'),
        'WM_COMPILE_CONTROL': os.environ.get('FOAM_COMPILE_CONTROL', ''),
        'WM_PROJECT_VERSION': os.environ.get('FOAM_VERSION', '2606'),
        'WM_VERSION': 'OPENFOAM=2606',
        'OPENFOAM': '2606',
        'AND': '&&',
        'FOAM_LINK_DUMMY_PSTREAM': '',
        'SCOTCH_LIB_NAME': 'scotch',
        'PTSCOTCH_LIB_NAME': 'ptscotch',
        'KAHIP_LIB_NAME': 'kahip',
        'METIS_LIB_NAME': 'metis',
        'MGRIDGEN_LIB_NAME': 'mgrid',
        'ZOLTAN_LIB_NAME': 'zoltan',
        'PFLAGS': '',
        'PINC': '',
        'PLIBS': '',
        'PTSCOTCH_INC_DIR': '@PTSCOTCH_INC_DIR@',
        'PTSCOTCH_LIB_DIR': '@PTSCOTCH_LIB_DIR@',
        'SCOTCH_INC_DIR': '@SCOTCH_INC_DIR@',
        'SCOTCH_LIB_DIR': '@SCOTCH_LIB_DIR@',
        'KAHIP_INC_DIR': '@KAHIP_INC_DIR@',
        'KAHIP_LIB_DIR': '@KAHIP_LIB_DIR@',
        'MGRIDGEN_INC_DIR': '@MGRIDGEN_INC_DIR@',
        'MGRIDGEN_LIB_DIR': '@MGRIDGEN_LIB_DIR@',
        'METIS_INC_DIR': '@METIS_INC_DIR@',
        'METIS_LIB_DIR': '@METIS_LIB_DIR@',
        'ZOLTAN_INC_DIR': '@ZOLTAN_INC_DIR@',
        'ZOLTAN_LIB_DIR': '@ZOLTAN_LIB_DIR@',
        'FFTW_INC_DIR': '@FFTW_INC_DIR@',
        'FFTW_LIB_DIR': '@FFTW_LIB_DIR@',
        'PETSC_ARCH_PATH': '@PETSC_DIR@',
        'CCMIO_INC_DIR': '@CCMIO_INC_DIR@',
        'CCMIO_LIB_DIR': '@CCMIO_LIB_DIR@',
        'GFLAGS': '', 'GINC': '', 'GLIBS': '', 'GLIB_LIBS': '',
        'SYS_INC': '', 'SYS_LIBS': '',
        'COMP_FLAGS': '', 'LINK_FLAGS': '',
        'FOAM_EXTRA_CXXFLAGS': '', 'FOAM_EXTRA_LDFLAGS': '',
        'FOAM_MPI': '@FOAM_MPI@',
        'CGAL_INC_DIR': '@CGAL_INC_DIR@',
        'CGAL_LIB_DIR': '@CGAL_LIB_DIR@',
        'BOOST_INC_DIR': '@BOOST_INC_DIR@',
        'BOOST_LIB_DIR': '@BOOST_LIB_DIR@',
        # wmake $(GENERAL_RULES) helper variables (include lines are skipped,
        # so expand the well-known aggregates inline)
        'BOOST_INCLUDES': '-I@BOOST_INC_DIR@',
        'BOOST_LIBRARIES': '',
        'CGAL_INCLUDES': '-DCGAL_HEADER_ONLY -I@CGAL_INC_DIR@ -I@BOOST_INC_DIR@ -I@GMP_INC_DIR@ -I@MPFR_INC_DIR@',
        'CGAL_LIBRARIES': '-lmpfr -lgmp',
        'FFTW_INCLUDES': '-I@FFTW_INC_DIR@',
        'FFTW_LIBRARIES': '-L@FFTW_LIB_DIR@ -lfftw3',
        'PETSC_INCLUDES': '-I@PETSC_DIR@/include',
        'PETSC_LIBRARIES': '',
        'SCOTCH_INCLUDES': '-I@SCOTCH_INC_DIR@',
        'SCOTCH_LIBRARIES': '-L@SCOTCH_LIB_DIR@ -l$(SCOTCH_LIB_NAME)',
        'PTSCOTCH_INCLUDES': '-I@PTSCOTCH_INC_DIR@',
        'PTSCOTCH_LIBRARIES': '-L@PTSCOTCH_LIB_DIR@ -l$(PTSCOTCH_LIB_NAME)',
        'METIS_INCLUDES': '-I@METIS_INC_DIR@',
        'METIS_LIBRARIES': '-L@METIS_LIB_DIR@ -l$(METIS_LIB_NAME)',
        'KAHIP_INCLUDES': '-I@KAHIP_INC_DIR@',
        'KAHIP_LIBRARIES': '-L@KAHIP_LIB_DIR@ -l$(KAHIP_LIB_NAME)',
        'ZOLTAN_INCLUDES': '-I@ZOLTAN_INC_DIR@',
        'ZOLTAN_LIBRARIES': '-L@ZOLTAN_LIB_DIR@ -l$(ZOLTAN_LIB_NAME)',
    }
    for kv in args.var:
        k, _, v = kv.partition('=')
        predefined[k] = v

    ev = MakeEval(predefined)

    no_makefiles = not os.path.isfile(files_path)
    if no_makefiles:
        sources, flex, lemon, lib, exe, outdir = [], [], [], None, None, None
    else:
        sources, flex, lemon, lib, exe, outdir = parse_files(files_path, ev)

    # Re-evaluate options with a fresh evaluator that shares files vars
    ev2 = MakeEval(predefined)
    includes, defines, cflags, libs, libdirs = parse_options(options_path, ev2)

    # Absolute-ise sources
    abs_sources = []
    missing = []
    for s in sources:
        s = s.replace('\\\\', '/')
        p = s if os.path.isabs(s) else os.path.join(srcdir, s)
        p = os.path.normpath(p).replace('\\', '/')
        if os.path.isfile(p):
            # wmake "pointer" files: a stub whose entire content is one
            # relative path to the real source (eg chtMultiRegionFoam
            # variants). Resolve it to the real file.
            try:
                with open(p, 'r', encoding='utf-8', errors='replace') as fh:
                    content = fh.read().strip()
            except OSError:
                content = ''
            if (content and '\n' not in content
                    and not content.startswith('#')
                    and ('/' in content or '\\' in content)
                    and content.lower().endswith(('.c', '.cxx', '.cc'))):
                q = os.path.normpath(
                    os.path.join(os.path.dirname(p), content)
                ).replace('\\', '/')
                if os.path.isfile(q):
                    p = q
            abs_sources.append(p)
        else:
            missing.append(s)

    abs_flex = []
    for s in flex:
        p = os.path.normpath(os.path.join(srcdir, s)).replace('\\', '/')
        if os.path.isfile(p):
            abs_flex.append(p)

    abs_lemon = []
    for s in lemon:
        p = os.path.normpath(os.path.join(srcdir, s)).replace('\\', '/')
        if os.path.isfile(p):
            abs_lemon.append(p)

    # MPI auto-detection. On Linux wmake's mpi-rules supplies PINC/PLIBS
    # (and mpi.h lives on the system include path for serial builds); on
    # Windows the MSMPI include/lib dirs must come from MPI::MPI_CXX.
    # Map any of these to the 'mpi' lib dependency which FoamMacros
    # resolves to MPI::MPI_CXX:
    #   - Make/options including mpi-rules or referencing PINC/PLIBS/PFLAGS
    #   - sources including mpi.h / openfoam_mpi.H or calling MPI_* APIs
    needs_mpi = False
    try:
        with open(options_path, 'r', encoding='utf-8',
                  errors='replace') as fh:
            needs_mpi = bool(re.search(
                r'mpi-rules|\$\((?:PINC|PLIBS|PFLAGS)\)', fh.read()))
    except OSError:
        pass
    if not needs_mpi:
        mpi_src_re = re.compile(
            r'#\s*include\s*[<"](?:mpi\.h|.*openfoam_mpi\.H)[>"]'
            r'|\bMPI_[A-Za-z]+')
        for p in abs_sources:
            try:
                with open(p, 'r', encoding='utf-8',
                          errors='replace') as fh:
                    if mpi_src_re.search(fh.read()):
                        needs_mpi = True
                        break
            except OSError:
                continue
    if needs_mpi and 'mpi' not in libs:
        libs.append('mpi')

    # wmake resolves relative -I/-L paths against the target's own directory
    incs = [_norm(srcdir, i) for i in includes]
    libdirs_n = [_norm(srcdir, d) for d in libdirs]

    # Generate lnInclude if requested
    ninc = 0
    if args.lninclude:
        sysinc = list(args.sysinclude)
        for e in ('FOAM_UCRT_INCLUDE', 'FOAM_VC_INCLUDE'):
            if os.environ.get(e):
                sysinc.append(os.environ[e])
        ninc, mode = make_lninclude(srcdir, args.lninclude, sysinc)
        if mode == 'merged':
            sys.stderr.write(
                'wmake2cmake: NOTE - %s is not case-sensitive; using\n'
                'merged header shims (enable Developer Mode or run\n'
                '"fsutil file setCaseSensitiveInfo <dir> enable" for the\n'
                'cleaner mechanism)\n' % args.lninclude)

    target_name = lib or exe or os.path.basename(srcdir)
    ttype = 'LIB' if lib else 'EXE'
    if ev.vars.get('LIBO') or 'libo' in os.environ.get('FOAM_TARGET_TYPE', ''):
        ttype = 'LIBO'

    with open(args.out, 'w', encoding='utf-8', newline='\n') as f:
        f.write('# Generated by wmake2cmake.py from %s\n' % srcdir)
        f.write('set(FOAM_TARGET_NAME "%s")\n' % target_name)
        f.write('set(FOAM_TARGET_TYPE "%s")\n' % ttype)
        f.write('set(FOAM_TARGET_OUTDIR "%s")\n' % outdir)
        f.write('set(FOAM_TARGET_DIR "%s")\n' % srcdir)
        f.write('set(FOAM_SOURCES\n')
        for s in abs_sources:
            f.write('    "%s"\n' % s)
        f.write(')\n')
        f.write('set(FOAM_FLEX_SOURCES\n')
        for s in abs_flex:
            f.write('    "%s"\n' % s)
        f.write(')\n')
        f.write('set(FOAM_LEMON_SOURCES\n')
        for s in abs_lemon:
            f.write('    "%s"\n' % s)
        f.write(')\n')
        f.write('set(FOAM_INCLUDES\n')
        for i in incs:
            if i:
                f.write('    "%s"\n' % i)
        f.write(')\n')
        f.write('set(FOAM_DEFINES\n')
        for d in defines:
            if d:
                f.write('    "%s"\n' % d.replace('"', '\\"'))
        f.write(')\n')
        f.write('set(FOAM_CFLAGS\n')
        for c in cflags:
            f.write('    "%s"\n' % c)
        f.write(')\n')
        f.write('set(FOAM_LIBS\n')
        for l in libs:
            if l:
                f.write('    "%s"\n' % l)
        f.write(')\n')
        f.write('set(FOAM_LIBDIRS\n')
        for l in libdirs_n:
            if l:
                f.write('    "%s"\n' % l)
        f.write(')\n')
        if missing:
            f.write('set(FOAM_MISSING_SOURCES\n')
            for s_ in missing:
                f.write('    "%s"\n' % s_)
            f.write(')\n')
        f.write('set(FOAM_LNINCLUDE_COUNT %d)\n' % ninc)


if __name__ == '__main__':
    main()
