load("@rules_cc//cc:defs.bzl", "cc_toolchain", "cc_toolchain_suite")
load(":clang_linux_x86_64_cc_toolchain_config.bzl", "cc_toolchain_config")
load(":armeabi_linux_x86_64_cc_toolchain_config.bzl", "armeabi_cc_toolchain_config")

package(default_visibility = ["//visibility:public"])

filegroup(name = "empty")

cc_toolchain_suite(
    name = "toolchain",
    toolchains = {
        "k8|gcc": "@local_config_cc//:cc-compiler-k8",
        "k8|clang": ":clang_linux_x86_64_toolchain",
        "k8": ":clang_linux_x86_64_toolchain",
        #"k8": "@local_config_cc//:cc-compiler-k8",
        "armeabi-v7a|compiler": ":cc-compiler-armeabi-v7a",
        "armeabi-v7a": ":cc-compiler-armeabi-v7a",
    },
)

#toolchain(
#name = "clang_toolchain_for_linux_x86_64",
#exec_compatible_with = [
#"@platforms//cpu:x86_64",
#"@platforms//os:linux",
#],
#target_compatible_with = [
#"@platforms//cpu:x86_64",
#"@platforms//os:linux",
#],
#toolchain = ":clang_linux_x86_64_toolchain",
#toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
#)

cc_toolchain(
    name = "clang_linux_x86_64_toolchain",
    all_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
    toolchain_config = ":clang_linux_x86_64_toolchain_config",
    toolchain_identifier = "clang_linux_x86_64_toolchain",
)

cc_toolchain_config(name = "clang_linux_x86_64_toolchain_config")

cc_toolchain(
    name = "cc-compiler-armeabi-v7a",
    all_files = ":empty",
    ar_files = ":empty",
    as_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 1,
    toolchain_config = ":stub_armeabi-v7a",
    toolchain_identifier = "stub_armeabi-v7a",
)

armeabi_cc_toolchain_config(name = "stub_armeabi-v7a")