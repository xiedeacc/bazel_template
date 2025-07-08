load("@bazel_skylib//lib:selects.bzl", "selects")
load("@bazel_skylib//rules:write_file.bzl", "write_file")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES", "template_rule")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [
        "/std:c++17",
        "/Iexternal/aws-sdk-cpp/src",
        "/Iexternal/aws-sdk-cpp/generated/src",
        "/Iexternal/aws-sdk-cpp/include",
        "/Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/include",
    ],
    "//conditions:default": [
        "-std=c++17",
        "-isystem external/aws-sdk-cpp/src",
        "-isystem external/aws-sdk-cpp/generated/src",
        "-isystem external/aws-sdk-cpp/include",
        "-isystem external/aws-sdk-cpp/crt/aws-crt-cpp/include",
    ],
}) + select({
    "@platforms//os:linux": [
        "-fPIC",
        "-D_GNU_SOURCE",
    ],
    "@platforms//os:osx": [
        "-fPIC",
    ],
    "@platforms//os:windows": [],
    "//conditions:default": [],
}) + select({
    "@bazel_template//bazel:linux_x86_64": [
        "-mavx2",
        "-msse4.2",
        "-march=x86-64",
        "-mtune=generic",
        "-msse4.1",
        "-mpclmul",
    ],
    "@bazel_template//bazel:windows_x86_64": [
        "/arch:AVX2",
    ],
    "@bazel_template//bazel:osx_x86_64": [
        "-mavx2",
        "-msse4.2",
        "-march=x86-64",
        "-mtune=generic",
    ],
    "//conditions:default": [],
}) + [
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
]

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + [
    "AWS_SDK_VERSION_MAJOR=1",
    "AWS_SDK_VERSION_MINOR=11",
    "AWS_SDK_VERSION_PATCH=602",
    "AWS_SDK_VERSION_STRING=\"1.11.602\"",
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
] + select({
    "@bazel_template//bazel:linux_x86_64": [
        "AWS_HAVE_AVX2_INTRINSICS",
        #"AWS_HAVE_AVX512_INTRINSICS",
        "AWS_HAVE_MM256_EXTRACT_EPI64",
        "AWS_HAVE_CLMUL",
        "AWS_ARCH_INTEL",
        "AWS_ARCH_INTEL_X64",
        "AWS_USE_CPU_EXTENSIONS",
    ],
    "@bazel_template//bazel:windows_x86_64": [
        "AWS_HAVE_AVX2_INTRINSICS",
        #"AWS_HAVE_AVX512_INTRINSICS",
        "AWS_HAVE_MM256_EXTRACT_EPI64",
        "AWS_HAVE_CLMUL",
        "AWS_ARCH_INTEL",
        "AWS_ARCH_INTEL_X64",
        "AWS_USE_CPU_EXTENSIONS",
    ],
    "@bazel_template//bazel:osx_x86_64": [
        "AWS_HAVE_AVX2_INTRINSICS",
        #"AWS_HAVE_AVX512_INTRINSICS",
        "AWS_HAVE_MM256_EXTRACT_EPI64",
        "AWS_HAVE_CLMUL",
        "AWS_ARCH_INTEL",
        "AWS_ARCH_INTEL_X64",
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
        "AWS_HAVE_WINAPI_DESKTOP",
    ],
    "@platforms//os:linux": [
        "_GNU_SOURCE",
        "AWS_HAVE_POSIX_LARGE_FILE_SUPPORT",
        "AWS_HAVE_EXECINFO",
        "AWS_HAVE_LINUX_IF_LINK_H",
        "AWS_AFFINITY_METHOD=AWS_AFFINITY_METHOD_PTHREAD_ATTR",
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
    ],
    "//conditions:default": [],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
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
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

write_file(
    name = "config_h_in",
    out = "config.h.in",
    content = [
        "#ifndef AWS_COMMON_CONFIG_H",
        "#define AWS_COMMON_CONFIG_H",
        "#define AWS_HAVE_GCC_OVERFLOW_MATH_EXTENSIONS",
        "#define AWS_HAVE_GCC_INLINE_ASM",
        "/* #undef AWS_HAVE_MSVC_INTRINSICS_X64 */",
        "#define AWS_HAVE_POSIX_LARGE_FILE_SUPPORT",
        "#define AWS_HAVE_EXECINFO",
        "/* #undef AWS_HAVE_WINAPI_DESKTOP */",
        "#define AWS_HAVE_LINUX_IF_LINK_H",
        "#define AWS_HAVE_AVX2_INTRINSICS",
        #"#define AWS_HAVE_AVX512_INTRINSICS",
        "#define AWS_HAVE_MM256_EXTRACT_EPI64",
        "#define AWS_HAVE_CLMUL",
        "/* #undef AWS_HAVE_ARM32_CRC */",
        "/* #undef AWS_HAVE_ARMv8_1 */",
        "/* #undef AWS_ARCH_ARM64 */",
        "#define AWS_ARCH_INTEL",
        "#define AWS_ARCH_INTEL_X64",
        "#define AWS_USE_CPU_EXTENSIONS",
        "",
        "#endif",
    ],
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

template_rule(
    name = "config_h",
    src = ":config_h_in",
    out = "crt/aws-c-common/generated/include/aws/common/config.h",
    substitutions = select({
        "@bazel_template//bazel:linux_aarch64": {
        },
        "//conditions:default": {},
    }) | select({
        "@platforms//os:linux": {},
        "@platforms//os:osx": {},
        "@platforms//os:windows": {},
    }),
)

# AWS Common Runtime C++ library (from submodule)
cc_library(
    name = "aws-crt-cpp",
    srcs = glob(
        [
            "crt/aws-crt-cpp/source/**/*.cpp",
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
            #"crt/aws-crt-cpp/crt/aws-lc/crypto/**/*.c",
            #"crt/aws-crt-cpp/crt/s2n/**/*.c",
        ],
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
        ],
    ) + select({
        "@platforms//os:windows": glob([
            "crt/aws-crt-cpp/crt/aws-c-io/source/windows/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/windows/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/windows/**/*.c",
        ]) + [
            #"crt/aws-crt-cpp/crt/aws-lc/crypto/refcount_win.c",
            #"crt/aws-crt-cpp/crt/aws-lc/crypto/thread_win.c",
        ],
        "@platforms//os:linux": glob([
            "crt/aws-crt-cpp/crt/aws-c-io/source/linux/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/linux/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-io/source/posix/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/posix/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/linux/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/posix/**/*.c",
        ]) + [
            #"crt/aws-crt-cpp/crt/aws-lc/crypto/thread_pthread.c",
        ],
        "@platforms//os:osx": glob([
            "crt/aws-crt-cpp/crt/aws-c-io/source/darwin/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/darwin/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-io/source/posix/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/posix/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/darwin/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-cal/source/posix/**/*.c",
        ]) + [
            #"crt/aws-crt-cpp/crt/aws-lc/crypto/thread_pthread.c",
        ],
        "//conditions:default": [],
    }) + select({
        "@bazel_template//bazel:windows_x86_64": glob([
            "crt/aws-crt-cpp/crt/aws-c-common/source/arch/generic/**/*.c",
            #"crt/aws-crt-cpp/crt/aws-c-common/source/arch/intel/msvc/*.c",
            #"crt/aws-crt-cpp/crt/aws-c-common/source/arch/intel/cpuid.c",
            #"crt/aws-crt-cpp/crt/aws-c-common/source/arch/intel/encoding_avx2.c",
        ]),
        "@bazel_template//bazel:windows_aarch64": glob([
            "crt/aws-crt-cpp/crt/aws-c-common/source/arch/arm/**/*.c",
        ]),
        "@bazel_template//bazel:linux_x86_64": glob([
            "crt/aws-crt-cpp/crt/aws-checksums/source/generic/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/arch/generic/**/*.c",
        ]),
        "@bazel_template//bazel:linux_aarch64": glob([
            "crt/aws-crt-cpp/crt/aws-checksums/source/arm/**/*.c",
            "crt/aws-crt-cpp/crt/aws-c-common/source/arch/arm/**/*.c",
        ]),
        "@bazel_template//bazel:osx_x86_64": glob([
            "crt/aws-crt-cpp/crt/aws-c-common/source/arch/intel/**/*.c",
        ]),
        "@bazel_template//bazel:osx_aarch64": glob([
            "crt/aws-crt-cpp/crt/aws-c-common/source/arch/arm/**/*.c",
        ]),
        "//conditions:default": [],
    }),
    hdrs = [
        ":Config_h",
        ":config_h",
        #"crt/aws-crt-cpp/crt/aws-lc/crypto/kyber/pqcrystals_kyber_ref_common/cbd.c",
        "crt/aws-crt-cpp/crt/aws-c-http/include/aws/http/private/hpack_header_static_table.def",
        "crt/aws-crt-cpp/crt/aws-c-http/include/aws/http/private/hpack_huffman_static_table.def",
        "crt/aws-crt-cpp/crt/aws-c-io/source/pkcs11_private.h",
        "crt/aws-crt-cpp/crt/aws-c-io/source/pkcs11/v2.40/pkcs11.h",
    ] + glob(
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
            "crt/aws-crt-cpp/crt/aws-c-mqtt/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-s3/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-sdkutils/include/**/*.h",
            "crt/aws-crt-cpp/crt/aws-checksums/include/**/*.h",
            #"crt/aws-crt-cpp/crt/aws-lc/include/**/*.h",
            #"crt/aws-crt-cpp/crt/aws-lc/crypto/**/*.h",
            #"crt/aws-crt-cpp/crt/s2n/include/**/*.h",
            #"crt/aws-crt-cpp/crt/s2n/**/*.h",
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
        "@platforms//os:windows": glob([
            "crt/aws-crt-cpp/crt/aws-c-io/include/windows/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/include/windows/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/windows/**/*.h",
        ]),
        "@platforms//os:linux": glob([
            "crt/aws-crt-cpp/crt/aws-c-io/include/linux/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/include/linux/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-io/include/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/include/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/linux/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/posix/**/*.h",
        ]),
        "@platforms//os:osx": glob([
            "crt/aws-crt-cpp/crt/aws-c-io/include/darwin/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/include/darwin/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-io/include/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-common/include/posix/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/darwin/**/*.h",
            "crt/aws-crt-cpp/crt/aws-c-cal/include/posix/**/*.h",
        ]),
        "//conditions:default": [],
    }),
    copts = COPTS + [
    ],
    includes = [
        "crt/aws-crt-cpp/crt/aws-c-common/include",
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

# Core AWS SDK library
cc_library(
    name = "aws-cpp-sdk-core",
    srcs = glob(
        [
            "src/aws-cpp-sdk-core/source/**/*.cpp",
        ],
        exclude = [
            "src/aws-cpp-sdk-core/source/platform/**",
            "src/aws-cpp-sdk-core/source/external/**",
            "src/aws-cpp-sdk-core/source/smithy/tracing/impl/opentelemetry/**",
            "src/aws-cpp-sdk-core/source/net/windows/**",
            "src/aws-cpp-sdk-core/source/http/windows/**",
            "src/aws-cpp-sdk-core/source/net/linux-shared/**",
            "src/aws-cpp-sdk-core/source/net/SimpleUDP.cpp",
            "src/aws-cpp-sdk-core/source/net/Net.cpp",
        ],
    ) + select({
        "@platforms//os:windows": glob([
            "src/aws-cpp-sdk-core/source/platform/windows/**/*.cpp",
        ]),
        "@platforms//os:linux": glob([
            "src/aws-cpp-sdk-core/source/net/linux-shared/**/*.cpp",
            "src/aws-cpp-sdk-core/source/platform/linux/**/*.cpp",
            "src/aws-cpp-sdk-core/source/platform/posix/**/*.cpp",
        ]),
        "@platforms//os:osx": glob([
            "src/aws-cpp-sdk-core/source/platform/darwin/**/*.cpp",
            "src/aws-cpp-sdk-core/source/platform/posix/**/*.cpp",
        ]),
        "//conditions:default": [],
    }),
    hdrs = glob(
        [
            "src/aws-cpp-sdk-core/include/**/*.h",
            "src/aws-cpp-sdk-core/include/**/*.hpp",
        ],
        exclude = [
            "src/aws-cpp-sdk-core/include/platform/**",
            "src/aws-cpp-sdk-core/include/smithy/tracing/impl/opentelemetry/**",
        ],
    ) + select({
        "@platforms//os:windows": glob([
            "src/aws-cpp-sdk-core/include/platform/windows/**/*.h",
        ]),
        "@platforms//os:linux": glob([
            "src/aws-cpp-sdk-core/include/platform/linux/**/*.h",
            "src/aws-cpp-sdk-core/include/platform/posix/**/*.h",
        ]),
        "@platforms//os:osx": glob([
            "src/aws-cpp-sdk-core/include/platform/darwin/**/*.h",
            "src/aws-cpp-sdk-core/include/platform/posix/**/*.h",
        ]),
        "//conditions:default": [],
    }),
    copts = COPTS,
    includes = [
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
    srcs = glob([
        "src/aws-cpp-sdk-access-management/source/**/*.cpp",
    ]),
    hdrs = glob([
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
        ":aws-cpp-sdk-core",
    ],
)

# Identity Management library
cc_library(
    name = "aws-cpp-sdk-identity-management",
    srcs = glob([
        "src/aws-cpp-sdk-identity-management/source/**/*.cpp",
    ]),
    hdrs = glob([
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
        ":aws-cpp-sdk-core",
    ],
)

# EC2 library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-ec2",
    srcs = glob([
        "generated/src/aws-cpp-sdk-ec2/source/**/*.cpp",
    ]),
    hdrs = glob([
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
    srcs = glob([
        "generated/src/aws-cpp-sdk-iam/source/**/*.cpp",
    ]),
    hdrs = glob([
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
    srcs = glob([
        "generated/src/aws-cpp-sdk-elasticloadbalancing/source/**/*.cpp",
    ]),
    hdrs = glob([
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
    srcs = glob([
        "generated/src/aws-cpp-sdk-route53/source/**/*.cpp",
    ]),
    hdrs = glob([
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
    srcs = glob([
        "generated/src/aws-cpp-sdk-ssm/source/**/*.cpp",
    ]),
    hdrs = glob([
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
    srcs = glob([
        "generated/src/aws-cpp-sdk-secretsmanager/source/**/*.cpp",
    ]),
    hdrs = glob([
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