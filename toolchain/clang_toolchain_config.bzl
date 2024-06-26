# clang_toolchain_config.bzl

#load("@bazel_tools//tools/cpp:unix_cc_toolchain_config.bzl", "cc_toolchain_config")
load("@local_config_cc//:cc_toolchain_config.bzl", "cc_toolchain_config")

cc_toolchain_config(
    name = "clang_toolchain_config",
    cpu = "local",
    compiler = "clang",
    toolchain_identifier = "local_clang",
    host_system_name = "local",
    target_system_name = "local",
    target_cpu = "local",
    target_libc = "local",
    abi_version = "local",
    abi_libc_version = "local",
    tool_paths = [
        {
            "name": "ar",
            "path": "/usr/bin/llvm-ar",
        },
        {
            "name": "cpp",
            "path": "/usr/bin/clang++",
        },
        {
            "name": "clang",
            "path": "/usr/bin/clang",
        },
        {
            "name": "gcov",
            "path": "/usr/bin/llvm-cov",
        },
        {
            "name": "ld",
            "path": "/usr/bin/ld.lld",
        },
    ],
    cxx_builtin_include_directories = [
        "/usr/include",
    ],
    unfiltered_cxx_flag = [
        "-no-canonical-prefixes",
    ],
)
