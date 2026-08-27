load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS")

package(default_visibility = ["//visibility:public"])

VLMCSD_DEFINES = [
    "NO_TIMEOUT",
    "NO_SIGHUP",
    "NO_CL_PIDS",
    "NO_LOG",
    "NO_RANDOM_EPID",
    "NO_INI_FILE",
    "NO_HELP",
    "NO_CUSTOM_INTERVALS",
    "NO_PID_FILE",
    "NO_USER_SWITCH",
    "NO_VERBOSE_LOG",
    "NO_LIMIT",
    "NO_VERSION_INFORMATION",
    "NO_PRIVATE_IP_DETECT",
    "NO_STRICT_MODES",
    "NO_CLIENT_LIST",
    "NO_TAP",
    "NO_EXTERNAL_DATA",
    "UNSAFE_DATA_LOAD",
    "SIMPLE_RPC",
    "USE_THREADS",
    "IS_LIBRARY=1",
    "VERSION=\\\"vlmcsd-embedded\\\"",
    "_CRYPTO_INTERNAL",
]

VLMCS_DEFINES = [
    d
    for d in VLMCSD_DEFINES
    if d not in ("IS_LIBRARY=1", "NO_LOG", "NO_VERBOSE_LOG")
] + ["NO_DNS"]

cc_library(
    name = "libkms",
    srcs = [
        "src/crypto.c",
        "src/crypto_internal.c",
        "src/endian.c",
        "src/helpers.c",
        "src/kms.c",
        "src/libkms.c",
        "src/network.c",
        "src/output.c",
        "src/rpc.c",
        "src/shared_globals.c",
        "src/vlmcs.c",
    ],
    hdrs = glob(["src/*.h"]),
    copts = GLOBAL_COPTS + select({
        "@platforms//os:windows": [
            "/FIwinsock2.h",
        ],
        "//conditions:default": [
            "-pthread",
            # vlmcsd type-puns byte buffers through DWORD pointers in its
            # crypto code. Upstream builds with -fno-strict-aliasing for that
            # reason; without it GCC miscompiles SHA-256 and every KMS v5/v6
            # response fails its hash and HMAC checks.
            "-fno-strict-aliasing",
            "-Wno-discarded-qualifiers",
            "-Wno-incompatible-pointer-types",
            "-Wno-missing-field-initializers",
            "-Wno-sign-compare",
            "-Wno-unused-function",
            "-Wno-unused-parameter",
        ],
    }),
    defines = GLOBAL_DEFINES + VLMCSD_DEFINES + select({
        "@platforms//os:windows": ["EXTERNAL=dllexport"],
        "//conditions:default": [],
    }),
    linkopts = GLOBAL_LINKOPTS + select({
        "@platforms//os:windows": [],
        "//conditions:default": ["-pthread"],
    }),
)

# vlmcsd's own KMS client emulator, used to exercise a KMS host end to end.
cc_binary(
    name = "vlmcs",
    srcs = [
        "src/crypto.c",
        "src/crypto_internal.c",
        "src/endian.c",
        "src/helpers.c",
        "src/kms.c",
        "src/kmsdata-full.c",
        "src/network.c",
        "src/output.c",
        "src/rpc.c",
        "src/shared_globals.c",
        "src/vlmcs.c",
    ] + select({
        "@platforms//os:windows": ["src/wingetopt.c"],
        "//conditions:default": [],
    }),
    copts = GLOBAL_COPTS + select({
        "@platforms//os:windows": [
            "/FIwinsock2.h",
        ],
        "//conditions:default": [
            "-pthread",
            # vlmcsd type-puns byte buffers through DWORD pointers in its
            # crypto code. Upstream builds with -fno-strict-aliasing for that
            # reason; without it GCC miscompiles SHA-256 and every KMS v5/v6
            # response fails its hash and HMAC checks.
            "-fno-strict-aliasing",
            "-Wno-discarded-qualifiers",
            "-Wno-incompatible-pointer-types",
            "-Wno-missing-field-initializers",
            "-Wno-sign-compare",
            "-Wno-unused-function",
            "-Wno-unused-parameter",
        ],
    }),
    defines = GLOBAL_DEFINES + VLMCS_DEFINES,
    linkopts = GLOBAL_LINKOPTS + select({
        "@platforms//os:windows": [
            "-DEFAULTLIB:ws2_32.lib",
            "-DEFAULTLIB:shlwapi.lib",
        ],
        "//conditions:default": ["-pthread"],
    }),
)
