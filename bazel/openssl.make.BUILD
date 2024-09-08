load("@bazel_skylib//lib:selects.bzl", "selects")
load("@rules_foreign_cc//foreign_cc:defs.bzl", "configure_make")
load("@rules_foreign_cc//toolchains/native_tools:native_tools_toolchain.bzl", "native_tool_toolchain")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
)

CONFIGURE_OPTIONS = [
    "enable-brotli",
    "enable-egd",
    "enable-tfo",
    "enable-thread-pool",
    "enable-default-thread-pool",
    "enable-zlib",
    "enable-zstd",
    "enable-rfc3779",
    "enable-cms",
    "no-tests",
    "--with-zlib-include=$$$$EXT_BUILD_DEPS$$$$/include",
    "--with-zlib-lib=$$$$EXT_BUILD_DEPS$$$$/lib",
    "--with-zstd-include=$$$$EXT_BUILD_DEPS$$$$/include",
    "--with-zstd-lib=$$$$EXT_BUILD_DEPS$$$$/lib",
    "--with-brotli-include=$$$$EXT_BUILD_DEPS$$$$/include",
    "--with-brotli-lib=$$$$EXT_BUILD_DEPS$$$$/lib",
] + select({
    "@bazel_template//bazel:not_cross_compiling_on_windows": [],
    "//conditions:default": ["enable-ec_nistp_64_gcc_128"],
})

LIB_NAME = "openssl"

MAKE_TARGETS = [
    "build_libs",
    "install_dev",
]

alias(
    name = "ssl",
    actual = ":openssl",
)

alias(
    name = "crypto",
    actual = ":openssl",
)

alias(
    name = "openssl",
    actual = ":openssl_static",
)

native_tool_toolchain(
    name = "preinstalled_pkgconfig",
    path = "pkg-config",
)

toolchain(
    name = "preinstalled_pkgconfig_toolchain",
    toolchain = ":preinstalled_pkgconfig",
    toolchain_type = "@rules_foreign_cc//toolchains:pkgconfig_toolchain",
)

native_tool_toolchain(
    name = "preinstalled_make",
    path = "make",
)

toolchain(
    name = "preinstalled_make_toolchain",
    toolchain = ":preinstalled_make",
    toolchain_type = "@rules_foreign_cc//toolchains:make_toolchain",
)

native_tool_toolchain(
    name = "preinstalled_nmake",
    path = "nmake.exe",
)

toolchain(
    name = "preinstalled_nmake_toolchain",
    exec_compatible_with = [
        "@platforms//os:windows",
    ],
    toolchain = ":preinstalled_nmake",
    toolchain_type = "@rules_foreign_cc//toolchains:make_toolchain",
)

configure_make(
    name = "openssl_static",
    args = select({
        "@bazel_template//bazel:not_cross_compiling_on_osx": ["-j4"],
        "@bazel_template//bazel:not_cross_compiling_on_windows": [],
        "//conditions:default": ["-j"],
    }),
    build_data = select({
        "@bazel_template//bazel:cross_compiling_for_windows_gcc": [
            "@cc_toolchain_repo_x86_64_windows_generic_mingw-w64_gcc//:windres",
        ],
        "@bazel_template//bazel:not_cross_compiling_on_windows": [
            "@nasm//:nasm",
            "@perl//:perl",
        ],
        "//conditions:default": [],
    }),
    configure_command = "Configure",
    configure_in_place = True,
    configure_options = CONFIGURE_OPTIONS + select({
        "@bazel_template//bazel:linux_aarch64": ["linux-aarch64"],
        "@bazel_template//bazel:cross_compiling_for_osx": [
            "darwin64-x86_64-cc",
        ],
        "@bazel_template//bazel:cross_compiling_for_windows": [
            "mingw64",
            "no-shared",
        ],
        "@bazel_template//bazel:not_cross_compiling_on_windows": [
            "VC-WIN64A",
            "no-shared",
            "ASFLAGS=\" \"",
            "CC=cl.exe",
            "LD=link.exe",
            "AR=lib.exe",
        ],
        "//conditions:default": [],
    }),
    configure_prefix = "$$PERL",
    # copts = select({
    #     "@bazel_template//bazel:not_cross_compiling_on_windows": [],
    #     "//conditions:default": [],
    # }),
    env = select({
        "@platforms//os:osx": {"ARFLAGS": "-static -o"},
        "@bazel_template//bazel:not_cross_compiling_on_windows": {
            "PATH": "$$(dirname $(execpath @nasm//:nasm)):$$PATH",
            "PERL": "$(execpath @perl//:perl)",
            "CL": "/MP16",
        },
        "@bazel_template//bazel:cross_compiling_for_windows": {
            "WINDRES": "x86_64-w64-mingw32-windres",
            "PATH": "$$(dirname $(execpath @cc_toolchain_repo_x86_64_windows_generic_mingw-w64_gcc//:windres)):$$PATH",
        },
        "//conditions:default": {},
    }),
    lib_name = LIB_NAME,
    lib_source = ":all_srcs",
    out_lib_dir = selects.with_or({
        ("@platforms//cpu:aarch64", "@platforms//os:osx", "@platforms//os:windows"): "lib",
        "//conditions:default": "lib64",
    }),
    out_static_libs = select({
        "@bazel_template//bazel:not_cross_compiling_on_windows": [
            "libssl.lib",
            "libcrypto.lib",
        ],
        "//conditions:default": [
            "libssl.a",
            "libcrypto.a",
        ],
    }),
    targets = MAKE_TARGETS,
    deps = [
        "@brotli//:brotlicommon",
        "@brotli//:brotlidec",
        "@brotli//:brotlienc",
        "@zlib//:z",
        "@zstd",
    ],
)

filegroup(
    name = "gen_dir",
    srcs = [":openssl"],
    output_group = "gen_dir",
)
