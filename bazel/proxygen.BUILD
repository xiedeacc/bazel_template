load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LOCAL_DEFINES")
load("@bazel_template//bazel:proxygen.bzl", "is_external", "proxygen_cpp_gen")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
        "/Iexternal/libdwarf/src/lib/libdwarf",
        "/Iexternal/libsodium/src/libsodium/include",
        "/Iexternal/proxygen",
        "/I$(GENDIR)/external/proxygen",
        "/Iexternal/fbthrift",
        "/I$(GENDIR)/external/fbthrift",
        "/Iexternal/fb303",
        "/I$(GENDIR)/external/fb303",
        "/Iexternal/double-conversion",
        "/Iexternal/xxhash",
        "/Iexternal/com_googlesource_code_re2",
        "/Iexternal/fatal",
        "/I$(GENDIR)/external/folly",
        "/I$(GENDIR)/external/fizz",
        "/Iexternal/folly",
        "/Iexternal/fizz",
        "/Iexternal/wangle",
        "/Iexternal/mvfst",
        "/std:c++17",
    ],
    "//conditions:default": [
        "-Iexternal/libdwarf/src/lib/libdwarf",
        "-isystem external/libsodium/src/libsodium/include",
        "-isystem external/proxygen",
        "-isystem $(GENDIR)/external/proxygen",
        "-isystem external/fbthrift",
        "-isystem $(GENDIR)/external/fbthrift",
        "-isystem external/fb303",
        "-isystem $(GENDIR)/external/fb303",
        "-isystem external/double-conversion",
        "-isystem external/xxhash",
        "-isystem external/com_googlesource_code_re2",
        "-isystem external/fatal",
        "-isystem $(GENDIR)/external/folly",
        "-isystem $(GENDIR)/external/fizz",
        "-isystem external/folly",
        "-isystem external/fizz",
        "-isystem external/wangle",
        "-isystem external/mvfst",
        "-std=c++17",
        "-Wno-register",
        "-fsized-deallocation",
    ],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@bazel_template//bazel:not_cross_compiling_on_windows": [],
    "//conditions:default": [],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

sh_binary(
    name = "gen_HTTPCommonHeaders_sh",
    srcs = ["proxygen/lib/http/gen_HTTPCommonHeaders.sh"],
)

proxygen_cpp_gen(
    name = "common_headers",
    data = [
        "proxygen/lib/http/HTTPCommonHeaders.txt",
        "proxygen/lib/utils/gen_perfect_hash_table.sh",
        "proxygen/lib/utils/perfect_hash_table_template.cpp.gperf",
        "proxygen/lib/utils/perfect_hash_table_template.h",
    ],
    is_external = is_external(),
    out_files = [
        "proxygen/lib/http/HTTPCommonHeaders.h",
        "proxygen/lib/http/HTTPCommonHeaders.cpp",
    ],
    tool = ":gen_HTTPCommonHeaders_sh",
    txt_file = "proxygen/lib/http/HTTPCommonHeaders.txt",
)

sh_binary(
    name = "gen_StatsWrapper_sh",
    srcs = ["proxygen/lib/stats/gen_StatsWrapper.sh"],
)

proxygen_cpp_gen(
    name = "stats_headers",
    data = [
        "proxygen/lib/stats/BaseStats.h",
    ],
    is_external = is_external(),
    out_files = [
        "proxygen/lib/stats/StatsWrapper.h",
    ],
    tool = ":gen_StatsWrapper_sh",
)

genrule(
    name = "trace",
    srcs = [
        "proxygen/lib/utils/samples/TraceEventType.txt",
        "proxygen/lib/utils/samples/TraceFieldType.txt",
        "proxygen/lib/utils/gen_trace_event_constants.py",
    ],
    outs = [
        "proxygen/lib/utils/TraceFieldType.h",
        "proxygen/lib/utils/TraceEventType.h",
        "proxygen/lib/utils/TraceFieldType.cpp",
        "proxygen/lib/utils/TraceEventType.cpp",
    ],
    cmd = """
python3 $(location :proxygen/lib/utils/gen_trace_event_constants.py) \
--output_type=cpp \
--input_files=$(location :proxygen/lib/utils/samples/TraceEventType.txt),$(location :proxygen/lib/utils/samples/TraceFieldType.txt) \
--output_scope=proxygen \
--header_path=proxygen/lib/utils \
--install_dir=$(GENDIR)/external/proxygen/proxygen/lib/utils \
--fbcode_dir=external/proxygen
""",
)

cc_library(
    name = "proxygen",
    srcs = [
        ":common_headers",
        ":stats_headers",
        ":trace",
    ] + glob(
        [
            "proxygen/**/*.cpp",
        ],
        exclude = [
            "proxygen/lib/dns/*.cpp",  # c-ares version mismatch and some enum missed
            #"proxygen/lib/http/codec/compress/experimental/interop/QPACKInterop.cpp",
            "proxygen/lib/http/codec/compress/experimental/simulator/**/*.cpp",
            "proxygen/**/fuzzers/**/*.cpp",
            "proxygen/**/test/**/*.cpp",
            "proxygen/**/tests/**/*.cpp",
            "proxygen/**/samples/**/*.cpp",
            "proxygen/lib/transport/AsyncUDPSocketFactory.cpp",
        ],
    ) + select({
        "@bazel_template//bazel:not_cross_compiling_on_windows": [],
        "//conditions:default": ["proxygen/lib/transport/AsyncUDPSocketFactory.cpp"],
    }),
    hdrs = glob(["proxygen/**/*.h"]),
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@c-ares",
        "@fb303",
    ],
)

#cc_binary(
#name = "QPACKInterop",
#srcs = ["proxygen/lib/http/codec/compress/experimental/interop/QPACKInterop.cpp"],
#copts = COPTS,
#deps = [
#":proxygen",
#],
#)
