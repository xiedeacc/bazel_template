load("@bazel_skylib//rules:write_file.bzl", "write_file")
load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES", "template_rule")
load("@bazel_template//bazel:glob.bzl", "optional_glob")

package(default_visibility = ["//visibility:public"])

COPTS_BASE = GLOBAL_COPTS + [
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-auth/include",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-cal/include",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-common/include",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-compression/include",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-event-stream/include",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-http/include",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-io/include",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-mqtt/include",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-s3/include",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-sdkutils/include",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-checksums/include",
    "-I$(GENDIR)/external/aws-sdk-cpp/crt/aws-c-common/generated/include",
    "-I$(GENDIR)/external/aws-sdk-cpp/crt/aws-crt-cpp/generated/include",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-common/source",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-common/source/external/libcbor",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-common/include",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/s2n",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/s2n/api",
    "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/include",
    "-include",
    "s2n_prelude.h",
]

COPTS = COPTS_BASE + select({
    "@platforms//os:windows": [],
    "@platforms//os:linux": [
        "-fPIC",
        "-D_GNU_SOURCE",
    ],
    "@platforms//os:osx": [
        "-fPIC",
    ],
    "//conditions:default": [],
})

AWS_CRT_COPTS = COPTS + select({
    "@platforms//os:linux": [
        "-include",
        "linux/limits.h",
    ],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + [
    "AWS_SDK_VERSION_MAJOR=1",
    "AWS_SDK_VERSION_MINOR=11",
    "AWS_SDK_VERSION_PATCH=853",
    "AWS_SDK_VERSION_STRING=\"1.11.853\"",
    "AWS_APPSTORE_SAFE=ON",
    "ENABLE_TESTING=OFF",
    "AWS_COMMON_EXPORTS",
    "AWS_COMMON_USE_IMPORT_EXPORT",
    "AWS_TEST_REGION=US_EAST_1",
    "CJSON_HIDE_SYMBOLS",
    "ENABLED_REQUEST_COMPRESSION",
    "ENABLED_ZLIB_REQUEST_COMPRESSION",
    "ENABLE_CURL_CLIENT",
    "ENFORCE_TLS_V1_2",
    "INTEL_NO_ITTNOTIFY_API",
    "aws_c_common_EXPORTS",
    "AWS_S2N_INSOURCE_PATH",
    "USE_S2N",
] + select({
    # Base architecture defines
    "@platforms//cpu:x86_64": [
        "AWS_ARCH_INTEL",
        "AWS_ARCH_INTEL_X64",
    ],
    "@platforms//cpu:aarch64": [
        "AWS_ARCH_ARM64",
    ],
    "//conditions:default": [],
}) + select({
    # CPU tier-specific defines (x86_64 only)
    "@bazel_template//bazel:cpu_model_ryzen9": [
        "AWS_HAVE_AVX2_INTRINSICS",
        "AWS_HAVE_MM256_EXTRACT_EPI64",
        "AWS_HAVE_AVX512_INTRINSICS",
        "AWS_HAVE_CLMUL",
        "AWS_USE_CPU_EXTENSIONS",
        "USE_SIMD_ENCODING",  # Enable SIMD base64 encoding
    ],
    "@bazel_template//bazel:cpu_model_ryzen5": [
        "AWS_HAVE_AVX2_INTRINSICS",
        "AWS_HAVE_MM256_EXTRACT_EPI64",
        "AWS_HAVE_CLMUL",
        "AWS_USE_CPU_EXTENSIONS",
        "USE_SIMD_ENCODING",  # Enable SIMD base64 encoding
    ],
    "@bazel_template//bazel:cpu_model_n105": [],
    "@bazel_template//bazel:cpu_model_neoverse11": [
        "AWS_HAVE_ARMv8_1",
        "AWS_USE_CPU_EXTENSIONS",
    ],
    "//conditions:default": [],
}) + select({
    "@platforms//os:windows": [
        "WIN32",
        "_WIN32",
        "UNICODE",
        "_UNICODE",
        "NOMINMAX",
        "WIN32_LEAN_AND_MEAN",
        "AWS_ENABLE_IO_COMPLETION_PORTS",
        "AWS_HAVE_WINAPI_DESKTOP",
    ],
    "@platforms//os:linux": [
        "_GNU_SOURCE",
        "AWS_HAVE_POSIX_LARGE_FILE_SUPPORT",
        "AWS_HAVE_LINUX_IF_LINK_H",
        "AWS_PTHREAD_GETNAME_TAKES_3ARGS",
        "AWS_PTHREAD_SETNAME_TAKES_2ARGS",
        "HAVE_SYSCONF",
        "PLATFORM_LINUX",
        "USE_SIMD_ENCODING",
        "_POSIX_C_SOURCE=200809L",
        "_XOPEN_SOURCE=500",
        "AWS_ENABLE_EPOLL",
    ],
    "@platforms//os:osx": [
        "AWS_HAVE_POSIX_LARGE_FILE_SUPPORT",
        "AWS_HAVE_EXECINFO",
        "AWS_ENABLE_KQUEUE",
    ],
    "//conditions:default": [],
}) + select({
    "@bazel_template//bazel:libc_musl": [
        "AWS_AFFINITY_METHOD=AWS_AFFINITY_METHOD_NONE",
    ],
    "@platforms//os:osx": [
        "AWS_AFFINITY_METHOD=AWS_AFFINITY_METHOD_NONE",
    ],
    "@platforms//os:windows": [
        "AWS_AFFINITY_METHOD=AWS_AFFINITY_METHOD_NONE",
    ],
    "//conditions:default": [
        "AWS_AFFINITY_METHOD=AWS_AFFINITY_METHOD_PTHREAD_ATTR",
        "AWS_HAVE_EXECINFO",
    ],
})

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:windows": [
        "ws2_32.lib",
        "bcrypt.lib",
        "crypt32.lib",
        "iphlpapi.lib",
        "userenv.lib",
        "version.lib",
        "wininet.lib",
        "winmm.lib",
        "wldap32.lib",
        "ncrypt.lib",
        "secur32.lib",
        "shlwapi.lib",
    ],
    "@platforms//os:linux": [
        "-lpthread",
        "-lrt",
        "-ldl",
    ],
    "@platforms//os:osx": [
        "-framework Security",
        "-framework CoreFoundation",
    ],
    "//conditions:default": [],
})

