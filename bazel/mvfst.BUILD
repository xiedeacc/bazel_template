load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES", "template_rule")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [
        "/Iexternal/libdwarf/src/lib/libdwarf",
        "/Iexternal/mvfst",
        "/Iexternal/libsodium/src/libsodium/include",
        "/Iexternal/libsodium/src/libsodium/include/sodium",
        "/I$(GENDIR)/external/libsodium/src/libsodium/include",
        "/Iexternal/double-conversion",
        "/I$(GENDIR)/external/folly",
        "/Iexternal/folly",
        "/I$(GENDIR)/external/fizz",
        "/Iexternal/fizz",
        "/Iexternal/libev",
    ],
    "//conditions:default": [
        "-Iexternal/libdwarf/src/lib/libdwarf",
        "-isystem external/mvfst",
        "-isystem external/libsodium/src/libsodium/include",
        "-Iexternal/libsodium/src/libsodium/include/sodium",
        "-isystem $(GENDIR)/external/libsodium/src/libsodium/include",
        "-isystem external/double-conversion",
        "-isystem $(GENDIR)/external/folly",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/fizz",
        "-isystem external/fizz",
        "-isystem external/libev",
    ],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@platforms//os:windows": [
        "_MBCS",
        "WIN32",
        "_WINDOWS",
        "NDEBUG",
    ],
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

template_rule(
    name = "quic_logging_config_h",
    src = "quic/quic-logging-config.h.in",
    out = "quic/quic-logging-config.h",
    substitutions = {
        "@MVFST_LOGGING_BACKEND@": "GLOG",
    },
)

cc_library(
    name = "mvfst",
    srcs = glob(
        [
            "quic/**/*.cpp",
        ],
        exclude = [
            "quic/common/events/LibevQuicEventBase.cpp",
            "quic/common/udpsocket/LibevQuicAsyncUDPSocket.cpp",
            "quic/api/QuicBatchWriterFactoryMobile.cpp",
            "quic/api/QuicBatchWriterFactoryMobileGSO.cpp",
            "quic/congestion_control/CongestionControllerFactoryMobile.cpp",
            "quic/congestion_control/PacerFactoryMobile.cpp",
            "quic/fizz/handshake/FizzPacketNumberCipher.cpp",
            "quic/server/QuicServerBackendIoUring.cpp",
            "quic/server/QuicServerBackend.cpp",
            "quic/samples/**",
            "quic/**/test/**",
            "quic/docs/**",
            "quic/tools/tperf/**/*.cpp",
        ],
    ) + select({
        "@platforms//os:linux": [],
        "@platforms//os:osx": [],
        "@platforms//os:windows": [],
        "//conditions:default": [],
    }),
    hdrs = [":quic_logging_config_h"] + glob(
        [
            "quic/**/*.h",
            "quic/**/*.hpp",
        ],
        exclude = [
            "quic/samples/**/*.h",
            "quic/**/test/**",
            "quic/docs/**",
            "quic/tools/tperf/**/*.h",
        ],
    ),
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@fizz",
    ],
)
