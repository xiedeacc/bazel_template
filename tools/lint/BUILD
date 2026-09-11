load("@rules_python//python:defs.bzl", "py_binary")
load("@bazel_skylib//rules:native_binary.bzl", "native_binary")

package(default_visibility = ["//visibility:public"])

# @llvm_tools is resolved by //bazel:llvm_tools.bzl from the host machine,
# because toolchains_llvm (which rules_lint's own examples use) has no Windows
# support.
native_binary(
    name = "clang_tidy",
    src = "@llvm_tools//:clang_tidy",
    out = "clang_tidy",
)

# Turns a C++20 module interface into a stub header plus a plain translation
# unit, and rewrites `import` lines in consumers to include those stubs.
py_binary(
    name = "module_rewriter",
    srcs = ["module_rewriter.py"],
    legacy_create_init = 0,
    python_version = "PY3",
    srcs_version = "PY3",
)

# Runs clang-tidy over a rewritten unit and reports findings against the file
# the developer actually edits.
py_binary(
    name = "clang_tidy_driver",
    srcs = ["clang_tidy_driver.py"],
    legacy_create_init = 0,
    python_version = "PY3",
    srcs_version = "PY3",
)