write_file(
    name = "config_h_in",
    out = "config.h.in",
    content = [
        "#ifndef AWS_COMMON_CONFIG_H",
        "#define AWS_COMMON_CONFIG_H",
        "{{GCC_OVERFLOW_MATH}}",
        "{{GCC_INLINE_ASM}}",
        "{{MSVC_INTRINSICS_X64}}",
        "{{POSIX_LARGE_FILE_SUPPORT}}",
        "{{HAVE_EXECINFO}}",
        "{{WINAPI_DESKTOP}}",
        "{{LINUX_IF_LINK_H}}",
        "{{AVX2_INTRINSICS}}",
        "{{MM256_EXTRACT}}",
        "{{CLMUL}}",
        "{{ARM32_CRC}}",
        "{{ARMv8_1}}",
        "{{ARCH_ARM64}}",
        "{{ARCH_INTEL}}",
        "{{ARCH_INTEL_X64}}",
        "{{USE_CPU_EXTENSIONS}}",
        "",
        "#endif",
    ],
)

template_rule(
    name = "config_h",
    src = ":config_h_in",
    out = "crt/aws-c-common/generated/include/aws/common/config.h",
    substitutions = select({
        "@platforms//cpu:x86_64": {
            "{{ARM32_CRC}}": "/* #undef AWS_HAVE_ARM32_CRC */",
            "{{ARMv8_1}}": "/* #undef AWS_HAVE_ARMv8_1 */",
            "{{ARCH_ARM64}}": "/* #undef AWS_ARCH_ARM64 */",
            "{{ARCH_INTEL}}": "#define AWS_ARCH_INTEL",
            "{{ARCH_INTEL_X64}}": "#define AWS_ARCH_INTEL_X64",
        },
        "@platforms//cpu:aarch64": {
            "{{ARM32_CRC}}": "/* #undef AWS_HAVE_ARM32_CRC */",
            "{{ARMv8_1}}": "/* #undef AWS_HAVE_ARMv8_1 */",
            "{{ARCH_ARM64}}": "#define AWS_ARCH_ARM64",
            "{{ARCH_INTEL}}": "/* #undef AWS_ARCH_INTEL */",
            "{{ARCH_INTEL_X64}}": "/* #undef AWS_ARCH_INTEL_X64 */",
            "{{USE_CPU_EXTENSIONS}}": "/* #undef AWS_USE_CPU_EXTENSIONS */",
            "{{AVX2_INTRINSICS}}": "/* #undef AWS_HAVE_AVX2_INTRINSICS */",
            "{{MM256_EXTRACT}}": "/* #undef AWS_HAVE_MM256_EXTRACT_EPI64 */",
            "{{CLMUL}}": "/* #undef AWS_HAVE_CLMUL */",
        },
        "//conditions:default": {
            "{{ARM32_CRC}}": "/* #undef AWS_HAVE_ARM32_CRC */",
            "{{ARMv8_1}}": "/* #undef AWS_HAVE_ARMv8_1 */",
            "{{ARCH_ARM64}}": "/* #undef AWS_ARCH_ARM64 */",
            "{{ARCH_INTEL}}": "/* #undef AWS_ARCH_INTEL */",
            "{{ARCH_INTEL_X64}}": "/* #undef AWS_ARCH_INTEL_X64 */",
            "{{USE_CPU_EXTENSIONS}}": "/* #undef AWS_USE_CPU_EXTENSIONS */",
            "{{AVX2_INTRINSICS}}": "/* #undef AWS_HAVE_AVX2_INTRINSICS */",
            "{{MM256_EXTRACT}}": "/* #undef AWS_HAVE_MM256_EXTRACT_EPI64 */",
            "{{CLMUL}}": "/* #undef AWS_HAVE_CLMUL */",
        },
    }) | select({
        "@bazel_template//bazel:cpu_model_ryzen9": {
            "{{AVX2_INTRINSICS}}": "#define AWS_HAVE_AVX2_INTRINSICS",
            "{{MM256_EXTRACT}}": "#define AWS_HAVE_MM256_EXTRACT_EPI64",
            "{{CLMUL}}": "#define AWS_HAVE_CLMUL",
            "{{USE_CPU_EXTENSIONS}}": "#define AWS_USE_CPU_EXTENSIONS",
        },
        "@bazel_template//bazel:cpu_model_ryzen5": {
            "{{AVX2_INTRINSICS}}": "#define AWS_HAVE_AVX2_INTRINSICS",
            "{{MM256_EXTRACT}}": "#define AWS_HAVE_MM256_EXTRACT_EPI64",
            "{{CLMUL}}": "#define AWS_HAVE_CLMUL",
            "{{USE_CPU_EXTENSIONS}}": "#define AWS_USE_CPU_EXTENSIONS",
        },
        "@bazel_template//bazel:cpu_model_n105": {
            "{{AVX2_INTRINSICS}}": "/* #undef AWS_HAVE_AVX2_INTRINSICS */",
            "{{MM256_EXTRACT}}": "/* #undef AWS_HAVE_MM256_EXTRACT_EPI64 */",
            "{{CLMUL}}": "/* #undef AWS_HAVE_CLMUL */",
            "{{USE_CPU_EXTENSIONS}}": "/* #undef AWS_USE_CPU_EXTENSIONS */",
        },
        "@bazel_template//bazel:cpu_model_neoverse11": {
            "{{ARM32_CRC}}": "#define AWS_HAVE_ARM32_CRC",
            "{{ARMv8_1}}": "#define AWS_HAVE_ARMv8_1",
            "{{USE_CPU_EXTENSIONS}}": "#define AWS_USE_CPU_EXTENSIONS",
        },
        "//conditions:default": {
            "{{AVX2_INTRINSICS}}": "/* #undef AWS_HAVE_AVX2_INTRINSICS */",
            "{{MM256_EXTRACT}}": "/* #undef AWS_HAVE_MM256_EXTRACT_EPI64 */",
            "{{CLMUL}}": "/* #undef AWS_HAVE_CLMUL */",
            "{{USE_CPU_EXTENSIONS}}": "/* #undef AWS_USE_CPU_EXTENSIONS */",
        },
    }) | select({
        "@platforms//os:windows": {
            "{{GCC_OVERFLOW_MATH}}": "/* #undef AWS_HAVE_GCC_OVERFLOW_MATH_EXTENSIONS */",
            "{{GCC_INLINE_ASM}}": "/* #undef AWS_HAVE_GCC_INLINE_ASM */",
            "{{MSVC_INTRINSICS_X64}}": "#define AWS_HAVE_MSVC_INTRINSICS_X64",
            "{{POSIX_LARGE_FILE_SUPPORT}}": "/* #undef AWS_HAVE_POSIX_LARGE_FILE_SUPPORT */",
            "{{WINAPI_DESKTOP}}": "#define AWS_HAVE_WINAPI_DESKTOP",
            "{{LINUX_IF_LINK_H}}": "/* #undef AWS_HAVE_LINUX_IF_LINK_H */",
        },
        "@platforms//os:linux": {
            "{{GCC_OVERFLOW_MATH}}": "#define AWS_HAVE_GCC_OVERFLOW_MATH_EXTENSIONS",
            "{{GCC_INLINE_ASM}}": "#define AWS_HAVE_GCC_INLINE_ASM",
            "{{MSVC_INTRINSICS_X64}}": "/* #undef AWS_HAVE_MSVC_INTRINSICS_X64 */",
            "{{POSIX_LARGE_FILE_SUPPORT}}": "#define AWS_HAVE_POSIX_LARGE_FILE_SUPPORT",
            "{{WINAPI_DESKTOP}}": "/* #undef AWS_HAVE_WINAPI_DESKTOP */",
            "{{LINUX_IF_LINK_H}}": "#define AWS_HAVE_LINUX_IF_LINK_H",
        },
        "@platforms//os:osx": {
            "{{GCC_OVERFLOW_MATH}}": "#define AWS_HAVE_GCC_OVERFLOW_MATH_EXTENSIONS",
            "{{GCC_INLINE_ASM}}": "#define AWS_HAVE_GCC_INLINE_ASM",
            "{{MSVC_INTRINSICS_X64}}": "/* #undef AWS_HAVE_MSVC_INTRINSICS_X64 */",
            "{{POSIX_LARGE_FILE_SUPPORT}}": "#define AWS_HAVE_POSIX_LARGE_FILE_SUPPORT",
            "{{WINAPI_DESKTOP}}": "/* #undef AWS_HAVE_WINAPI_DESKTOP */",
            "{{LINUX_IF_LINK_H}}": "/* #undef AWS_HAVE_LINUX_IF_LINK_H */",
        },
        "//conditions:default": {
            "{{GCC_OVERFLOW_MATH}}": "#define AWS_HAVE_GCC_OVERFLOW_MATH_EXTENSIONS",
            "{{GCC_INLINE_ASM}}": "#define AWS_HAVE_GCC_INLINE_ASM",
            "{{MSVC_INTRINSICS_X64}}": "/* #undef AWS_HAVE_MSVC_INTRINSICS_X64 */",
            "{{POSIX_LARGE_FILE_SUPPORT}}": "#define AWS_HAVE_POSIX_LARGE_FILE_SUPPORT",
            "{{WINAPI_DESKTOP}}": "/* #undef AWS_HAVE_WINAPI_DESKTOP */",
            "{{LINUX_IF_LINK_H}}": "/* #undef AWS_HAVE_LINUX_IF_LINK_H */",
        },
    }) | select({
        "@bazel_template//bazel:libc_musl": {
            "{{HAVE_EXECINFO}}": "/* #undef AWS_HAVE_EXECINFO */",
        },
        "//conditions:default": {
            "{{HAVE_EXECINFO}}": "#define AWS_HAVE_EXECINFO",
        },
    }),
)

