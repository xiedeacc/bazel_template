package(default_visibility = ["//visibility:public"])

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
    copts = [
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
        "-std=c++17",
    ],
    local_defines = [
        #"MVFST_USE_LIBEV",
    ],
    deps = [
        "@fizz",
        #"@libev//:ev",
    ],
)
