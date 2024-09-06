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
    name = "arch",
    define_values = {"distro": "arch"},
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
    visibility = ["//visibility:public"],
)

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
)

config_setting(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
)

config_setting(
    name = "osx_x86_64",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:osx",
    ],
)

config_setting(
    name = "osx_aarch64",
    constraint_values = [
        "@platforms//cpu:aarch64",
        "@platforms//os:osx",
    ],
)

selects.config_setting_group(
    name = "osx_gcc",
    match_all = [
        "@platforms//os:osx",
        ":gcc",
    ],
)

selects.config_setting_group(
    name = "osx_clang",
    match_all = [
        "@platforms//os:osx",
        ":clang",
    ],
)

selects.config_setting_group(
    name = "windows_x86_64_gcc",
    match_all = [
        ":windows_x86_64",
        ":gcc",
    ],
)

selects.config_setting_group(
    name = "windows_x86_64_clang",
    match_all = [
        ":windows_x86_64",
        ":clang",
    ],
)

selects.config_setting_group(
    name = "windows_x86_64_msvc",
    match_all = [
        ":windows_x86_64",
        ":msvc",
    ],
)

selects.config_setting_group(
    name = "linux_x86_64_gcc",
    match_all = [
        ":linux_x86_64",
        ":gcc",
    ],
)

selects.config_setting_group(
    name = "linux_x86_64_clang",
    match_all = [
        ":linux_x86_64",
        ":clang",
    ],
)

selects.config_setting_group(
    name = "linux_aarch64_gcc",
    match_all = [
        ":linux_aarch64",
        ":gcc",
    ],
)

selects.config_setting_group(
    name = "linux_aarch64_clang",
    match_all = [
        ":linux_aarch64",
        ":clang",
    ],
)

selects.config_setting_group(
    name = "osx_x86_64_gcc",
    match_all = [
        ":osx_x86_64",
        ":gcc",
    ],
)

selects.config_setting_group(
    name = "osx_x86_64_clang",
    match_all = [
        ":osx_x86_64",
        ":clang",
    ],
)

selects.config_setting_group(
    name = "osx_aarch64_gcc",
    match_all = [
        ":osx_aarch64",
        ":gcc",
    ],
)

selects.config_setting_group(
    name = "osx_aarch64_clang",
    match_all = [
        ":osx_aarch64",
        ":clang",
    ],
)

platform(
    name = "windows_x86_64_platform",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:windows",
    ],
)

platform(
    name = "linux_x86_64_platform",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
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
    name = "osx_x86_64_platform",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:osx",
    ],
)

platform(
    name = "osx_aarch64_platform",
    constraint_values = [
        "@platforms//cpu:aarch64",
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

config_setting(
    name = "cross_compiling",
    define_values = {"cross_compiling": "true"},
)

config_setting(
    name = "not_cross_compiling",
    define_values = {"cross_compiling": "false"},
)

selects.config_setting_group(
    name = "cross_compiling_for_osx_gcc",
    match_all = [
        ":cross_compiling",
        "@platforms//os:osx",
        ":gcc",
    ],
)

selects.config_setting_group(
    name = "cross_compiling_for_osx_clang",
    match_all = [
        ":cross_compiling",
        "@platforms//os:osx",
        ":clang",
    ],
)

selects.config_setting_group(
    name = "not_cross_compiling_on_osx",
    match_all = [
        ":not_cross_compiling",
        "@platforms//os:osx",
    ],
)

selects.config_setting_group(
    name = "cross_compiling_for_windows_gcc",
    match_all = [
        ":cross_compiling",
        "@platforms//os:windows",
        ":gcc",
    ],
)

selects.config_setting_group(
    name = "not_cross_compiling_on_windows",
    match_all = [
        ":not_cross_compiling",
        "@platforms//os:windows",
        ":msvc",
    ],
)

