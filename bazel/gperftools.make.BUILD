load("@rules_foreign_cc//foreign_cc:configure.bzl", "configure_make", "configure_make_variant")

filegroup(
    name = "all",
    srcs = glob(
        ["**"],
        exclude = [],
    ),
    visibility = ["//visibility:public"],
)

configure_make(
    name = "gperftools_build",
    args = ["-j"],
    autogen = True,
    configure_in_place = True,
    configure_options = [
        "--enable-shared=yes",
        #"--enable-static=no",
        #"--disable-static",
        "--enable-frame-pointers",
        "--enable-libunwind",
    ],
    copts = [
        #"-std=c++17",
    ],
    lib_source = ":all",
    linkopts = [
        #"$$EXT_BUILD_DEPS$$/lib/libunwind.a",
        #"-static",
        "-lunwind",
        "-llzma",
        "-lz",
    ],
    out_shared_libs = [
        "libtcmalloc_and_profiler.so",
    ],
    #out_static_libs = [
    #"libtcmalloc_and_profiler.a",
    #],
    tags = ["skip_on_windows"],
    targets = [
        "install",
    ],
    #toolchain = "@rules_foreign_cc//toolchains:preinstalled_automake_toolchain",
    deps = [
        "@libunwind//:unwind",
        "@xz",
        "@zstd",
    ],
)

# Workaround for https://github.com/bazelbuild/rules_foreign_cc/issues/227
cc_library(
    name = "gperftools",
    tags = ["skip_on_windows"],
    visibility = ["//visibility:public"],
    deps = [
        ":gperftools_build",
    ],
)
