load("@rules_cc//cc:defs.bzl", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [
        "/Iexternal/fizz",
        "/I$(GENDIR)/external/fizz",
        "/Iexternal/zstd/lib",
        "/Iexternal/double-conversion",
        "/Iexternal/lz4/lib",
        "/Iexternal/bzip2",
        "/Iexternal/com_github_google_snappy",
        "/Iexternal/libsodium/src/libsodium/include",
        "/Iexternal/libsodium/src/libsodium/include/sodium",
        "/I$(GENDIR)/external/libsodium/src/libsodium/include",
        "/I$(GENDIR)/external/folly",
        "/Iexternal/folly",
    ],
    "//conditions:default": [
        "-isystem external/fizz",
        "-isystem $(GENDIR)/external/fizz",
        "-isystem external/zstd/lib",
        "-isystem external/double-conversion",
        "-isystem external/lz4/lib",
        "-isystem external/bzip2",
        "-isystem external/com_github_google_snappy",
        "-isystem external/libsodium/src/libsodium/include",
        "-Iexternal/libsodium/src/libsodium/include/sodium",
        "-isystem $(GENDIR)/external/libsodium/src/libsodium/include",
        "-isystem $(GENDIR)/external/folly",
        "-isystem external/folly",
    ],
}) + select({
    "@platforms//os:linux": [
        "-I$(GENDIR)/external/libunwind/include",
        "-Iexternal/libunwind/src",
        "-Iexternal/libunwind/include",
        "-I$(GENDIR)/external/libunwind/include/tdep",
        "-Iexternal/libunwind/include/tdep",
        "-Iexternal/libunwind/src/mi",
    ],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

DEFINES = GLOBAL_DEFINES

LINKOPTS = GLOBAL_LINKOPTS

cc_library(
    name = "fizz",
    srcs = glob(
        [
            "fizz/**/*.cpp",
            "fizz/**/*.c",
        ],
        exclude = [
            "fizz/experimental/crypto/exchange/OQSKeyExchange.cpp",
            "fizz/extensions/javacrypto/**",
            "fizz/tool/**",
            "fizz/cmake",
            "fizz/**/test/**",
        ],
    ),
    hdrs = [":fizz-config_h"] + glob(
        ["fizz/**/*.h"],
    ),
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@double-conversion//:double-conversion",
        "@folly",
    ],
)

genrule(
    name = "fizz-config_h",
    outs = ["fizz/fizz-config.h"],
    cmd = "\n".join([
        "cat <<'EOF' >$@",
        "/*",
        " *  Copyright (c) 2023-present, Facebook, Inc.",
        " *  All rights reserved.",
        " *",
        " *  This source code is licensed under the BSD-style license found in the",
        " *  LICENSE file in the root directory of this source tree.",
        " */",
        "",
        "/**",
        " * <fizz/fizz-config.h> holds compile time configuration options for Fizz.",
        " */",
        "#pragma once",
        "",
        "#define FIZZ_HAVE_LIBAEGIS 0",
        "#define FIZZ_HAVE_SODIUM 1",
        "#define FIZZ_BUILD_AEGIS 0",
        "#define FIZZ_CERTIFICATE_USE_OPENSSL_CERT 1",
        "#define FIZZ_HAVE_OQS 0",
        "#define FIZZ_LOGGING_XLOG 1",
        "",
        "#if !defined(FIZZ_ENABLE_CONTEXT_COMPATIBILITY_CHECKS)",
        "#if defined(NDEBUG)",
        "#define FIZZ_ENABLE_CONTEXT_COMPATIBILITY_CHECKS 0",
        "#else",
        "#define FIZZ_ENABLE_CONTEXT_COMPATIBILITY_CHECKS 1",
        "#endif",
        "#endif",
        "",
        "#define FIZZ_CONTEXT_VALIDATION_SHOULD_CHECK_CIPHER(x) (true)",
        "",
        "#define FIZZ_DEFAULT_FACTORY_HEADER <fizz/protocol/MultiBackendFactory.h>",
        "#define FIZZ_DEFAULT_FACTORY ::fizz::MultiBackendFactory",
        "EOF",
    ]),
)
