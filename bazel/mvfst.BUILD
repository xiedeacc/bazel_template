package(default_visibility = ["//visibility:public"])

config_setting(
    name = "windows_x86_64",
    constraint_values = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    visibility = ["//visibility:public"],
)

config_setting(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
)

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
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
            "quic/server/QuicServerBackendIoUring.cpp",
            "quic/server/QuicServerBackend.cpp",
            "quic/samples/**",
            "quic/**/test/**",
            "quic/docs/**",
            "quic/tools/tperf/**/*.cpp",
        ],
    ) + select({
        ":linux_x86_64": ["quic/server/QuicServerBackendIoUring.cpp"],
        ":linux_aarch64": ["quic/server/QuicServerBackendIoUring.cpp"],
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
        "-isystem $(BINDIR)/external/libsodium/src/libsodium/include",
        "-isystem external/double-conversion",
        "-isystem $(BINDIR)/external/folly",
        "-isystem external/folly",
        "-isystem $(BINDIR)/external/fizz",
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
