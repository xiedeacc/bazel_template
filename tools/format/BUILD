load("@aspect_rules_lint//format:defs.bzl", "format_multirun")

package(default_visibility = ["//visibility:public"])

# `bazel run //tools/format` rewrites files in place;
# `bazel run //tools/format -- --mode=check` only reports.
format_multirun(
    name = "format",
    c = "@llvm_tools//:clang_format",
    cc = "@llvm_tools//:clang_format",
)
