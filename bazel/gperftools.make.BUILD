load("@rules_foreign_cc//foreign_cc:configure.bzl", "configure_make")

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
    args = ["-j"],
    autogen = True,
    configure_in_place = True,
    configure_options = [
        "--enable-shared=yes",
        "--enable-static=no",
        "--enable-frame-pointers",
    ],
    copts = [
        "-std=c++17",
    ],
    lib_source = ":all",
    linkopts = [],
    out_shared_libs = [
        "libtcmalloc_and_profiler.so",
        "libtcmalloc_and_profiler.so.4",
        "libtcmalloc_and_profiler.so.4.6.11",
    ],
    #out_static_libs = select({
    #":debug_tcmalloc": ["libtcmalloc_debug.a"],
    #"//conditions:default": ["libtcmalloc_and_profiler.a"],
    #}),
    tags = ["skip_on_windows"],
    targets = [
        "install",
    ],
    deps = [
        "@libunwind//:unwind",
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
