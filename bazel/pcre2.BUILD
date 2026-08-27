load("@bazel_skylib//rules:copy_file.bzl", "copy_file")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": ["/Ox"],
    "//conditions:default": ["-O3"],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + [
    "HAVE_CONFIG_H",
    "PCRE2_CODE_UNIT_WIDTH=8",
] + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

DEFINES = GLOBAL_DEFINES

copy_file(
    name = "config_h_generic",
    src = "src/config.h.generic",
    out = "src/config.h",
)

copy_file(
    name = "pcre2_h_generic",
    src = "src/pcre2.h.generic",
    out = "src/pcre2.h",
)

copy_file(
    name = "pcre2_chartables_c",
    src = "src/pcre2_chartables.c.dist",
    out = "src/pcre2_chartables.c",
)

cc_library(
    name = "pcre2",
    srcs = [
        "src/pcre2_auto_possess.c",
        "src/pcre2_chkdint.c",
        "src/pcre2_compile.c",
        "src/pcre2_compile_cgroup.c",
        "src/pcre2_compile_class.c",
        "src/pcre2_config.c",
        "src/pcre2_context.c",
        "src/pcre2_convert.c",
        "src/pcre2_dfa_match.c",
        "src/pcre2_error.c",
        "src/pcre2_extuni.c",
        "src/pcre2_find_bracket.c",
        "src/pcre2_maketables.c",
        "src/pcre2_match.c",
        "src/pcre2_match_data.c",
        "src/pcre2_match_next.c",
        "src/pcre2_newline.c",
        "src/pcre2_ord2utf.c",
        "src/pcre2_pattern_info.c",
        "src/pcre2_script_run.c",
        "src/pcre2_serialize.c",
        "src/pcre2_string_utils.c",
        "src/pcre2_study.c",
        "src/pcre2_substitute.c",
        "src/pcre2_substring.c",
        "src/pcre2_tables.c",
        "src/pcre2_ucd.c",
        "src/pcre2_valid_utf.c",
        "src/pcre2_xclass.c",
        ":pcre2_chartables_c",
    ],
    hdrs = glob(["src/*.h"]) + [
        ":config_h_generic",
        ":pcre2_h_generic",
    ],
    copts = COPTS,
    defines = DEFINES + ["PCRE2_STATIC"],
    includes = ["src"],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    strip_include_prefix = "src",
)

cc_binary(
    name = "pcre2demo",
    srcs = ["src/pcre2demo.c"],
    copts = COPTS,
    defines = DEFINES,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [":pcre2"],
)
