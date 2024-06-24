load("@rules_foreign_cc//foreign_cc:configure.bzl", "configure_make")
load("@bazel_skylib//lib:selects.bzl", "selects")

filegroup(
    name = "all",
    srcs = glob(
        ["**"],
        exclude = [],
    ),
    visibility = ["//visibility:public"],
)

config_setting(
    name = "debug_tcmalloc",
    values = {"define": "tcmalloc=debug"},
)

configure_make(
    name = "gperftools_build",
    args = ["-j6"],
    configure_options = [
        "--enable-shared=no",
        "--enable-frame-pointers",
        "--disable-libunwind",
    ],
    copts = [
        #"-D_POSIX_C_SOURCE=199309L",
        "-std=c++17",
    ],
    lib_source = ":all",
    linkopts = [],
    out_static_libs = select({
        ":debug_tcmalloc": ["libtcmalloc_debug.a"],
        "//conditions:default": ["libtcmalloc_and_profiler.a"],
    }),
    tags = ["skip_on_windows"],
)

# Workaround for https://github.com/bazelbuild/rules_foreign_cc/issues/227
cc_library(
    name = "gperftools",
    tags = ["skip_on_windows"],
    visibility = ["//visibility:public"],
    deps = [
        "gperftools_build",
    ],
)
