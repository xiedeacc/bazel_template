load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LOCAL_DEFINES")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
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
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
        "_MBCS",
        "WIN32",
        "_WINDOWS",
        "NDEBUG",
    ],
    "//conditions:default": [],
})

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
            "quic/server/QuicServerBackendIoUring.cpp",
            "quic/server/QuicServerBackend.cpp",
            "quic/samples/**",
            "quic/**/test/**",
            "quic/docs/**",
            "quic/tools/tperf/**/*.cpp",
        ],
    ) + select({
        "@platforms//os:linux": ["quic/server/QuicServerBackendIoUring.cpp"],
        "@platforms//os:osx": [],
        "@platforms//os:windows": [],
        "//conditions:default": [],
    }),
    hdrs = glob(
        [
            "quic/**/*.h",
        ],
        exclude = [
            "quic/samples/**/*.h",
            "quic/**/test/**",
            "quic/docs/**",
            "quic/tools/tperf/**/*.h",
        ],
    ),
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@fizz",
    ],
)
