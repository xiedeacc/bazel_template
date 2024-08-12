load("@bazel_skylib//lib:selects.bzl", "selects")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "exported_testdata",
    srcs = glob(["**"]),
)

exports_files(glob(["**"]))

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
    name = "jemalloc",
    define_values = {"profiling_type": "jemalloc"},
)

config_setting(
    name = "tcmalloc",
    define_values = {"profiling_type": "tcmalloc"},
)

config_setting(
    name = "windows_x86_64",
    constraint_values = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    visibility = ["//visibility:public"],
)

config_setting(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
)

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
)

selects.config_setting_group(
    name = "linux_aarch64_clang",
    match_all = [
        ":linux_aarch64",
        ":clang",
    ],
)

platform(
    name = "linux_aarch64_platform",
    constraint_values = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
)

platform(
    name = "linux_x86_64_platform",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
)
