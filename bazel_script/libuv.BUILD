cc_library(
    name = "libuv",
    srcs = glob([
        "src/*.c",
    ]) + select({
        "@platforms//os:windows": glob(["src/win/*.c"]),
        "//conditions:default": [
            "src/unix/internal.h",
            "src/unix/async.c",
            "src/unix/core.c",
            "src/unix/dl.c",
            "src/unix/fs.c",
            "src/unix/getaddrinfo.c",
            "src/unix/getnameinfo.c",
            "src/unix/loop-watcher.c",
            "src/unix/loop.c",
            "src/unix/pipe.c",
            "src/unix/poll.c",
            "src/unix/process.c",
            "src/unix/random-devurandom.c",
            "src/unix/signal.c",
            "src/unix/stream.c",
            "src/unix/tcp.c",
            "src/unix/thread.c",
            "src/unix/tty.c",
            "src/unix/udp.c",
            "src/unix/proctitle.c",
            "src/unix/linux.c",
            "src/unix/procfs-exepath.c",
            "src/unix/random-getrandom.c",
            "src/unix/random-sysctl-linux.c",
        ],
    }),
    hdrs = [
        "include/uv.h",
        "include/uv/errno.h",
        "include/uv/threadpool.h",
        "include/uv/version.h",
        "include/uv/tree.h",
    ] + glob(["src/*.h"]) + select({
        "@platforms//os:windows": glob([
            "include/uv/win.h",
            "src/win/*.h",
        ]),
        "//conditions:default": [
            "include/uv/linux.h",
            "include/uv/unix.h",
            "include/uv/posix.h",
        ],
    }),
    copts = [
        "-std=c11",
        "-pedantic",
        "-O3",
        "-Wno-error",
        "-Wno-strict-aliasing",
        "-Wstrict-aliasing",
        "-Wno-implicit-function-declaration",
        "-Wno-unused-function",
        "-Wno-unused-variable",
        "-pthread",
    ] + select({
        "@platforms//os:android": [],
        "@platforms//os:macos": [
            "-D_DARWIN_USE_64_BIT_INODE=1",
            "-D_DARWIN_UNLIMITED_SELECT=1",
        ],
        "@platforms//os:windows": [
            "-O2",
            "-DWIN32_LEAN_AND_MEAN",
            "-D_WIN32_WINNT=0x0600",
        ],
        "//conditions:default": [
            "-Wno-tree-vrp",
            "-Wno-omit-frame-pointer",
        ],
    }),
    includes = [
        "include",
        "src",
        "src/unix",
    ],
    linkopts = select({
        "@platforms//os:windows": [
            "-DEFAULTLIB:Ws2_32.lib",
            "-DEFAULTLIB:Iphlpapi.lib",
            "-DEFAULTLIB:Psapi.lib",
            "-DEFAULTLIB:User32.lib",
            "-DEFAULTLIB:Userenv.lib",
        ],
        "//conditions:default": [
            "-lpthread",
            "-ldl",
        ],
    }),
    linkstatic = True,
    local_defines = [
        "LARGEFILE_SOURCE",
        "_FILE_OFFSET_BITS=64",
        "_GNU_SOURCE",
    ],
    visibility = [
        "//visibility:public",
    ],
    deps = select({
        "@platforms//os:windows": [
            "@pthread_windows//:pthread",
        ],
        "//conditions:default": [],
    }),
)
