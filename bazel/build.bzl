def cc_test(
        deps = [],
        autolink_main = True,
        **kwargs):
    test_main = []
    test_deps = [
        #"@gperftools//:tcmalloc_and_profiler",
        "@libunwind//:unwind-all",
        "@com_google_googletest//:gtest",
    ]

    if autolink_main:
        if "//external:gtest_main" in deps:
            deps.pop("//external:gtest_main")
    test_main = ["//src/test_util:test_main"]
    native.cc_test(
        linkstatic = 1,
        deps = depset(test_main + test_deps + deps).to_list(),
        **kwargs
    )
