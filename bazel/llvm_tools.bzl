"""Locate clang-tidy and clang-format on the host machine.

rules_lint needs real binaries to run. toolchains_llvm, which its examples use,
does not support Windows (bazel-contrib/toolchains_llvm#4), so instead of
downloading a toolchain we reuse whatever the machine already has: the LLVM
tools that ship with Visual Studio Build Tools on Windows, or clang-tidy /
clang-format on PATH everywhere else.

If a tool is missing the repository still builds; the corresponding target just
fails with a readable message when something actually asks for it, so a machine
without clang-tidy can still build and test normally.
"""

# Visual Studio does not put its LLVM tools on PATH, so look in the usual spots.
_WINDOWS_SEARCH_DIRS = [
    "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/Llvm/x64/bin",
    "C:/Program Files/Microsoft Visual Studio/2022/BuildTools/VC/Tools/Llvm/x64/bin",
    "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/x64/bin",
    "C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/Llvm/x64/bin",
    "C:/Program Files/LLVM/bin",
]

_TOOLS = ["clang-tidy", "clang-format"]

def _is_windows(repository_ctx):
    return repository_ctx.os.name.lower().startswith("windows")

def _find_tool(repository_ctx, tool):
    """Return a path to `tool`, or None if the machine does not have it."""
    if _is_windows(repository_ctx):
        exe = tool + ".exe"
        for directory in _WINDOWS_SEARCH_DIRS:
            candidate = repository_ctx.path(directory + "/" + exe)
            if candidate.exists:
                return candidate

    # PATH lookup covers Linux, macOS, and a Windows box with LLVM installed
    # somewhere we did not guess.
    return repository_ctx.which(tool)

def _llvm_tools_impl(repository_ctx):
    build = [
        '"""Host clang-tidy / clang-format, located by //bazel:llvm_tools.bzl."""',
        "",
        'package(default_visibility = ["//visibility:public"])',
        "",
    ]

    for tool in _TOOLS:
        found = _find_tool(repository_ctx, tool)
        name = tool.replace("-", "_")

        if found:
            local_name = tool + (".exe" if _is_windows(repository_ctx) else "")
            repository_ctx.symlink(found, local_name)
            build += [
                'exports_files(["%s"])' % local_name,
                "",
                "alias(",
                '    name = "%s",' % name,
                '    actual = "%s",' % local_name,
                ")",
                "",
            ]
        else:
            # Keep the label resolvable so unrelated builds are unaffected, but
            # fail loudly if a lint action actually reaches for it.
            build += [
                "genrule(",
                '    name = "%s",' % name,
                '    outs = ["%s_missing"],' % name,
                '    cmd = "echo \'%s was not found on this machine; ' % tool +
                "install LLVM or Visual Studio's C++ Clang tools' >&2; exit 1\",",
                "    executable = True,",
                ")",
                "",
            ]

    repository_ctx.file("BUILD.bazel", "\n".join(build))

llvm_tools = repository_rule(
    implementation = _llvm_tools_impl,
    doc = "Exposes the host's clang-tidy and clang-format to rules_lint.",
    local = True,
    configure = True,
)
