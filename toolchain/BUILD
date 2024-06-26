load("@rules_cc//cc:defs.bzl", "cc_toolchain")
load("@local_config_cc//:cc_toolchain_config.bzl", "cc_toolchain_config")
#load(":clang_toolchain_config.bzl", "cc_toolchain_config")

package(default_visibility = ["//visibility:public"])

#cc_toolchain_config(name = "clang_toolchain_config")

cc_toolchain_config(
    name = "clang_toolchain_config",
    abi_libc_version = "local",
    abi_version = "local",
    compiler = "clang",
    cpu = "local",
    cxx_builtin_include_directories = [
        "/usr/include",
    ],
    host_system_name = "local",
    target_libc = "local",
    target_system_name = "local",
    tool_paths = {
        "as": "/usr/bin/llvm-as",
        "nm": "/usr/bin/nm",
        "objcopy": "/usr/bin/llvm-objcopy",
        "objdump": "/usr/bin/llvm-objdump",
        "strip": "/usr/bin/llvm-strip",
        "gcov": "/usr/bin/llvm-cov",
        "ld": "/usr/bin/ld.lld",
        "clangxx": "/usr/bin/clang++",
        "ar": "/usr/bin/llvm-ar",
        "cpp": "/usr/bin/clang++",
        "clang": "/usr/bin/clang",
        "gcc": "/usr/bin/clang",
        "llvm-cov": "/usr/bin/llvm-cov",
    },
    toolchain_identifier = "local_clang",
)

# Clang Toolchain
cc_toolchain(
    name = "clang_toolchain",
    all_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
    toolchain_config = ":clang_toolchain_config",
    toolchain_identifier = "local_clang",
)

# GCC Toolchain
#cc_toolchain(
#name = "gcc_toolchain",
#all_files = ":empty",
#compiler_files = ":empty",
#dwp_files = ":empty",
#linker_files = ":empty",
#objcopy_files = ":empty",
#strip_files = ":empty",
#supports_param_files = 0,
#toolchain_config = ":gcc_toolchain_config",
#toolchain_identifier = "local_gcc",
#)

#cc_toolchain(
#name = "clang_armv8_toolchain",
#toolchain_config = ":gcc_toolchain_config",
#toolchain_identifier = "local_clang_armv8",
#)

#cc_toolchain(
#name = "gcc_armv8_toolchain",
#toolchain_config = ":gcc_toolchain_config",
#toolchain_identifier = "local_gcc_armv8",
#)

toolchain(
    name = "clang_toolchain_for_linux_x86_64",
    exec_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    target_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    toolchain = ":clang_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)

toolchain(
    name = "gcc_toolchain_for_linux_x86_64",
    exec_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    target_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    toolchain = ":gcc_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)
