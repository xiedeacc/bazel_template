load("@bazel_skylib//lib:selects.bzl", "selects")

cc_library(
    name = "jemalloc",
    srcs = [
    ] + glob(
        [
            "src/**/*.c",
        ],
        exclude = [
            "src/zone.c",
            "test/**/*.cpp",
            "msvc/test_threads/test_threads_main.cpp",
            "msvc/test_threads/test_threads.cpp",
        ],
    ),
    hdrs = glob(["include/**/*.h"]),
    copts = [
        "-Wall",
        "-Wextra",
        "-Wsign-compare",
        "-Wundef",
        "-Wno-format-zero-length",
        "-Wpointer-arith",
        "-Wno-missing-braces",
        "-Wno-missing-field-initializers",
        "-Wno-missing-attributes",
        "-pipe",
        "-g3",
        "-fvisibility=hidden",
        "-Wimplicit-fallthrough",
        "-O3",
        "-funroll-loops",
        "-march=native",
    ],
    includes = [
        "include",
    ],
    linkopts = select({
        "@platforms//os:android": [],
        "//conditions:default": [
            "-lpthread",
            "-lm",
            "-lstdc++",
            "-lm",
            "-pthread",
        ],
    }),
    linkstatic = select({
        "@platforms//os:windows": True,
        "//conditions:default": False,
    }),
    local_defines = [
        "_GNU_SOURCE",
        "_REENTRANT",
    ],
    visibility = ["//visibility:public"],
    deps = [
        #"@libunwind//:unwind",
    ],
)

cc_library(
    name = "jemalloc_cpp",
    srcs = [
    ] + glob(
        [
            "src/**/*.cpp",
        ],
        exclude = [
            "test/**/*.cpp",
            "msvc/test_threads/test_threads_main.cpp",
            "msvc/test_threads/test_threads.cpp",
        ],
    ),
    hdrs = glob(["include/**/*.h"]),
    copts = [
        "-Wall",
        "-Wextra",
        "-Wsign-compare",
        "-Wundef",
        "-Wno-format-zero-length",
        "-Wpointer-arith",
        "-Wno-missing-braces",
        "-Wno-missing-field-initializers",
        "-Wno-missing-attributes",
        "-Wimplicit-fallthrough",
        "-pipe",
        "-std=c++17",
        "-g3",
        "-O3",
        "-funroll-loops",
    ],
    includes = [
        "include",
    ],
    linkopts = select({
        "@platforms//os:android": [],
        "//conditions:default": [
            "-lstdc++",
            "-lm",
            "-pthread",
        ],
    }),
    linkstatic = select({
        "@platforms//os:windows": True,
        "//conditions:default": False,
    }),
    local_defines = [
        "_GNU_SOURCE",
        "_REENTRANT",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":jemalloc",
    ],
)
