#!/usr/bin/env python3
"""Generate a filtered exports .def for an OpenFOAM DLL target.

Replacement for CMake's WINDOWS_EXPORT_ALL_SYMBOLS: scans the exact objects
listed by a Visual Studio project with dumpbin, collects defined external
symbols and writes a module-definition file. Import machinery, CRT internals
and compiler-generated deleting-destructor thunks are omitted.
"""

import argparse
import os
import re
import subprocess
import sys
import xml.etree.ElementTree as ET
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

_SYM_RE = re.compile(
    r"^\w+\s+\w+\s+(SECT\w+|ABSOLUTE|UNDEF|DEBUG|STATIC)\s+"
    r"(\w+)\s*(\(\))?\s+(External|Static)\s+\|\s+(\S+)"
)

_DROP_PREFIXES = (
    "__imp_", "_imp_",
    "__IMPORT_DESCRIPTOR", "__NULL_IMPORT_DESCRIPTOR",
    "__dyn_tls", "__tls_used", "__tls_index",
    "__xi_", "__xc_", "__xp_", "__xt_", "__rtc_",
    "??_E", "??_G",
)
_DROP_EXACT = {
    "DllMainCRTStartup", "main", "wmain", "WinMain", "wWinMain",
    "mainCRTStartup", "wmainCRTStartup", "WinMainCRTStartup",
    "wWinMainCRTStartup", "_load_config_used",
    "__iob_func", "_legacy_iob_init",
}


def _expand_msbuild_path(value, project_dir, config, objdir):
    value = value.replace("$(Configuration)", config)
    value = value.replace("$(IntDir)", str(Path(objdir)) + os.sep)
    value = value.replace("$(ProjectDir)", str(Path(project_dir)) + os.sep)
    path = Path(value)
    if not path.is_absolute():
        path = Path(project_dir) / path
    return str(path.resolve())


def _project_objects(project, config, objdir):
    root = ET.parse(project).getroot()
    project_dir = str(Path(project).resolve().parent)
    objects = []

    for node in root.iter():
        tag = node.tag.rsplit("}", 1)[-1]
        include = node.attrib.get("Include")
        if not include:
            continue

        if tag == "Object":
            objects.append(
                _expand_msbuild_path(include, project_dir, config, objdir)
            )
            continue

        if tag != "ClCompile":
            continue

        object_name = None
        for child in node:
            if child.tag.rsplit("}", 1)[-1] != "ObjectFileName":
                continue
            condition = child.attrib.get("Condition", "")
            if not condition or config.lower() in condition.lower():
                object_name = child.text
                if condition:
                    break

        if object_name:
            objects.append(
                _expand_msbuild_path(object_name, project_dir, config, objdir)
            )
        else:
            objects.append(str((Path(objdir) / (Path(include).stem + ".obj")).resolve()))

    return list(dict.fromkeys(objects))


def _scan_obj(dumpbin, objpath):
    try:
        out = subprocess.run(
            [dumpbin, "/NOLOGO", "/SYMBOLS", objpath],
            capture_output=True,
            text=True,
            errors="replace",
            check=False,
        ).stdout
    except OSError:
        return [], []

    code, data = [], []
    for line in out.splitlines():
        match = _SYM_RE.match(line.strip())
        if not match:
            continue
        section, _symbol_type, is_function, linkage, name = match.groups()
        if linkage != "External":
            continue
        if section in ("UNDEF", "ABSOLUTE", "DEBUG", "STATIC"):
            continue
        if name.startswith(_DROP_PREFIXES) or name in _DROP_EXACT:
            continue
        (code if is_function else data).append(name)
    return code, data


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--project")
    parser.add_argument("--config")
    parser.add_argument("--objdir", action="append", default=[])
    parser.add_argument("--dumpbin", required=True)
    parser.add_argument("--out", required=True)
    args = parser.parse_args()

    objects = []
    if args.project:
        if not args.config or not args.objdir:
            parser.error("--project requires --config and --objdir")
        objects = _project_objects(
            args.project, args.config, args.objdir[0]
        )
    else:
        for directory in args.objdir:
            objects.extend(
                str(Path(directory, name).resolve())
                for name in os.listdir(directory)
                if name.lower().endswith(".obj")
            )

    objects = [path for path in objects if os.path.isfile(path)]
    if not objects:
        sys.exit("genExportsDef: no target objects found")

    code, data = set(), set()
    with ThreadPoolExecutor(max_workers=8) as pool:
        for code_symbols, data_symbols in pool.map(
            lambda path: _scan_obj(args.dumpbin, path), objects
        ):
            code.update(code_symbols)
            data.update(data_symbols)
    data -= code

    os.makedirs(os.path.dirname(args.out), exist_ok=True)
    with open(args.out, "w", newline="\n") as stream:
        stream.write("EXPORTS\n")
        for symbol in sorted(code):
            stream.write("\t" + symbol + "\n")
        for symbol in sorted(data):
            stream.write("\t" + symbol + " \t DATA\n")
    print(
        "genExportsDef:", len(code) + len(data), "exports from",
        len(objects), "objects ->", args.out
    )


if __name__ == "__main__":
    main()
