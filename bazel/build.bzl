"""Helper macros for C++ tests and build wiring.

Currently provides a thin wrapper around native.cc_test with repo defaults.
"""

load("@rules_cc//cc:defs.bzl", _cc_test = "cc_test")

def cc_test(
        deps = [],
        autolink_main = True,
        **kwargs):
    """Wrapper around native.cc_test with repo defaults.

    Ensures a shared test main unless explicitly provided, and merges
    common test dependencies.

    Args:
      deps: Additional dependencies to append to the test target.
      autolink_main: If True, links a shared test main unless one is provided.
      **kwargs: Other native.cc_test keyword arguments.
    """
    test_main = []
    test_deps = [
        "@com_google_googletest//:gtest",
        "@com_github_glog_glog//:glog",
    ]

    if autolink_main:
        if "//external:gtest_main" in deps:
            deps.pop("//external:gtest_main")
    test_main = ["//src/test:test_main"]
    _cc_test(
        linkstatic = True,
        deps = depset(test_main + test_deps + deps).to_list() + select({
            "@bazel_template//bazel:libc_musl": [],
            "//conditions:default": ["@mimalloc//:mimalloc"],
        }),
        **kwargs
    )
