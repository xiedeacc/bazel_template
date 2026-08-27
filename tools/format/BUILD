load("@aspect_rules_lint//format:defs.bzl", "format_multirun")

package(default_visibility = ["//visibility:public"])

# Only cc is declared. rules_lint routes .h files to both the C and the C++
# formatter, and the C++ one already covers every .cc and .h in the repo, so
# declaring c as well would just format the headers twice. There is no
# first-party .c file here; add `c = ...` back if that changes.
format_multirun(
    name = "format",
    cc = "@llvm_tools//:clang_format",
)

# format_multirun's own aggregate goes through rules_multirun, which cannot
# find its runfiles on Windows. These point straight at the per-language
# target, which does not, and give it a name worth typing.
alias(
    name = "fix",
    actual = "format_C++_with_clang-format",
)

alias(
    name = "check",
    actual = "format_C++_with_clang-format.check",
)