write_file(
    name = "Config_h",
    out = "crt/aws-crt-cpp/generated/include/aws/crt/Config.h",
    content = [
        "#pragma once",
        "#define AWS_CRT_CPP_VERSION \"0.32.8\"",
        "#define AWS_CRT_CPP_VERSION_MAJOR 0",
        "#define AWS_CRT_CPP_VERSION_MINOR 32",
        "#define AWS_CRT_CPP_VERSION_PATCH 8",
        "#define AWS_CRT_CPP_GIT_HASH \"05edb40f592813fab2b6f7ba141554fdcf86a7c9\"",
    ],
)

genrule(
    name = "SDKConfig_h_in",
    outs = ["src/aws-cpp-sdk-core/include/aws/core/SDKConfig.h.in"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "/**",
        " * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.",
        " * SPDX-License-Identifier: Apache-2.0.",
        " */",
        "",
        "/* #undef USE_AWS_MEMORY_MANAGEMENT */",
        "",
        "EOF",
    ]),
)

template_rule(
    name = "SDKConfig_h",
    src = ":SDKConfig_h_in",
    out = "src/aws-cpp-sdk-core/include/aws/core/SDKConfig.h",
    substitutions = select({
        "@platforms//os:linux": {
        },
        "@platforms//os:osx": {
        },
        "@platforms//os:windows": {
        },
        "//conditions:default": {},
    }),
)

