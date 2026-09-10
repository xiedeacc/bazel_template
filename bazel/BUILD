load("@rules_python//python:defs.bzl", "py_binary")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "exported_testdata",
    srcs = glob(["**"]),
)

py_binary(
    name = "gen_local_config_git",
    srcs = ["gen_local_config_git.py"],
    legacy_create_init = 0,
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
    name = "libc_glibc",
    define_values = {"libc": "glibc"},
)

config_setting(
    name = "libc_musl",
    define_values = {"libc": "musl"},
)

config_setting(
    name = "msvc",
    values = {"compiler": "msvc-cl"},
)

config_setting(
    name = "cpu_model_n105",
    define_values = {"cpu_model": "n105"},
)

config_setting(
    name = "cpu_model_ryzen5",
    define_values = {"cpu_model": "ryzen5"},
)

config_setting(
    name = "cpu_model_ryzen9",
    define_values = {"cpu_model": "ryzen9"},
)

config_setting(
    name = "cpu_model_neoverse11",
    define_values = {"cpu_model": "neoverse-11"},
)

config_setting(
    name = "osx_ryzen9",
    constraint_values = [
        "@platforms//os:osx",
    ],
    define_values = {"cpu_model": "ryzen9"},
)

config_setting(
    name = "linux_n105",
    constraint_values = [
        "@platforms//os:linux",
    ],
    define_values = {"cpu_model": "n105"},
)

config_setting(
    name = "linux_ryzen5",
    constraint_values = [
        "@platforms//os:linux",
    ],
    define_values = {"cpu_model": "ryzen5"},
)

config_setting(
    name = "linux_ryzen9",
    constraint_values = [
        "@platforms//os:linux",
    ],
    define_values = {"cpu_model": "ryzen9"},
)

config_setting(
    name = "linux_neoverse11",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
    define_values = {"cpu_model": "neoverse-11"},
)

config_setting(
    name = "windows_n105",
    constraint_values = [
        "@platforms//os:windows",
    ],
    define_values = {"cpu_model": "n105"},
)

config_setting(
    name = "windows_ryzen5",
    constraint_values = [
        "@platforms//os:windows",
    ],
    define_values = {"cpu_model": "ryzen5"},
)

config_setting(
    name = "windows_ryzen9",
    constraint_values = [
        "@platforms//os:windows",
    ],
    define_values = {"cpu_model": "ryzen9"},
)

config_setting(
    name = "windows_neoverse11",
    constraint_values = [
        "@platforms//os:windows",
    ],
    define_values = {"cpu_model": "neoverse-11"},
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
    name = "linux_x86_64_glibc_platform",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
        "@cc_toolchains//toolchain:libc_glibc",
    ],
)

platform(
    name = "linux_x86_64_musl_platform",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
        "@cc_toolchains//toolchain:libc_musl",
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
    name = "linux_aarch64_glibc_platform",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
        "@cc_toolchains//toolchain:libc_glibc",
    ],
)

platform(
    name = "linux_aarch64_musl_platform",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
        "@cc_toolchains//toolchain:libc_musl",
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
