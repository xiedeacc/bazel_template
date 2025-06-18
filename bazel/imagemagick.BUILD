load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

cc_library(
    name = "imagemagick",
    srcs = glob(
        [
            "Magick++/lib/**/*.cpp",
            "MagickCore/**/*.c",
            "MagickWand/**/*.c",
        ],
        exclude = ["MagickWand/tests/**/**.c"],
    ) + select({
        "@bazel_template//bazel:linux_x86_64": [
        ],
        "@bazel_template//bazel:osx_x86_64": [
        ],
        "@bazel_template//bazel:windows_x86_64": [
        ],
        "@platforms//cpu:aarch64": [],
        "//conditions:default": [],
    }) + [
    ],
    hdrs = [
        ":config_h",
    ],
    copts = COPTS,
    includes = ["include"],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
)

write_file(
    name = "config_h_in",
    out = "config/config.h.in",
    content = [],
)

template_rule(
    name = "config_h",
    src = ":config_h_in",
    out = "config/config.h",
    substitutions = {},
)