genrule(
    name = "VersionConfig_h",
    outs = ["src/aws-cpp-sdk-core/include/aws/core/VersionConfig.h"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "/**",
        " * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.",
        " * SPDX-License-Identifier: Apache-2.0.",
        " */",
        "#pragma once",
        "",
        "#define AWS_SDK_VERSION_STRING \"1.11.853\"",
        "#define AWS_SDK_VERSION_MAJOR 1",
        "#define AWS_SDK_VERSION_MINOR 11",
        "#define AWS_SDK_VERSION_PATCH 853",
        "EOF",
    ]),
)

# AWS Common Runtime C library (from submodule)
cc_library(
    name = "aws-crt-c",
    srcs = optional_glob(
        [
            "crt/aws-crt-cpp/crt/aws-c-auth/source/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-compression/source/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-event-stream/source/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-http/source/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-io/source/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-mqtt/source/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-s3/source/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-sdkutils/source/**/*.c",
            "crt/aws-crt-cpp/crt/aws-checksums/source/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/api/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/crypto/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/error/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/stuffer/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/tls/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/utils/**/*.c",
            #"crt/aws-crt-cpp/crt/aws-lc/crypto/**/*.c",
        ],
        allow_empty = True,
        exclude = [
            "crt/aws-crt-cpp/crt/aws-c-io/source/windows/**",
            "crt/aws-crt-cpp/crt/aws-c-io/source/linux/**",
            "crt/aws-crt-cpp/crt/aws-c-io/source/darwin/**",
            "crt/aws-crt-cpp/crt/aws-c-io/source/posix/**",
            "crt/aws-crt-cpp/crt/aws-c-io/source/s2n/**",
            "crt/aws-crt-cpp/crt/aws-c-common/source/windows/**",
            "crt/aws-crt-cpp/crt/aws-c-common/source/linux/**",
            "crt/aws-crt-cpp/crt/aws-c-common/source/darwin/**",
            "crt/aws-crt-cpp/crt/aws-c-common/source/posix/**",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/windows/**",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/linux/**",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/darwin/**",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/posix/**",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/unix/**",
            "crt/aws-crt-cpp/crt/aws-checksums/source/arm/**",
            "crt/aws-crt-cpp/crt/aws-checksums/source/intel/**",
            "crt/aws-crt-cpp/crt/**/tests/**",
            "crt/aws-crt-cpp/crt/aws-c-io/source/bsd/**",
            "crt/aws-crt-cpp/crt/aws-c-common/source/android/**",
            "crt/aws-crt-cpp/crt/aws-c-common/source/arch/**",
            "crt/aws-crt-cpp/crt/aws-c-common/source/external/**",
            "crt/aws-crt-cpp/crt/aws-lc/crypto/thread_pthread.c",
            "crt/aws-crt-cpp/crt/aws-lc/crypto/thread_win.c",
            "crt/aws-crt-cpp/crt/aws-lc/crypto/refcount_win.c",
            "crt/aws-crt-cpp/crt/aws-lc/crypto/kyber/pqcrystals_kyber_ref_common/cbd.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/shared/ed25519_noop.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/platform_fallback_stubs/**",
            "crt/aws-crt-cpp/crt/aws-c-io/source/s2n/s2n_apple_keychain.c",
            "crt/aws-crt-cpp/crt/s2n/**",
        ],
    ) + select({
        "@platforms//os:windows": optional_glob([
            "crt/aws-crt-cpp/crt/aws-c-io/source/windows/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/windows/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/platform_fallback_stubs/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/windows/**/*.c",
        ]) + [
            #"crt/aws-crt-cpp/crt/aws-lc/crypto/refcount_win.c",
            #"crt/aws-crt-cpp/crt/aws-lc/crypto/thread_win.c",
        ],
        "@platforms//os:linux": optional_glob([
            "crt/aws-crt-cpp/crt/s2n/api/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/crypto/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/error/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/stuffer/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/tls/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/utils/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-io/source/s2n/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-io/source/linux/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/linux/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-io/source/posix/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/posix/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/linux/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/posix/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/unix/**/*.c",
        ]) + [
            #"crt/aws-crt-cpp/crt/aws-lc/crypto/thread_pthread.c",
        ],
        "@platforms//os:osx": optional_glob([
            "crt/aws-crt-cpp/crt/s2n/api/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/crypto/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/error/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/stuffer/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/tls/**/*.c",
            "crt/aws-crt-cpp/crt/s2n/utils/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-io/source/s2n/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-io/source/bsd/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-io/source/darwin/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/darwin/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-io/source/posix/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/posix/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/darwin/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/posix/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/unix/**/*.c",
        ]) + [
            "crt/aws-crt-cpp/crt/aws-c-common/source/platform_fallback_stubs/file_direct_io.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/platform_fallback_stubs/system_info.c",
            #"crt/aws-crt-cpp/crt/aws-lc/crypto/thread_pthread.c",
        ],
        "//conditions:default": [],
    }) + select({
        # Base architecture-specific source files (only generic for x86_64, excluding cpuid)
        "@platforms//cpu:x86_64": optional_glob(
            [
                "crt/aws-crt-cpp/crt/aws-checksums/source/generic/**/*.c",
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/generic/**/*.c",
            ],
            exclude = [
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/generic/cpuid.c",  # Conditionally added below
            ],
        ),
        "@platforms//cpu:aarch64": optional_glob(
            [
                "crt/aws-crt-cpp/crt/aws-checksums/source/generic/**/*.c",
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/arm/**/*.c",
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/generic/**/*.c",
            ],
            exclude = [
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/arm/darwin/**",
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/arm/linux/**",
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/arm/windows/**",
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/generic/cpuid.c",
            ],
        ),
        "//conditions:default": [],
    }) + select({
        # CPU tier-specific sources: Intel optimizations for medium/large tiers
        "@bazel_template//bazel:cpu_model_ryzen9": optional_glob(
            [
                "crt/aws-crt-cpp/crt/aws-checksums/source/intel/**/*.c",
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/intel/**/*.c",
            ],
            exclude = [
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/intel/msvc/**",
                "crt/aws-crt-cpp/crt/aws-checksums/source/intel/cpuid.c",
            ],
        ),
        "@bazel_template//bazel:cpu_model_ryzen5": optional_glob(
            [
                "crt/aws-crt-cpp/crt/aws-checksums/source/intel/**/*.c",
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/intel/**/*.c",
            ],
            exclude = [
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/intel/msvc/**",
                "crt/aws-crt-cpp/crt/aws-checksums/source/intel/cpuid.c",
                "crt/aws-crt-cpp/crt/aws-c-common/source/arch/intel/encoding_avx2.c",
                "crt/aws-crt-cpp/crt/aws-checksums/source/intel/crc32c_sse42_avx512.c",
            ],
        ),
        "@bazel_template//bazel:cpu_model_n105": [
            "crt/aws-crt-cpp/crt/aws-c-common/source/arch/generic/cpuid.c",
            "@bazel_template//bazel:aws_cpu_stubs_small.c",
        ],
        "@bazel_template//bazel:cpu_model_neoverse11": [
            # Use generic cpuid for ARM as this submodule layout lacks arch/arm/cpuid.c
            "crt/aws-crt-cpp/crt/aws-c-common/source/arch/generic/cpuid.c",
            "@bazel_template//bazel:aws_cpu_stubs_small.c",
        ] + optional_glob([
            "crt/aws-crt-cpp/crt/aws-checksums/source/arm/**/*.c",
        ]),
        "//conditions:default": [
            "crt/aws-crt-cpp/crt/aws-c-common/source/arch/generic/cpuid.c",
            "@bazel_template//bazel:aws_cpu_stubs_small.c",
        ],
    }),
    hdrs = [
        ":Config_h",
        ":config_h",
        #"crt/aws-crt-cpp/crt/aws-lc/crypto/kyber/pqcrystals_kyber_ref_common/cbd.c",
        "crt/aws-crt-cpp/crt/aws-c-http/include/aws/http/private/hpack_header_static_table.def",
        "crt/aws-crt-cpp/crt/aws-c-http/include/aws/http/private/hpack_huffman_static_table.def",
        "crt/aws-crt-cpp/crt/aws-c-io/source/pkcs11_private.h",
        "crt/aws-crt-cpp/crt/aws-c-io/source/pkcs11/v2.40/pkcs11.h",
    ] + optional_glob(
        [
            "crt/aws-crt-cpp/include/**/*.h",
            "crt/aws-crt-cpp/include/**/*.hpp",
            "crt/aws-crt-cpp/crt/aws-c-auth/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/include/**/*.inl",
            "crt/aws-crt-cpp/crt/aws-c-common/source/external/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-compression/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-event-stream/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-http/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-io/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-io/source/s2n/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-mqtt/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-s3/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-sdkutils/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-checksums/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-checksums/source/external/**/*.h",
            #"crt/aws-crt-cpp/crt/aws-lc/include/**/*.h",
            #"crt/aws-crt-cpp/crt/aws-lc/crypto/**/*.h",
            "crt/aws-crt-cpp/crt/s2n/api/**/*.h",
            "crt/aws-crt-cpp/crt/s2n/crypto/**/*.h",
            "crt/aws-crt-cpp/crt/s2n/error/**/*.h",
            "crt/aws-crt-cpp/crt/s2n/stuffer/**/*.h",
            "crt/aws-crt-cpp/crt/s2n/tls/**/*.h",
            "crt/aws-crt-cpp/crt/s2n/utils/**/*.h",
        ],
        exclude = [
            "crt/aws-crt-cpp/crt/aws-c-io/include/windows/**",
            "crt/aws-crt-cpp/crt/aws-c-io/include/linux/**",
            "crt/aws-crt-cpp/crt/aws-c-io/include/darwin/**",
            "crt/aws-crt-cpp/crt/aws-c-io/include/posix/**",
            "crt/aws-crt-cpp/crt/aws-c-common/include/windows/**",
            "crt/aws-crt-cpp/crt/aws-c-common/include/linux/**",
            "crt/aws-crt-cpp/crt/aws-c-common/include/darwin/**",
            "crt/aws-crt-cpp/crt/aws-c-common/include/posix/**",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/windows/**",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/linux/**",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/darwin/**",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/posix/**",
            "crt/aws-crt-cpp/crt/**/tests/**",
        ],
    ) + select({
        "@platforms//os:windows": optional_glob([
            "crt/aws-crt-cpp/crt/aws-c-io/include/windows/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-io/source/windows/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/include/windows/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/source/windows/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/windows/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/windows/**/*.h",
        ]),
        "@platforms//os:linux": optional_glob([
            "crt/aws-crt-cpp/crt/aws-c-io/include/linux/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/include/linux/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-io/include/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/include/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/linux/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-io/source/linux/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-io/source/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/source/linux/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/source/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/linux/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/posix/**/*.h",
        ]),
        "@platforms//os:osx": optional_glob([
            "crt/aws-crt-cpp/crt/aws-c-io/include/darwin/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/include/darwin/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-io/include/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/include/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/darwin/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-io/source/darwin/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-io/source/bsd/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-io/source/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/source/darwin/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/source/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/darwin/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/posix/**/*.h",
        ]),
        "//conditions:default": [],
    }),
    copts = AWS_CRT_COPTS,
    includes = [
        "crt/aws-c-common/generated/include",
        "crt/aws-crt-cpp/crt",
        "crt/aws-crt-cpp/crt/aws-c-auth/include",
        "crt/aws-crt-cpp/crt/aws-c-cal/include",
        "crt/aws-crt-cpp/crt/aws-c-common/include",
        "crt/aws-crt-cpp/crt/aws-c-common/source",
        "crt/aws-crt-cpp/crt/aws-c-common/source/external/libcbor",
        "crt/aws-crt-cpp/crt/aws-c-compression/include",
        "crt/aws-crt-cpp/crt/aws-c-event-stream/include",
        "crt/aws-crt-cpp/crt/aws-c-http/include",
        "crt/aws-crt-cpp/crt/aws-c-io/include",
        "crt/aws-crt-cpp/crt/aws-c-io/source/s2n",
        "crt/aws-crt-cpp/crt/aws-c-mqtt/include",
        "crt/aws-crt-cpp/crt/aws-c-s3/include",
        "crt/aws-crt-cpp/crt/aws-c-sdkutils/include",
        "crt/aws-crt-cpp/crt/aws-checksums/include",
        "crt/aws-crt-cpp/crt/s2n",
        "crt/aws-crt-cpp/crt/s2n/api",
        "crt/aws-crt-cpp/crt/s2n/utils",
        "crt/aws-crt-cpp/generated/include",
        "crt/aws-crt-cpp/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@cjson",
        "@curl",
        "@libcbor",
        "@openssl",
        "@tinyxml2",
        "@zlib",
    ],
)

