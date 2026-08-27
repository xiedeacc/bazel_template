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
