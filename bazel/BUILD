load("@bazel_skylib//lib:selects.bzl", "selects")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "exported_testdata",
    srcs = glob(["**"]),
)

py_binary(
    name = "gen_local_config_git",
    srcs = ["gen_local_config_git.py"],
    python_version = "PY3",
    srcs_version = "PY3",
    tags = ["no-remote-exec"],
)

config_setting(
    name = "clang",
    values = {"compiler": "clang"},
)

config_setting(
    name = "gcc",
    values = {"compiler": "gcc"},
)

config_setting(
    name = "msvc",
    values = {"compiler": "msvc-cl"},
)

config_setting(
    name = "debian",
    define_values = {"distro": "debian"},
)

config_setting(
    name = "redhat",
    define_values = {"distro": "redhat"},
)

config_setting(
    name = "windows_x86_64",
    constraint_values = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
)

config_setting(
    name = "windows_aarch64",
    constraint_values = [
        "@platforms//os:windows",
        "@platforms//cpu:aarch64",
    ],
)

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

config_setting(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

config_setting(
    name = "osx_x86_64",
    constraint_values = [
        "@platforms//os:osx",
        "@platforms//cpu:x86_64",
    ],
)

config_setting(
    name = "osx_aarch64",
    constraint_values = [
        "@platforms//os:osx",
        "@platforms//cpu:aarch64",
    ],
)

platform(
    name = "windows_x86_64_platform",
    constraint_values = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
)

platform(
    name = "windows_aarch64_platform",
    constraint_values = [
        "@platforms//os:windows",
        "@platforms//cpu:aarch64",
    ],
)

platform(
    name = "linux_x86_64_platform",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

platform(
    name = "linux_aarch64_platform",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

platform(
    name = "osx_x86_64_platform",
    constraint_values = [
        "@platforms//os:osx",
        "@platforms//cpu:x86_64",
    ],
)

platform(
    name = "osx_aarch64_platform",
    constraint_values = [
        "@platforms//os:osx",
        "@platforms//cpu:aarch64",
    ],
)

config_setting(
    name = "cross_compiling",
    define_values = {"cross_compiling": "true"},
)

config_setting(
    name = "not_cross_compiling",
    define_values = {"cross_compiling": "false"},
)

config_setting(
    name = "gnu-abi",
    define_values = {"abi": "gnu"},
)

config_setting(
    name = "musl-abi",
    define_values = {"abi": "musl"},
)

selects.config_setting_group(
    name = "cross_compiling_for_linux_aarch64",
    match_all = [
        ":cross_compiling",
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

selects.config_setting_group(
    name = "cross_compiling_for_linux_aarch64_musl",
    match_all = [
        ":musl-abi",
        ":cross_compiling",
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

selects.config_setting_group(
    name = "linux_gnu",
    match_all = [
        ":gnu-abi",
        "@platforms//os:linux",
    ],
)

selects.config_setting_group(
    name = "cross_compiling_for_osx_x86_64",
    match_all = [
        ":cross_compiling",
        "@platforms//os:osx",
        "@platforms//cpu:x86_64",
    ],
)

selects.config_setting_group(
    name = "cross_compiling_for_osx_aarch64",
    match_all = [
        ":cross_compiling",
        "@platforms//os:osx",
        "@platforms//cpu:aarch64",
    ],
)

selects.config_setting_group(
    name = "not_cross_compiling_on_osx",
    match_all = [
        ":not_cross_compiling",
        "@platforms//os:osx",
    ],
)

config_setting(
    name = "jemalloc",
    define_values = {"profiling_type": "jemalloc"},
)

config_setting(
    name = "tcmalloc",
    define_values = {"profiling_type": "tcmalloc"},
)
