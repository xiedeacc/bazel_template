load("@bazel_template//bazel:gflags.bzl", "gflags_library", "gflags_sources")

licenses(["notice"])

exports_files([
    "src/gflags_completions.sh",
    "COPYING.txt",
])

(hdrs, srcs) = gflags_sources(namespace = [
    "gflags",
    "google",
])

gflags_library(
    srcs = srcs,
    hdrs = hdrs,
    threads = 0,
)

gflags_library(
    srcs = srcs,
    hdrs = hdrs,
    threads = 1,
)
