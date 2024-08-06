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
    autogen = True,
    configure_in_place = True,
    configure_options = [
        "--enable-static",
        "--enable-shared",
        "--with-version=5.3.0-186-g21bcc0a8d49ab2944ae53c7e43f5c84fc8a34322",
    ] + select({
        "@platforms//cpu:aarch64": ["--host=aarch64-unknown-linux-gnu"],
        "//conditions:default": [],
    }),
    lib_name = "jemalloc",
    lib_source = ":all",
    out_binaries = [
        "jemalloc.sh",
        "jemalloc-config",
    ],
    out_shared_libs = [
        "libjemalloc.so",
        "libjemalloc.so.2",
    ],
    #out_static_libs = ["libjemalloc.a"],
    targets = [
        "install_lib_static",
        "install_bin",
        "install_lib",
        "install_include",
    ],
    toolchain = "@rules_foreign_cc//toolchains:preinstalled_automake_toolchain",
)

alias(
    name = "jemalloc",
    actual = ":jemalloc_build",
    visibility = ["//visibility:public"],
)
