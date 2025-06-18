load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

cc_library(
    name = "blake3",
    srcs = select({
        "@bazel_template//bazel:linux_x86_64": [
            "c/blake3_avx2_x86-64_unix.S",
            "c/blake3_avx512_x86-64_unix.S",
            "c/blake3_sse2_x86-64_unix.S",
            "c/blake3_sse41_x86-64_unix.S",
        ],
        "@bazel_template//bazel:osx_x86_64": [
            "c/blake3_avx2_x86-64_unix.S",
            "c/blake3_avx512_x86-64_unix.S",
            "c/blake3_sse2_x86-64_unix.S",
            "c/blake3_sse41_x86-64_unix.S",
        ],
        "@bazel_template//bazel:windows_x86_64": [
            "c/blake3_avx2_x86-64_windows_msvc.asm",
            "c/blake3_avx512_x86-64_windows_msvc.asm",
            "c/blake3_sse2_x86-64_windows_msvc.asm",
            "c/blake3_sse41_x86-64_windows_msvc.asm",
        ],
        "@platforms//cpu:aarch64": ["c/blake3_neon.c"],
        "//conditions:default": [],
    }) + [
        "c/blake3.c",
        "c/blake3_dispatch.c",
        "c/blake3_portable.c",
    ],
    hdrs = [
        "c/blake3.h",
        "c/blake3_impl.h",
    ],
    copts = COPTS,
    includes = ["include"],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
)
