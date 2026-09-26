import os, re, sys

QBMM = r"E:/openfoam/modules/OpenQBMM"

# path-prefix -> API macro
MAP = [
    ("applications/solvers/compressible/ButcherTable/", "ButcherTable_API"),
    ("applications/solvers/compressible/explicitRhoFoam/compressibleSystem/", "compressibleSystem_API"),
    ("applications/utilities/generateMoments/momentGenerationModels/", "momentGenerationModels_API"),
    ("src/quadratureMethods/fieldMomentInversion/", "fieldMomentInversion_API"),
    ("src/quadratureMethods/mixingModels/", "mixing_API"),
    ("src/quadratureMethods/momentAdvection/", "momentAdvection_API"),
    ("src/quadratureMethods/momentInversion/", "momentInversion_API"),
    ("src/quadratureMethods/populationBalanceModels/", "populationBalance_API"),
]

def api_for(path):
    rel = os.path.relpath(path, QBMM).replace("\\", "/")
    for pfx, api in MAP:
        if rel.startswith(pfx):
            return api
    return None

MACROS = [
    "TypeName", "TypeNameNoDebug", "ClassName", "ClassNameNoDebug",
    "declareRunTimeSelectionTable", "declareRunTimeNewSelectionTable",
    "declareMemberFunctionSelectionTable", "NamespaceName",
]

changed_files = []
for dp, _, fns in os.walk(QBMM):
    for fn in fns:
        if not fn.endswith(".H"):
            continue
        f = os.path.join(dp, fn)
        api = api_for(f)
        if not api:
            continue
        txt = open(f, encoding="utf-8", errors="replace").read()
        orig = txt
        for m in MACROS:
            # bare MACRO( - "Api" suffix contains word chars so \b fails to
            # match inside TypeNameApi( etc; safe against double-annotation
            pat = re.compile(r"\b" + m + r"\b(\s*)\(")
            txt = pat.sub(lambda mm, api=api, m=m: m + "Api(" + api + ",", txt)
        if txt != orig:
            open(f, "w", encoding="utf-8").write(txt)
            changed_files.append(f)

print("changed:", len(changed_files))
for f in changed_files:
    print(" ", os.path.relpath(f, QBMM))
