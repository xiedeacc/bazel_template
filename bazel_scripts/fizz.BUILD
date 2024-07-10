package(default_visibility = ["//visibility:public"])

config_setting(
    name = "windows_x86_64",
    constraint_values = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    visibility = ["//visibility:public"],
)

platform(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
)

platform(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
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
        "-isystem $(BINDIR)/external/fizz",
        "-isystem external/zlib",
        "-isystem external/zstd/lib",
        "-isystem external/double-conversion",
        "-isystem external/lz4/lib",
        "-isystem external/bzip2",
        "-isystem external/com_github_google_snappy",
        "-isystem external/libsodium/src/libsodium/include",
        "-Iexternal/libsodium/src/libsodium/include/sodium",
        "-isystem $(BINDIR)/external/libsodium/src/libsodium/include",
        "-isystem $(BINDIR)/external/libunwind/include",
        "-Iexternal/libunwind/src",
        "-Iexternal/libunwind/include",
        "-I$(BINDIR)/external/libunwind/include/tdep",
        "-Iexternal/libunwind/include/tdep",
        "-Iexternal/libunwind/src/mi",
        "-isystem $(BINDIR)/external/folly",
        "-isystem external/folly",
        "-std=c++17",
    ],
    deps = [
        "@double-conversion//:double-conversion",
        "@folly",
    ],
)
