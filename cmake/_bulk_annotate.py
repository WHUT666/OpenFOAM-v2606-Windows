import os, re, sys

ROOT = "src/OpenFOAM"
API = "OpenFOAM_API"

# collect header-defined member names (out-of-class defs in .H/.I files)
hdr_defs = set()
hdr_files = []
for dp, _, fns in os.walk(ROOT):
    for fn in fns:
        if fn.endswith((".H", ".I")):
            hdr_files.append(os.path.join(dp, fn))

def_re = re.compile(r'::(\w+)\s*(?:[({=<]|=|;|\s*$)')
for f in hdr_files:
    txt = open(f, encoding='utf-8', errors='replace').read()
    for m in def_re.finditer(txt):
        hdr_defs.add(m.group(1))

# member static decl:  static TYPE NAME;  (no paren, no '=', not constexpr/inline/template/typedef)
mem_re = re.compile(r'^(\s*)static\s+((?:(?!constexpr|inline|typename|template|friend|virtual|typedef|using)\S[^();=]*?))\s+(\w+)\s*(\[[^\]]*\])?\s*;\s*(?://.*)?$')
# namespace extern decl: extern TYPE NAME;
ext_re = re.compile(r'^(\s*)extern\s+((?:(?!template|inline)\S[^();=]*?))\s+(\w+)\s*(\[[^\]]*\])?\s*;\s*(?://.*)?$')

annotated = []
skipped_hdr = []
for f in hdr_files:
    if not f.endswith(".H"):
        continue
    lines = open(f, encoding='utf-8', errors='replace').read().split('\n')
    cont = False
    out = []
    changed = False
    for ln in lines:
        prev_cont = cont
        cont = ln.rstrip().endswith('\\')
        if prev_cont or API in ln:
            out.append(ln)
            continue
        m = mem_re.match(ln) or ext_re.match(ln)
        if m:
            name = m.group(3)
            if name in hdr_defs:
                skipped_hdr.append((f, name))
                out.append(ln)
                continue
            kw = 'static' if ln.lstrip().startswith('static') else 'extern'
            out.append(ln.replace(kw, kw + ' ' + API, 1))
            annotated.append((f, name))
            changed = True
            continue
        out.append(ln)
    if changed:
        open(f, 'w', encoding='utf-8').write('\n'.join(out))

print("annotated:", len(annotated))
print("skipped(header-defined):", len(skipped_hdr))
for f, n in skipped_hdr[:30]:
    print("  skip", f, n)
