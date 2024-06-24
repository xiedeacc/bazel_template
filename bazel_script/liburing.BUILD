package(default_visibility = ["//visibility:public"])

cc_library(
    name = "liburing",
    srcs = [
        "src/arch/syscall-defs.h",
        "src/arch/x86/lib.h",
        "src/arch/x86/syscall.h",
        "src/ffi.c",
        "src/int_flags.h",
        "src/lib.h",
        "src/nolibc.c",
        "src/queue.c",
        "src/register.c",
        "src/setup.c",
        "src/syscall.c",
        "src/syscall.h",
        "src/version.c",
    ],
    hdrs = [
        "config-host.h",
        "src/include/liburing.h",
        "src/include/liburing/barrier.h",
        "src/include/liburing/compat.h",
        "src/include/liburing/io_uring.h",
        "src/include/liburing/io_uring_version.h",
    ],
    copts = [
        "-include config-host.h",
        "-g",
        "-O3",
        "-Wall",
        "-Wextra",
        "-Wno-unused-parameter",
        "-fPIC",
        "-fno-builtin",
        "-fno-stack-protector",
    ],
    includes = [
        "src/include",
    ],
    linkopts = [
        "-lpthread",
        "-nostdlib",
        "-nodefaultlibs",
    ],
    local_defines = [
        "_LARGEFILE_SOURCE",
        "_FILE_OFFSET_BITS=64",
        "_GNU_SOURCE",
        "_SANE_USERSPACE_TYPES__",
        "LIBURING_INTERNAL",
    ],
    visibility = ["//visibility:public"],
    deps = [
    ],
)
