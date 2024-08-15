load("@bazel_skylib//lib:selects.bzl", "selects")
load("@rules_foreign_cc//foreign_cc:defs.bzl", "configure_make", "configure_make_variant")

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
    actual = "openssl",
)

alias(
    name = "crypto",
    actual = "openssl",
)

alias(
    name = "openssl",
    actual = "openssl_default",
)

configure_make(
    name = "openssl_default",
    args = ["-j"],
    configure_command = "Configure",
    configure_in_place = True,
    configure_options = CONFIGURE_OPTIONS + select({
        "@bazel_template//bazel:linux_aarch64": ["linux-aarch64"],
        "@bazel_template//bazel:osx_x86_64": ["darwin64-x86_64-cc"],  #darwin64-x86_64-cc
        "//conditions:default": [],
    }),
    env = select({
        "@bazel_template//bazel:osx_x86_64": {"ARFLAGS": "-static -o"},
        "//conditions:default": {},
    }),
    lib_name = LIB_NAME,
    lib_source = ":all_srcs",
    #linkopts = ["-ldl"],
    out_lib_dir = selects.with_or({
        ("@platforms//cpu:aarch64", "@platforms//os:osx"): "lib",
        "//conditions:default": "lib64",
    }),
    out_static_libs = [
        "libssl.a",
        "libcrypto.a",
    ],
    targets = MAKE_TARGETS,
    toolchains = ["@rules_perl//:current_toolchain"],
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
