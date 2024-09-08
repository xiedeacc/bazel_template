package(default_visibility = ["//visibility:public"])

cc_library(
    name = "libuv",
    srcs = glob([
        "src/*.c",
    ]) + select({
        "@platforms//os:windows": glob(["src/win/*.c"]),
        "@platforms//os:osx": [
            "src/unix/async.c",
            "src/unix/bsd-ifaddrs.c",
            "src/unix/core.c",
            "src/unix/darwin.c",
            "src/unix/darwin-proctitle.c",
            "src/unix/dl.c",
            "src/unix/fs.c",
            "src/unix/fsevents.c",
            "src/unix/getaddrinfo.c",
            "src/unix/getnameinfo.c",
            "src/unix/internal.h",
            "src/unix/kqueue.c",
            "src/unix/loop.c",
            "src/unix/pipe.c",
            "src/unix/poll.c",
            "src/unix/process.c",
            "src/unix/proctitle.c",
            "src/unix/random-devurandom.c",
            "src/unix/random-getrandom.c",
            "src/unix/signal.c",
            "src/unix/stream.c",
            "src/unix/tcp.c",
            "src/unix/thread.c",
            "src/unix/tty.c",
            "src/unix/udp.c",
        ],
        "//conditions:default": [
            "src/unix/async.c",
            "src/unix/core.c",
            "src/unix/dl.c",
            "src/unix/fs.c",
            "src/unix/getaddrinfo.c",
            "src/unix/getnameinfo.c",
            "src/unix/internal.h",
            "src/unix/linux.c",
            "src/unix/loop.c",
            "src/unix/loop-watcher.c",
            "src/unix/pipe.c",
            "src/unix/poll.c",
            "src/unix/process.c",
            "src/unix/procfs-exepath.c",
            "src/unix/proctitle.c",
            "src/unix/random-devurandom.c",
            "src/unix/random-getrandom.c",
            "src/unix/random-sysctl-linux.c",
            "src/unix/signal.c",
            "src/unix/stream.c",
            "src/unix/tcp.c",
            "src/unix/thread.c",
            "src/unix/tty.c",
            "src/unix/udp.c",
        ],
    }),
    hdrs = [
        "include/uv.h",
        "include/uv/errno.h",
        "include/uv/threadpool.h",
        "include/uv/tree.h",
        "include/uv/version.h",
    ] + glob(["src/*.h"]) + select({
        "@platforms//os:windows": glob([
            "include/uv/win.h",
            "src/win/*.h",
        ]),
        "@platforms//os:osx": glob([
            "include/uv/darwin.h",
            "include/uv/posix.h",
            "include/uv/unix.h",
            "src/unix/darwin-stub.h",
        ]),
        "//conditions:default": [
            "include/uv/linux.h",
            "include/uv/posix.h",
            "include/uv/unix.h",
        ],
    }),
    copts = select({
        "@platforms//os:android": [],
        "@platforms//os:macos": [
            "-D_DARWIN_USE_64_BIT_INODE=1",
            "-D_DARWIN_UNLIMITED_SELECT=1",
            "-Iexternal/libuv/src",
            "-std=c11",
            "-pedantic",
            "-O3",
            "-pthread",
        ],
        "@bazel_template//bazel:cross_compiling_for_windows_gcc": [
            "-Iexternal/libuv/src/win",
            "-O2",
            "-DWIN32_LEAN_AND_MEAN",
            "-D_WIN32_WINNT=0x0601",
        ],
        "@bazel_template//bazel:not_cross_compiling_on_windows": [
            "/std:c11",
            "/Iexternal/libuv/src",
        ],
        "//conditions:default": [
            "-Iexternal/libuv/src/unix",
            "-Wno-tree-vrp",
            "-Wno-omit-frame-pointer",
            "-Iexternal/libuv/src",
            "-std=c11",
            "-pedantic",
            "-O3",
            "-pthread",
        ],
    }),
    includes = ["include"],
    linkopts = select({
        "@platforms//os:windows": [],
        "//conditions:default": [
            "-lpthread",
            "-ldl",
        ],
    }),
    local_defines = [
        "LARGEFILE_SOURCE",
        "_FILE_OFFSET_BITS=64",
        "SUPPORT_ATTRIBUTE_VISIBILITY_DEFAULT=1",
        "SUPPORT_FLAG_VISIBILITY=1",
        "HAVE_STDIO_H=1",
        "HAVE_STDLIB_H=1",
        "HAVE_STRING_H=1",
        "HAVE_INTTYPES_H=1",
        "HAVE_STDINT_H=1",
        "HAVE_SYS_STAT_H=1",
        "HAVE_SYS_TYPES_H=1",
        "STDC_HEADERS=1",
    ] + select({
        "@bazel_template//bazel:not_cross_compiling_on_windows": [
        ],
        "@platforms//os:osx": [
            "_DARWIN_USE_64_BIT_INODE=1",
            "_DARWIN_UNLIMITED_SELECT=1",
            "HAVE_DLFCN_H=1",
            "HAVE_PTHREAD_PRIO_INHERIT=1",
            "HAVE_STRINGS_H=1",
            "HAVE_UNISTD_H=1",
        ],
        "//conditions:default": [
            "_GNU_SOURCE",
            "HAVE_STRINGS_H=1",
            "HAVE_UNISTD_H=1",
        ],
    }),
)
