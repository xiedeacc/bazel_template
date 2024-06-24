package(default_visibility = ["//visibility:public"])

cc_library(
    name = "libevent",
    srcs = [
    ] + glob(
        [
            "*.c",
            "*.h",
        ],
        exclude = [
            "bufferevent_async.c",
            "win32select.c",
            "kqueue.c",
            "devpoll.c",
            "wepoll.c",
            "buffer_iocp.c",
            "event_iocp.c",
            "bufferevent_mbedtls.c",
            "evthread_win32.c",
        ],
    ),
    hdrs = [
        "arc4random.c",
    ] + glob(
        [
            "include/**/*.h",
        ],
        exclude = [
        ],
    ),
    copts = [
        "-Wall",
        "-Wextra",
        "-Wno-unused-parameter",
        "-Wstrict-aliasing",
        "-Wstrict-prototypes",
        "-Wundef",
        "-Wmissing-prototypes",
        "-Winit-self",
        "-Wmissing-field-initializers",
        "-Wdeclaration-after-statement",
        "-Waddress",
        "-Wnormalized=id",
        "-Woverride-init",
        "-Wlogical-op",
        "-Wwrite-strings",
        "-Wno-unused-function",
        "-Wno-pragmas",
        "-Wvla",
        "-Wstack-protector",
        "-O3",
        "-g",
        "-fPIC",
    ],
    includes = ["include"],
    linkopts = [
    ],
    local_defines = [
        "HAVE_CONFIG_H",
        "LITTLE_ENDIAN",
        "NDEBUG",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@openssl//:crypto",
        "@openssl//:ssl",
        "@zlib",
    ],
)
