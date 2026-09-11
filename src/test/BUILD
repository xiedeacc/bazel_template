load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    features = [
        "cpp_modules",
        "layering_check",
    ],
)

cc_library(
    name = "test_main",
    srcs = ["test_main.cc"],
    # The module deps scanner runs cl.exe directly, so it chokes on gcc-style
    # -isystem flags the compiler would merely warn about.
    copts = select({
        "@platforms//os:windows": [
            "/Iexternal/double-conversion",
            "/Iexternal/folly",
            "/I$(GENDIR)/external/folly",
        ],
        "//conditions:default": [
            "-isystem external/double-conversion",
            "-isystem external/folly",
            "-isystem $(GENDIR)/external/folly",
        ],
    }),
    deps = [
        "//src/common:logging",
        "@com_google_googletest//:gtest",
        "@folly",
        "@folly//:common",
        "@gflags",
    ],
)
