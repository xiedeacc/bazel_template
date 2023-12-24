load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "all_srcs",
    srcs = glob(
        include = ["**"],
        exclude = ["*.bazel"],
    ),
)

cmake(
    name = "libevent",
    build_args = [
        "-j6",
    ],
    cache_entries = {
        "EVENT__DISABLE_OPENSSL": "OFF",
        "EVENT__DISABLE_MBEDTLS": "ON",
        "EVENT__DISABLE_REGRESS": "OFF",
        "EVENT__DISABLE_TESTS": "ON",
        "EVENT__DISABLE_SAMPLES": "ON",
        "EVENT__LIBRARY_TYPE": "STATIC",
        # Force _GNU_SOURCE on for Android builds. This would be contained in
        # a 'select' but the downstream macro uses a select on all of these
        # options, and they cannot be nested.
        # If https://github.com/bazelbuild/rules_foreign_cc/issues/289 is fixed
        # this can be removed.
        # More details https://github.com/envoyproxy/envoy-mobile/issues/116
        "_GNU_SOURCE": "ON",
    },
    copts = [
    ],
    generate_args = [],
    lib_source = ":all_srcs",
    # linkopts = ["-lpthread"],
    out_static_libs = [
        "libevent.a",
        "libevent_pthreads.a",
        "libevent_extra.a",
    ],
    targets = [],
    visibility = ["//visibility:public"],
    deps = [
        "@openssl//:ssl",
    ],
)
