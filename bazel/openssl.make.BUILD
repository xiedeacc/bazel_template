load("@bazel_skylib//lib:selects.bzl", "selects")
load("@rules_foreign_cc//foreign_cc:defs.bzl", "configure_make", "configure_make_variant")
load("@rules_foreign_cc//toolchains/native_tools:native_tools_toolchain.bzl", "native_tool_toolchain")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
)

#EXT_BUILD_DEPS introduced by rules_foreign_cc,it will generate a directory
#like openssl.ext_build_deps
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
    "enable-ec_nistp_64_gcc_128",
    "no-tests",
    "--with-zlib-include=$$EXT_BUILD_DEPS$$/include",
    "--with-zlib-lib=$$EXT_BUILD_DEPS$$/lib",
    "--with-zstd-include=$$EXT_BUILD_DEPS$$/include",
    "--with-zstd-lib=$$EXT_BUILD_DEPS$$/lib",
    "--with-brotli-include=$$EXT_BUILD_DEPS$$/include",
    "--with-brotli-lib=$$EXT_BUILD_DEPS$$/lib",
]

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
    #actual = ":openssl_static",
    actual = select({
        "@platforms//os:linux": ":openssl_static",
        "@platforms//os:osx": ":openssl_static",
        "@platforms//os:windows": ":openssl_static",
        "//conditions:default": ":openssl_static",
    }),
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

configure_make(
    name = "openssl_shared",
    args = ["-j4"],
    configure_command = "Configure",
    configure_in_place = True,
    configure_options = CONFIGURE_OPTIONS + select({
        "@bazel_template//bazel:linux_aarch64": ["linux-aarch64"],
        "@bazel_template//bazel:osx_x86_64": [
            #"darwin64-x86_64-cc",
            "enable-shared",
        ],
        "@bazel_template//bazel:windows_x86_64": [
            "mingw64",
            "no-shared",
        ],
        "//conditions:default": [],
    }),
    env = select({
        "@bazel_template//bazel:osx_x86_64": {"ARFLAGS": "-static -o"},
        "//conditions:default": {},
    }),
    lib_name = LIB_NAME,
    lib_source = ":all_srcs",
    out_lib_dir = selects.with_or({
        ("@platforms//cpu:aarch64", "@platforms//os:osx"): "lib",
        "//conditions:default": "lib64",
    }),
    out_shared_libs = select({
        "@platforms//os:osx": [
            "libssl.dylib",
            "libcrypto.dylib",
            "libssl.3.dylib",
            "libcrypto.3.dylib",
        ],
        "@platforms//os:linux": [
            "libssl.so",
            "libcrypto.so",
        ],
        "@platforms//os:windows": [
            "libssl.dll",
            "libcrypto.dll",
        ],
        "//conditions:default": [],
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

configure_make(
    name = "openssl_static",
    args = ["-j4"],
    build_data = select({
        "@bazel_template//bazel:cross_compiling_for_windows_gcc": [
            "@cc_toolchain_repo_x86_64_windows_generic_mingw-w64_gcc//:windres",
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
        ],
        "//conditions:default": [],
    }),
    env = select({
        "@platforms//os:osx": {"ARFLAGS": "-static -o"},
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
    out_static_libs = [
        "libssl.a",
        "libcrypto.a",
    ],
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
