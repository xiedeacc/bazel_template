"""Lint aspects for this repository.

Replaces the old //bazel:cpplint.bzl macro, which discovered targets by
reflecting over native.existing_rules() and silently stopped generating
anything when that API's return type changed. An aspect reads the real rule
attributes instead, so it cannot go quietly out of date.

The aspect comes from //tools/lint:cpp_modules.bzl rather than from rules_lint
directly: it is rules_lint's clang-tidy aspect for everything that does not use
C++20 named modules, plus a source-level path for everything that does, since
clang-tidy cannot read the module interfaces Bazel builds. See that file.
"""

load(":cpp_modules.bzl", "lint_clang_tidy_aspect")

clang_tidy = lint_clang_tidy_aspect(
    binary = Label("//tools/lint:clang_tidy"),
    global_config = [Label("//:.clang-tidy")],
    # Left off on purpose. This and an explicit header_filter both go through
    # rules_lint's _quoted_arg, whose quotes survive literally on Windows, so
    # clang-tidy reads the pattern as a filename and dies with "no input files".
    # Header coverage comes from HeaderFilterRegex in .clang-tidy instead, which
    # takes the same regex without ever touching a command line.
    lint_target_headers = False,
    angle_includes_are_system = False,
    verbose = False,
    args = [
        # The UCRT defines offsetof with a reinterpret_cast, which clang refuses
        # inside a constant expression, so abseil's static_asserts fail to parse.
        # This switches the macro to __builtin_offsetof, which clang accepts.
        "--extra-arg=-D_CRT_USE_BUILTIN_OFFSETOF",
    ],
)
