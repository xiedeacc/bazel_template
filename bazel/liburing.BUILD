package(default_visibility = ["//visibility:public"])

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

config_setting(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

platform(
    name = "linux_aarch64_platform",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

COPTS = [
    "-g",
    "-O3",
    "-Wall",
    "-Wextra",
    "-Wno-unused-parameter",
    "-nostdlib",
    "-nodefaultlibs",
    "-ffreestanding",
    "-fno-builtin",
    "-fno-stack-protector",
]

LOCAL_DEFINES = [
    "_LARGEFILE_SOURCE",
    "_FILE_OFFSET_BITS=64",
    "_GNU_SOURCE",
    "_SANE_USERSPACE_TYPES__",
    "LIBURING_INTERNAL",
    "CONFIG_NOLIBC",
    "CONFIG_HAVE_KERNEL_RWF_T",
    "CONFIG_HAVE_KERNEL_TIMESPEC",
    "CONFIG_HAVE_OPEN_HOW",
    "CONFIG_HAVE_STATX",
    "CONFIG_HAVE_GLIBC_STATX",
    "CONFIG_HAVE_CXX",
    "CONFIG_HAVE_UCONTEXT",
    "CONFIG_HAVE_STRINGOP_OVERFLOW",
    "CONFIG_HAVE_ARRAY_BOUNDS",
    "CONFIG_HAVE_FANOTIFY",
]

genrule(
    name = "io_uring_version_h",
    outs = ["src/include/liburing/io_uring_version.h"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "/* SPDX-License-Identifier: MIT */",
        "#ifndef LIBURING_VERSION_H",
        "#define LIBURING_VERSION_H",
        "",
        "#define IO_URING_VERSION_MAJOR 2",
        "#define IO_URING_VERSION_MINOR 7",
        "",
        "#endif",
        "EOF",
    ]),
)

genrule(
    name = "compat_h",
    outs = ["src/include/liburing/compat.h"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "/* SPDX-License-Identifier: MIT */",
        "#ifndef LIBURING_COMPAT_H",
        "#define LIBURING_COMPAT_H",
        "",
        "#include <linux/time_types.h>",
        "/* <linux/time_types.h> is included above and not needed again */",
        "#define UAPI_LINUX_IO_URING_H_SKIP_LINUX_TIME_TYPES_H 1",
        "",
        "#include <linux/openat2.h>",
        "",
        "#include <inttypes.h>",
        "",
        "#define FUTEX_32       2",
        "#define FUTEX_WAITV_MAX        128",
        "",
        "struct futex_waitv {",
        "       uint64_t        val;",
        "       uint64_t        uaddr;",
        "       uint32_t        flags;",
        "       uint32_t        __reserved;",
        "};",
        "",
        "#endif",
        "EOF",
    ]),
)

cc_library(
    name = "private_header",
    hdrs = [
        "src/arch/syscall-defs.h",
    ],
    visibility = ["//visibility:private"],
)

cc_library(
    name = "uring",
    srcs = glob(
        [
            "src/*.c",
            "src/*.h",
        ],
        exclude = ["src/ffi.c"],
    ) + select({
        ":linux_x86_64": [
            "src/arch/x86/lib.h",
            "src/arch/x86/syscall.h",
        ],
        ":linux_aarch64": [
            "src/arch/aarch64/lib.h",
            "src/arch/aarch64/syscall.h",
        ],
    }) + [
        ":io_uring_version_h",
        ":compat_h",
    ],
    hdrs = glob([
        "src/include/**/*.h",
    ]),
    copts = COPTS,
    includes = [
        "src/include",
    ],
    linkopts = [
    ],
    local_defines = LOCAL_DEFINES + select({
        ":linux_x86_64": [
        ],
        ":linux_aarch64": [
        ],
    }),
    visibility = ["//visibility:public"],
    deps = [
        ":private_header",
    ],
)

cc_library(
    name = "liburing-ffi",
    srcs = glob([
        "src/*.c",
        "src/*.h",
    ]) + select({
        ":linux_x86_64": [
            "src/arch/x86/lib.h",
            "src/arch/x86/syscall.h",
        ],
        ":linux_aarch64": [
            "src/arch/aarch64/lib.h",
            "src/arch/aarch64/syscall.h",
        ],
    }) + [
        ":io_uring_version_h",
        ":compat_h",
    ],
    hdrs = glob([
        "src/include/**/*.h",
    ]),
    copts = COPTS,
    includes = [
        "src/include",
    ],
    linkopts = [
    ],
    local_defines = LOCAL_DEFINES + select({
        ":linux_x86_64": [
        ],
        ":linux_aarch64": [
        ],
    }),
    visibility = ["//visibility:public"],
    deps = [
        ":private_header",
    ],
)
