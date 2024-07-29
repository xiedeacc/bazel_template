load("@rules_foreign_cc//foreign_cc:configure.bzl", "configure_make", "configure_make_variant")

filegroup(
    name = "all",
    srcs = glob(
        ["**"],
        exclude = [],
    ),
    visibility = ["//visibility:public"],
)

configure_make_variant(
    name = "jemalloc_build",
    args = ["-j"],
    #autogen = True,
    configure_in_place = True,
    configure_options = [
        "--enable-static",
    ] + select({
        "@platforms//cpu:aarch64": ["--host=aarch64-unknown-linux-gnu"],
        "//conditions:default": [],
    }),
    lib_name = "jemalloc",
    lib_source = ":all",
    out_static_libs = ["libjemalloc.a"],
    targets = [
        "install_lib_static",
        "install_include",
    ],
    toolchain = "@rules_foreign_cc//toolchains:preinstalled_automake_toolchain",
)

alias(
    name = "jemalloc",
    actual = ":jemalloc_build",
    visibility = ["//visibility:public"],
)
