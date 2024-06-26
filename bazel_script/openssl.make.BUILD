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

config_setting(
    name = "msvc_compiler",
    flag_values = {
        "@bazel_tools//tools/cpp:compiler": "msvc-cl",
    },
    visibility = ["//visibility:public"],
)

alias(
    name = "ssl",
    actual = "openssl",
    visibility = ["//visibility:public"],
)

alias(
    name = "crypto",
    actual = "openssl",
    visibility = ["//visibility:public"],
)

#alias(
#name = "openssl",
#actual = select({
#":msvc_compiler": "openssl_msvc",
#"//conditions:default": "openssl_default",
#}),
#visibility = ["//visibility:public"],
#)

alias(
    name = "openssl",
    actual = "openssl_default",
    visibility = ["//visibility:public"],
)

#configure_make_variant(
#name = "openssl_msvc",
#build_data = [
#"@nasm//:nasm",
#"@perl//:perl",
#],
#configure_command = "Configure",
#configure_in_place = True,
#configure_options = CONFIGURE_OPTIONS + [
#"VC-WIN64A",
## Unset Microsoft Assembler (MASM) flags set by built-in MSVC toolchain,
## as NASM is unsed to build OpenSSL rather than MASM
#"ASFLAGS=\" \"",
#],
#configure_prefix = "$$PERL",
#env = {
## The Zi flag must be set otherwise OpenSSL fails to build due to missing .pdb files
#"CFLAGS": "-Zi",
#"PATH": "$$(dirname $(execpath @nasm//:nasm)):$$PATH",
#"PERL": "$(execpath @perl//:perl)",
#},
#lib_name = LIB_NAME,
#lib_source = ":all_srcs",
#out_static_libs = [
#"libssl.lib",
#"libcrypto.lib",
#],
#targets = MAKE_TARGETS,
#toolchain = "@rules_foreign_cc//toolchains:preinstalled_nmake_toolchain",
#deps = [
#"@zlib",
#],
#)

# https://wiki.openssl.org/index.php/Compilation_and_Installation
configure_make(
    name = "openssl_default",
    args = ["-j"],
    configure_command = "config",
    configure_in_place = True,
    configure_options = CONFIGURE_OPTIONS,
    env = select({
        "@platforms//os:macos": {
            "AR": "",
            "PERL": "$$EXT_BUILD_ROOT$$/$(PERL)",
        },
        "//conditions:default": {
            "PERL": "$$EXT_BUILD_ROOT$$/$(PERL)",
        },
    }),
    lib_name = LIB_NAME,
    lib_source = ":all_srcs",
    linkopts = ["-ldl"],
    out_lib_dir = "lib64",
    out_static_libs = [
        "libcrypto.a",
        "libssl.a",
    ],
    targets = MAKE_TARGETS,
    toolchains = ["@rules_perl//:current_toolchain"],
    deps = [
        "@brotli//:brotlicommon",
        "@brotli//:brotlidec",
        "@brotli//:brotlienc",
        "@zlib",
        "@zstd",
    ],
)

filegroup(
    name = "gen_dir",
    srcs = [":openssl"],
    output_group = "gen_dir",
)