# AWS Common Runtime C++ library (from submodule)
cc_library(
    name = "aws-crt-cpp",
    srcs = optional_glob(
        [
            "crt/aws-crt-cpp/source/**/*.cpp",
        ],
    ),
    hdrs = [
        ":Config_h",
        ":config_h",
    ] + optional_glob(
        [
            "crt/aws-crt-cpp/include/**/*.h",
            "crt/aws-crt-cpp/include/**/*.hpp",
        ],
    ),
    copts = COPTS,
    includes = [
        "crt/aws-c-common/generated/include",
        "crt/aws-crt-cpp/generated/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-crt-c",
    ],
)

# Core AWS SDK library
cc_library(
    name = "aws-cpp-sdk-core",
    srcs = optional_glob(
        [
            "src/aws-cpp-sdk-core/source/**/*.cpp",
        ],
        exclude = [
            "src/aws-cpp-sdk-core/source/platform/**",
            #"src/aws-cpp-sdk-core/source/external/**",
            "src/aws-cpp-sdk-core/source/smithy/tracing/impl/opentelemetry/**",
            "src/aws-cpp-sdk-core/source/net/windows/**",
            "src/aws-cpp-sdk-core/source/http/windows/**",
            "src/aws-cpp-sdk-core/source/net/linux-shared/**",
            "src/aws-cpp-sdk-core/source/net/SimpleUDP.cpp",
            "src/aws-cpp-sdk-core/source/net/Net.cpp",
        ],
    ) + select({
        "@platforms//os:windows": optional_glob(
            [
                "src/aws-cpp-sdk-core/source/http/windows/**/*.cpp",
                "src/aws-cpp-sdk-core/source/net/windows/**/*.cpp",
                "src/aws-cpp-sdk-core/source/platform/windows/**/*.cpp",
            ],
            exclude = [
                "src/aws-cpp-sdk-core/source/http/windows/IXmlHttpRequest2HttpClient.cpp",
            ],
        ),
        "@platforms//os:linux": optional_glob([
            "src/aws-cpp-sdk-core/source/net/linux-shared/**/*.cpp",
            "src/aws-cpp-sdk-core/source/platform/linux-shared/**/*.cpp",
        ]),
        "@platforms//os:osx": optional_glob([
            "src/aws-cpp-sdk-core/source/net/linux-shared/**/*.cpp",
            "src/aws-cpp-sdk-core/source/platform/linux-shared/**/*.cpp",
        ]),
        "//conditions:default": [],
    }),
    hdrs = optional_glob(
        [
            "src/aws-cpp-sdk-core/include/**/*.h",
            "src/aws-cpp-sdk-core/include/**/*.hpp",
        ],
        exclude = [
            "src/aws-cpp-sdk-core/include/platform/**",
            "src/aws-cpp-sdk-core/include/smithy/tracing/impl/opentelemetry/**",
        ],
    ) + select({
        "@platforms//os:windows": optional_glob([
            "src/aws-cpp-sdk-core/include/platform/windows/**/*.h",
        ]),
        "@platforms//os:linux": optional_glob([
            "src/aws-cpp-sdk-core/include/platform/linux/**/*.h",
            "src/aws-cpp-sdk-core/include/platform/posix/**/*.h",
        ]),
        "@platforms//os:osx": optional_glob([
            "src/aws-cpp-sdk-core/include/platform/darwin/**/*.h",
            "src/aws-cpp-sdk-core/include/platform/posix/**/*.h",
        ]),
        "//conditions:default": [],
    }) + [
        ":SDKConfig_h",
        ":VersionConfig_h",
        ":config_h",
    ],
    copts = COPTS + [
        "-I$(GENDIR)/external/aws-sdk-cpp/crt/aws-c-common/generated/include",
        "-I$(GENDIR)/external/aws-sdk-cpp/crt/aws-crt-cpp/generated/include",
    ],
    includes = [
        "crt/aws-c-common/generated/include",
        "crt/aws-crt-cpp/crt/aws-c-cal/include",
        "crt/aws-crt-cpp/crt/aws-c-common/include",
        "crt/aws-crt-cpp/crt/aws-c-io/include",
        "crt/aws-crt-cpp/generated/include",
        "crt/aws-crt-cpp/include",
        "include",
        "src/aws-cpp-sdk-core/include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-crt-cpp",
        "@curl",
        "@openssl",
        "@zlib",
    ],
)

