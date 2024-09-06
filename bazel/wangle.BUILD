package(default_visibility = ["//visibility:public"])

cc_library(
    name = "wangle",
    srcs = glob(
        [
            "wangle/**/*.cpp",
        ],
        exclude = [
            "wangle/example/**",
            "wangle/**/test/**",
        ],
    ),
    hdrs = glob(
        [
            "wangle/**/*.h",
        ],
        exclude = [
            "wangle/example/**",
            "wangle/**/test/**",
        ],
    ),
    copts = [
        "-isystem external/wangle",
        "-isystem external/libsodium/src/libsodium/include",
        "-Iexternal/libsodium/src/libsodium/include/sodium",
        "-isystem $(GENDIR)/external/libsodium/src/libsodium/include",
        "-isystem external/double-conversion",
        "-isystem $(GENDIR)/external/folly",
        "-isystem external/folly",
        "-isystem $(GENDIR)/external/fizz",
        "-isystem external/fizz",
    ],
    deps = [
        "@fizz",
        "@folly",
    ],
)
