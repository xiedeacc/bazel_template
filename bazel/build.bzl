def cc_test(
        deps = [],
        autolink_main = True,
        **kwargs):
    test_main = []
    test_deps = [
        "@gperftools//:profiler",
        "@com_google_googletest//:gtest",
    ]

    if autolink_main:
        if "//external:gtest_main" in deps:
            deps.pop("//external:gtest_main")
    test_main = ["//src/test:test_main"]
    native.cc_test(
        linkstatic = 1,
        deps = depset(test_main + test_deps + deps).to_list() + select({
            "//bazel:jemalloc": ["@jemalloc"],
            #"//bazel:tcmalloc": ["@tcmalloc//tcmalloc"],
            "//bazel:tcmalloc": ["//lib:jemalloc_lib"],
            "//conditions:default": [],
        }),
        **kwargs
    )
