"""Rewrite C++20 named-module sources into ordinary translation units for lint.

clang-tidy can only see a named module through a binary module interface (BMI),
and it can only read a BMI that its own clang produced. On this project neither
half is available: Bazel's MSVC toolchain emits `.ifc` files that clang cannot
read at all, and the only LLVM tools the host ships are clang-tidy.exe and
clang-format.exe -- there is no clang driver to precompile a `.pcm` with.

So instead of handing clang-tidy a BMI, this tool hands it source. Two modes:

  * interface mode (`--source X.cppm --stub-out --map-out`): writes a *stub
    header* -- the interface with its module declarations neutralised -- and a
    JSON map naming the module it declares. Consumers of the module include
    the stub where they used to import;

  * tree mode (`--tree-out DIR --copy SRC=REL ...`): writes a rewritten copy of
    every file of a linted target's closure -- its sources, its module
    interfaces and every header of this repository it can reach -- under DIR at
    the file's own repository-relative path. clang-tidy then compiles the copy
    of the source with `-iquote DIR` searched first, so the headers it pulls in
    are the rewritten copies too. Headers matter as much as sources: a header
    may itself say `import bazel_template.x;`, and there is no other way to make that
    line compile without a BMI.

In both modes each `import foo.bar;` becomes an `#include` of that module's
stub header, `export` is blanked out, and module declarations are commented
out (an implementation unit's `module foo;` becomes an include of foo's stub).

Every rewrite is line-preserving and, where it matters, column-preserving: a
declaration never moves, so a diagnostic clang-tidy reports at line 42 of the
rewritten file is at line 42 of the original. clang_tidy_driver.py maps the
generated paths back to the original ones, which is why no `#line` directives
appear here -- clang-tidy ignores them when it prints a diagnostic.

Limitations, all of which fail loudly rather than silently mislint:
  * `import std;` and other module names with no known interface are an error.
  * Header units (`import <vector>;`, `import "foo.h";`) become plain includes,
    which is what they mean textually but not what they mean to the compiler.
  * `export`-ing something whose declaration does not start the line, or a
    module declaration spread over several lines, is not recognised.
"""

import argparse
import json
import os
import re
import sys

# `module;`, `module :private;`
_GLOBAL_FRAGMENT_RE = re.compile(r"^(\s*)module\s*;\s*$")
_PRIVATE_FRAGMENT_RE = re.compile(r"^(\s*)module\s*:\s*private\s*;\s*$")

# A module name: dotted identifiers, optionally followed by a `:partition`.
_NAME = r"[A-Za-z_][A-Za-z0-9_]*(?:\.[A-Za-z_][A-Za-z0-9_]*)*"
_MODULE_NAME = r"(?::)?%s(?::%s)?" % (_NAME, _NAME)

# `export module foo.bar;` / `module foo.bar;` (an implementation unit).
_MODULE_DECL_RE = re.compile(
    r"^(\s*)(export\s+)?module\s+(%s)\s*(\[\[[^\]]*\]\]\s*)?;\s*$" % _MODULE_NAME
)

# `import foo.bar;`, `export import foo.bar;`, `import :part;`
_IMPORT_RE = re.compile(
    r"^(\s*)(export\s+)?import\s+(%s)\s*(\[\[[^\]]*\]\]\s*)?;\s*$" % _MODULE_NAME
)

# `import <vector>;` / `import "foo.h";` -- header units.
_HEADER_IMPORT_RE = re.compile(
    r"^(\s*)(export\s+)?import\s+(<[^>]+>|\"[^\"]+\")\s*;\s*$"
)

# A leading `export` keyword on a declaration.
_EXPORT_KEYWORD_RE = re.compile(r"^(\s*)export\b")


class RewriteError(Exception):
    pass


def _include_of(target_path, from_path):
    """A quoted include of `target_path` written relative to `from_path`.

    Both paths are execroot-relative, and clang resolves a quoted include
    against the directory of the real file doing the including, so a relative
    path here works no matter what -I flags the toolchain passes.
    """
    rel = os.path.relpath(target_path, os.path.dirname(from_path))
    return '#include "%s"' % rel.replace("\\", "/")


def _resolve(name, modules, own_module, source, lineno):
    """Map a module name (possibly a `:partition`) to its stub header."""
    if name.startswith(":"):
        if not own_module:
            raise RewriteError(
                "%s:%d: partition import outside a module unit" % (source, lineno)
            )
        name = own_module.split(":")[0] + name
    stub = modules.get(name)
    if stub is None:
        known = ", ".join(sorted(modules)) or "<none>"
        raise RewriteError(
            "%s:%d: no lint stub for module '%s'. Add the library that declares "
            "it to this target's deps. Known modules: %s" % (source, lineno, name, known)
        )
    return stub


