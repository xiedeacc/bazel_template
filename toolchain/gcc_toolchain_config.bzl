# gcc_toolchain_config.bzl

load("@bazel_tools//tools/cpp:unix_toolchain_config_lib.bzl", "cc_toolchain_config")

cc_toolchain_config(
    name = "gcc_toolchain_config",
    cpu = "local",
    compiler = "gcc",
    toolchain_identifier = "local_gcc",
    host_system_name = "local",
    target_system_name = "local",
    target_cpu = "local",
    target_libc = "local",
    abi_version = "local",
    abi_libc_version = "local",
    tool_paths = [
        {
            "name": "ar",
            "path": "/usr/bin/gcc-ar",
        },
        {
            "name": "cpp",
            "path": "/usr/bin/cpp",
        },
        {
            "name": "gcc",
            "path": "/usr/bin/gcc",
        },
        {
            "name": "gcov",
            "path": "/usr/bin/gcov",
        },
        {
            "name": "ld",
            "path": "/usr/bin/ld",
        },
    ],
    cxx_builtin_include_directories = [
        "/usr/include",
        "/usr/lib/gcc/x86_64-linux-gnu/11/include",
    ],
    unfiltered_cxx_flag = [
        "-no-canonical-prefixes",
    ],
)