# Access Management library
cc_library(
    name = "aws-cpp-sdk-access-management",
    srcs = optional_glob([
        "src/aws-cpp-sdk-access-management/source/**/*.cpp",
    ]),
    hdrs = optional_glob([
        "src/aws-cpp-sdk-access-management/include/**/*.h",
        "src/aws-cpp-sdk-access-management/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "include",
        "src/aws-cpp-sdk-access-management/include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-cognito-identity",
        ":aws-cpp-sdk-core",
        ":aws-cpp-sdk-iam",
    ],
)

# Identity Management library
cc_library(
    name = "aws-cpp-sdk-identity-management",
    srcs = optional_glob([
        "src/aws-cpp-sdk-identity-management/source/**/*.cpp",
    ]),
    hdrs = optional_glob([
        "src/aws-cpp-sdk-identity-management/include/**/*.h",
        "src/aws-cpp-sdk-identity-management/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "include",
        "src/aws-cpp-sdk-identity-management/include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-cognito-identity",
        ":aws-cpp-sdk-core",
        ":aws-cpp-sdk-sts",
    ],
)

# EC2 library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-ec2",
    srcs = optional_glob([
        "generated/src/aws-cpp-sdk-ec2/source/**/*.cpp",
    ]),
    hdrs = optional_glob([
        "generated/src/aws-cpp-sdk-ec2/include/**/*.h",
        "generated/src/aws-cpp-sdk-ec2/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-ec2/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# IAM library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-iam",
    srcs = optional_glob([
        "generated/src/aws-cpp-sdk-iam/source/**/*.cpp",
    ]),
    hdrs = optional_glob([
        "generated/src/aws-cpp-sdk-iam/include/**/*.h",
        "generated/src/aws-cpp-sdk-iam/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-iam/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# ELB library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-elasticloadbalancing",
    srcs = optional_glob([
        "generated/src/aws-cpp-sdk-elasticloadbalancing/source/**/*.cpp",
    ]),
    hdrs = optional_glob([
        "generated/src/aws-cpp-sdk-elasticloadbalancing/include/**/*.h",
        "generated/src/aws-cpp-sdk-elasticloadbalancing/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-elasticloadbalancing/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# Route53 library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-route53",
    srcs = optional_glob([
        "generated/src/aws-cpp-sdk-route53/source/**/*.cpp",
    ]),
    hdrs = optional_glob([
        "generated/src/aws-cpp-sdk-route53/include/**/*.h",
        "generated/src/aws-cpp-sdk-route53/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-route53/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# Systems Manager library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-ssm",
    srcs = optional_glob([
        "generated/src/aws-cpp-sdk-ssm/source/**/*.cpp",
    ]),
    hdrs = optional_glob([
        "generated/src/aws-cpp-sdk-ssm/include/**/*.h",
        "generated/src/aws-cpp-sdk-ssm/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-ssm/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# Secrets Manager library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-secretsmanager",
    srcs = optional_glob([
        "generated/src/aws-cpp-sdk-secretsmanager/source/**/*.cpp",
    ]),
    hdrs = optional_glob([
        "generated/src/aws-cpp-sdk-secretsmanager/include/**/*.h",
        "generated/src/aws-cpp-sdk-secretsmanager/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-secretsmanager/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# Cognito Identity library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-cognito-identity",
    srcs = optional_glob([
        "generated/src/aws-cpp-sdk-cognito-identity/source/**/*.cpp",
    ]),
    hdrs = optional_glob([
        "generated/src/aws-cpp-sdk-cognito-identity/include/**/*.h",
        "generated/src/aws-cpp-sdk-cognito-identity/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-cognito-identity/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# STS (Security Token Service) library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-sts",
    srcs = optional_glob([
        "generated/src/aws-cpp-sdk-sts/source/**/*.cpp",
    ]),
    hdrs = optional_glob([
        "generated/src/aws-cpp-sdk-sts/include/**/*.h",
        "generated/src/aws-cpp-sdk-sts/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-sts/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)