def _rewrite(lines, modules, out_path, source, own_module):
    """Rewrite one file's lines. Returns (new_lines, declared_module_name)."""
    out = []
    declared = own_module
    for index, line in enumerate(lines):
        lineno = index + 1

        if _GLOBAL_FRAGMENT_RE.match(line) or _PRIVATE_FRAGMENT_RE.match(line):
            out.append("//" + line)
            continue

        match = _MODULE_DECL_RE.match(line)
        if match:
            name = match.group(3)
            if match.group(2):  # `export module foo;` -- this file's interface
                declared = name
                out.append("//" + line)
            else:  # `module foo;` -- an implementation unit of `foo`
                declared = name
                out.append(
                    match.group(1) + _include_of(
                        _resolve(name, modules, declared, source, lineno), out_path
                    )
                )
            continue

        match = _HEADER_IMPORT_RE.match(line)
        if match:
            out.append(match.group(1) + "#include " + match.group(3))
            continue

        match = _IMPORT_RE.match(line)
        if match:
            stub = _resolve(match.group(3), modules, declared, source, lineno)
            out.append(match.group(1) + _include_of(stub, out_path))
            continue

        match = _EXPORT_KEYWORD_RE.match(line)
        if match:
            rest = line[match.end():]
            if rest.lstrip().startswith("{"):
                # `export { ... }`: a linkage specification is the only
                # namespace-scope block that leaves names and linkage alone.
                out.append(match.group(1) + 'extern "C++"' + rest)
            else:
                # Blank the keyword out so every column after it is unmoved.
                out.append(match.group(1) + " " * len("export") + rest)
            continue

        out.append(line)
    return out, declared


def _read_lines(path):
    with open(path, "r", encoding="utf-8") as handle:
        lines = handle.read().split("\n")
    if lines and lines[-1] == "":
        lines.pop()
    return lines


def _write(path, text):
    directory = os.path.dirname(path)
    if directory:
        os.makedirs(directory, exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="\n") as handle:
        handle.write(text)


def _rewrite_file(source, out, modules):
    body, _ = _rewrite(_read_lines(source), modules, out, source, None)
    _write(out, "\n".join(body) + "\n")


def _write_stub(source, stub_out, map_out, modules):
    body, declared = _rewrite(_read_lines(source), modules, stub_out, source, None)
    if not declared:
        raise RewriteError("%s: no 'export module <name>;' declaration found" % source)
    # A consumer's TU must not re-report the interface's own findings: the
    # copy of the interface in the consumer's tree is where they are reported.
    _write(stub_out, "#pragma once\n#pragma clang system_header\n" + "\n".join(body) + "\n")
    _write(map_out, json.dumps({"module": declared, "stub": stub_out}))


def main(argv):
    parser = argparse.ArgumentParser(description=__doc__, fromfile_prefix_chars="@")
    parser.add_argument("--source", help="module interface to make a stub of")
    parser.add_argument("--stub-out", help="stub header (interface mode)")
    parser.add_argument("--map-out", help="JSON describing this interface (interface mode)")
    parser.add_argument("--tree-out", help="directory to write rewritten copies under (tree mode)")
    parser.add_argument(
        "--copy",
        action="append",
        default=[],
        help="<file>=<path under --tree-out to write its rewritten copy to>, repeatable",
    )
    parser.add_argument(
        "--module-map",
        action="append",
        default=[],
        help="JSON emitted by --map-out for a module these files may import",
    )
    args = parser.parse_args(argv)

    modules = {}
    for path in args.module_map:
        with open(path, "r", encoding="utf-8") as handle:
            entry = json.load(handle)
        modules[entry["module"]] = entry["stub"]

    try:
        if args.source:
            if not (args.stub_out and args.map_out):
                raise RewriteError("--source needs --stub-out and --map-out")
            _write_stub(args.source, args.stub_out, args.map_out, modules)
        if args.copy and not args.tree_out:
            raise RewriteError("--copy needs --tree-out")
        for item in args.copy:
            source, _, rel = item.partition("=")
            if not rel:
                raise RewriteError("--copy expects <file>=<relative path>, got %r" % item)
            _rewrite_file(source, os.path.join(args.tree_out, rel), modules)
    except RewriteError as error:
        sys.stderr.write("module_rewriter: %s\n" % error)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
