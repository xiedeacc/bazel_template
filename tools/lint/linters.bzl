"""Lint aspects for this repository.

Replaces the old //bazel:cpplint.bzl macro, which discovered targets by
reflecting over native.existing_rules() and silently stopped generating
anything when that API's return type changed. An aspect reads the real rule
attributes instead, so it cannot go quietly out of date.
"""

load("@aspect_rules_lint//lint:clang_tidy.bzl", "lint_clang_tidy_aspect")

clang_tidy = lint_clang_tidy_aspect(
    binary = Label("//tools/lint:clang_tidy"),
    global_config = [Label("//:.clang-tidy")],
    # Left off on purpose. Both this and an explicit header_filter go through
    # rules_lint's _quoted_arg, whose quotes survive literally on Windows, so
    # clang-tidy reads the pattern as a filename and dies with "no input files".
    # Without it clang-tidy still reports diagnostics for each linted source,
    # just not for headers pulled in along the way.
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
