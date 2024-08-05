package(default_visibility = ["//visibility:public"])

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
        "#define FIZZ_BUILD_AEGIS 0",
        "#define FIZZ_CERTIFICATE_USE_OPENSSL_CERT 1",
        "#define FIZZ_HAVE_OQS 0",
        "EOF",
    ]),
)

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
            "fizz/cmake",
            "fizz/**/test/**",
        ],
    ),
    hdrs = [":fizz-config_h"] + glob(
        ["fizz/**/*.h"],
    ),
    copts = [
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
        "-isystem $(GENDIR)/external/libunwind/include",
        "-Iexternal/libunwind/src",
        "-Iexternal/libunwind/include",
        "-I$(GENDIR)/external/libunwind/include/tdep",
        "-Iexternal/libunwind/include/tdep",
        "-Iexternal/libunwind/src/mi",
        "-isystem $(GENDIR)/external/folly",
        "-isystem external/folly",
        "-std=c++17",
    ],
    deps = [
        "@double-conversion//:double-conversion",
        "@folly",
    ],
)
