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
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + [
    "AWS_SDK_VERSION_MAJOR=1",
    "AWS_SDK_VERSION_MINOR=11",
    "AWS_SDK_VERSION_PATCH=602",
    "AWS_SDK_VERSION_STRING=\"1.11.602\"",
    "AWS_APPSTORE_SAFE=ON",
    "ENABLE_TESTING=OFF",
] + select({
    "@platforms//os:windows": [
        "WIN32",
        "_WIN32",
        "UNICODE",
        "_UNICODE",
        "NOMINMAX",
        "WIN32_LEAN_AND_MEAN",
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
        "#define AWS_HAVE_AVX512_INTRINSICS",
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
    srcs = glob([
        "crt/aws-crt-cpp/source/**/*.cpp",
        "crt/aws-crt-cpp/crt/aws-c-auth/source",
        "crt/aws-crt-cpp/crt/aws-c-cal/source",
        "crt/aws-crt-cpp/crt/aws-c-common/source",
        "crt/aws-crt-cpp/crt/aws-c-compression/source",
        "crt/aws-crt-cpp/crt/aws-c-event-stream/source",
        "crt/aws-crt-cpp/crt/aws-c-http/source",
        "crt/aws-crt-cpp/crt/aws-c-io/source",
        "crt/aws-crt-cpp/crt/aws-c-mqtt/source",
        "crt/aws-crt-cpp/crt/aws-c-s3/source",
        "crt/aws-crt-cpp/crt/aws-c-sdkutils/source",
        "crt/aws-crt-cpp/crt/aws-checksums/source",
        "crt/aws-crt-cpp/crt/aws-lc/source",
        "crt/aws-crt-cpp/crt/s2n/source",
    ]),
    hdrs = [":config_h"] + glob([
        "crt/aws-crt-cpp/include/**/*.h",
        "crt/aws-crt-cpp/include/**/*.hpp",
        "crt/aws-crt-cpp/crt/aws-c-auth/include/**/*.h",
        "crt/aws-crt-cpp/crt/aws-c-cal/include/**/*.h",
        "crt/aws-crt-cpp/crt/aws-c-common/include/**/*.h",
        "crt/aws-crt-cpp/crt/aws-c-compression/include/**/*.h",
        "crt/aws-crt-cpp/crt/aws-c-event-stream/include/**/*.h",
        "crt/aws-crt-cpp/crt/aws-c-http/include/**/*.h",
        "crt/aws-crt-cpp/crt/aws-c-io/include/**/*.h",
        "crt/aws-crt-cpp/crt/aws-c-mqtt/include/**/*.h",
        "crt/aws-crt-cpp/crt/aws-c-s3/include/**/*.h",
        "crt/aws-crt-cpp/crt/aws-c-sdkutils/include/**/*.h",
        "crt/aws-crt-cpp/crt/aws-checksums/include/**/*.h",
        "crt/aws-crt-cpp/crt/aws-lc/include/**/*.h",
        "crt/aws-crt-cpp/crt/s2n/include/**/*.h",
    ]),
    copts = COPTS + [
        "-Iexternal/aws-sdk-cpp/crt/aws-crt-cpp/crt/aws-c-common/include",
        "-I$(GENDIR)/external/aws-sdk-cpp/crt/aws-c-common/generated/include",
    ],
    includes = [
        "crt/aws-crt-cpp/crt/aws-c-common/include",
        "crt/aws-crt-cpp/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@curl",
        "@openssl",
        "@zlib",
    ],
)

# Core AWS SDK library
cc_library(
    name = "aws-cpp-sdk-core",
    srcs = glob([
        "src/aws-cpp-sdk-core/source/**/*.cpp",
    ]),
    hdrs = glob([
        "src/aws-cpp-sdk-core/include/**/*.h",
        "src/aws-cpp-sdk-core/include/**/*.hpp",
    ]),
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

# S3 Encryption library
cc_library(
    name = "aws-cpp-sdk-s3-encryption",
    srcs = glob([
        "src/aws-cpp-sdk-s3-encryption/source/**/*.cpp",
    ]),
    hdrs = glob([
        "src/aws-cpp-sdk-s3-encryption/include/**/*.h",
        "src/aws-cpp-sdk-s3-encryption/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "include",
        "src/aws-cpp-sdk-s3-encryption/include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# Transfer library
cc_library(
    name = "aws-cpp-sdk-transfer",
    srcs = glob([
        "src/aws-cpp-sdk-transfer/source/**/*.cpp",
    ]),
    hdrs = glob([
        "src/aws-cpp-sdk-transfer/include/**/*.h",
        "src/aws-cpp-sdk-transfer/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "include",
        "src/aws-cpp-sdk-transfer/include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# Queues library
cc_library(
    name = "aws-cpp-sdk-queues",
    srcs = glob([
        "src/aws-cpp-sdk-queues/source/**/*.cpp",
    ]),
    hdrs = glob([
        "src/aws-cpp-sdk-queues/include/**/*.h",
        "src/aws-cpp-sdk-queues/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "include",
        "src/aws-cpp-sdk-queues/include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
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

# Text to Speech library
cc_library(
    name = "aws-cpp-sdk-text-to-speech",
    srcs = glob([
        "src/aws-cpp-sdk-text-to-speech/source/**/*.cpp",
    ]),
    hdrs = glob([
        "src/aws-cpp-sdk-text-to-speech/include/**/*.h",
        "src/aws-cpp-sdk-text-to-speech/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "include",
        "src/aws-cpp-sdk-text-to-speech/include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# S3 library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-s3",
    srcs = glob([
        "generated/src/aws-cpp-sdk-s3/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-s3/include/**/*.h",
        "generated/src/aws-cpp-sdk-s3/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-s3/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# DynamoDB library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-dynamodb",
    srcs = glob([
        "generated/src/aws-cpp-sdk-dynamodb/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-dynamodb/include/**/*.h",
        "generated/src/aws-cpp-sdk-dynamodb/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-dynamodb/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# Lambda library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-lambda",
    srcs = glob([
        "generated/src/aws-cpp-sdk-lambda/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-lambda/include/**/*.h",
        "generated/src/aws-cpp-sdk-lambda/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-lambda/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# CloudFront library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-cloudfront",
    srcs = glob([
        "generated/src/aws-cpp-sdk-cloudfront/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-cloudfront/include/**/*.h",
        "generated/src/aws-cpp-sdk-cloudfront/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-cloudfront/include",
        "include",
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

# CloudWatch library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-cloudwatch",
    srcs = glob([
        "generated/src/aws-cpp-sdk-cloudwatch/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-cloudwatch/include/**/*.h",
        "generated/src/aws-cpp-sdk-cloudwatch/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-cloudwatch/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# SQS library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-sqs",
    srcs = glob([
        "generated/src/aws-cpp-sdk-sqs/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-sqs/include/**/*.h",
        "generated/src/aws-cpp-sdk-sqs/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-sqs/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# SNS library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-sns",
    srcs = glob([
        "generated/src/aws-cpp-sdk-sns/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-sns/include/**/*.h",
        "generated/src/aws-cpp-sdk-sns/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-sns/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# KMS library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-kms",
    srcs = glob([
        "generated/src/aws-cpp-sdk-kms/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-kms/include/**/*.h",
        "generated/src/aws-cpp-sdk-kms/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-kms/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# STS library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-sts",
    srcs = glob([
        "generated/src/aws-cpp-sdk-sts/source/**/*.cpp",
    ]),
    hdrs = glob([
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

# CloudFormation library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-cloudformation",
    srcs = glob([
        "generated/src/aws-cpp-sdk-cloudformation/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-cloudformation/include/**/*.h",
        "generated/src/aws-cpp-sdk-cloudformation/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-cloudformation/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# Auto Scaling library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-autoscaling",
    srcs = glob([
        "generated/src/aws-cpp-sdk-autoscaling/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-autoscaling/include/**/*.h",
        "generated/src/aws-cpp-sdk-autoscaling/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-autoscaling/include",
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

# RDS library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-rds",
    srcs = glob([
        "generated/src/aws-cpp-sdk-rds/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-rds/include/**/*.h",
        "generated/src/aws-cpp-sdk-rds/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-rds/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# ECS library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-ecs",
    srcs = glob([
        "generated/src/aws-cpp-sdk-ecs/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-ecs/include/**/*.h",
        "generated/src/aws-cpp-sdk-ecs/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-ecs/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# EKS library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-eks",
    srcs = glob([
        "generated/src/aws-cpp-sdk-eks/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-eks/include/**/*.h",
        "generated/src/aws-cpp-sdk-eks/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-eks/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# API Gateway library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-apigateway",
    srcs = glob([
        "generated/src/aws-cpp-sdk-apigateway/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-apigateway/include/**/*.h",
        "generated/src/aws-cpp-sdk-apigateway/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-apigateway/include",
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
    srcs = glob([
        "generated/src/aws-cpp-sdk-cognito-identity/source/**/*.cpp",
    ]),
    hdrs = glob([
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

# Cognito User Pools library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-cognito-idp",
    srcs = glob([
        "generated/src/aws-cpp-sdk-cognito-idp/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-cognito-idp/include/**/*.h",
        "generated/src/aws-cpp-sdk-cognito-idp/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-cognito-idp/include",
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

# CloudTrail library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-cloudtrail",
    srcs = glob([
        "generated/src/aws-cpp-sdk-cloudtrail/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-cloudtrail/include/**/*.h",
        "generated/src/aws-cpp-sdk-cloudtrail/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-cloudtrail/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# CloudWatch Logs library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-logs",
    srcs = glob([
        "generated/src/aws-cpp-sdk-logs/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-logs/include/**/*.h",
        "generated/src/aws-cpp-sdk-logs/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-logs/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# Config library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-config",
    srcs = glob([
        "generated/src/aws-cpp-sdk-config/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-config/include/**/*.h",
        "generated/src/aws-cpp-sdk-config/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-config/include",
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

# CloudWatch Events library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-events",
    srcs = glob([
        "generated/src/aws-cpp-sdk-events/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-events/include/**/*.h",
        "generated/src/aws-cpp-sdk-events/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-events/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# CloudWatch Synthetics library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-synthetics",
    srcs = glob([
        "generated/src/aws-cpp-sdk-synthetics/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-synthetics/include/**/*.h",
        "generated/src/aws-cpp-sdk-synthetics/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-synthetics/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# CloudWatch RUM library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-rum",
    srcs = glob([
        "generated/src/aws-cpp-sdk-rum/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-rum/include/**/*.h",
        "generated/src/aws-cpp-sdk-rum/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-rum/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# CloudWatch Internet Monitor library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-internetmonitor",
    srcs = glob([
        "generated/src/aws-cpp-sdk-internetmonitor/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-internetmonitor/include/**/*.h",
        "generated/src/aws-cpp-sdk-internetmonitor/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-internetmonitor/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# CloudWatch Network Monitor library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-networkmonitor",
    srcs = glob([
        "generated/src/aws-cpp-sdk-networkmonitor/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-networkmonitor/include/**/*.h",
        "generated/src/aws-cpp-sdk-networkmonitor/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-networkmonitor/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# CloudWatch Application Signals library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-applicationsignals",
    srcs = glob([
        "generated/src/aws-cpp-sdk-applicationsignals/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-applicationsignals/include/**/*.h",
        "generated/src/aws-cpp-sdk-applicationsignals/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-applicationsignals/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# CloudWatch Evidently library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-evidently",
    srcs = glob([
        "generated/src/aws-cpp-sdk-evidently/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-evidently/include/**/*.h",
        "generated/src/aws-cpp-sdk-evidently/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-evidently/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# CloudWatch Application Insights library (from generated sources)
cc_library(
    name = "aws-cpp-sdk-applicationinsights",
    srcs = glob([
        "generated/src/aws-cpp-sdk-applicationinsights/source/**/*.cpp",
    ]),
    hdrs = glob([
        "generated/src/aws-cpp-sdk-applicationinsights/include/**/*.h",
        "generated/src/aws-cpp-sdk-applicationinsights/include/**/*.hpp",
    ]),
    copts = COPTS,
    includes = [
        "generated/src/aws-cpp-sdk-applicationinsights/include",
        "include",
    ],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":aws-cpp-sdk-core",
    ],
)

# Convenience alias for the core library
alias(
    name = "aws-sdk-cpp",
    actual = ":aws-cpp-sdk-core",
    visibility = ["//visibility:public"],
)
