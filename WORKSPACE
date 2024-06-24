workspace(name = "bazel_problem")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

git_repository(
    name = "bazel_skylib",
    remote = "git@github.com:bazelbuild/bazel-skylib.git",
    tag = "1.7.1",
)

git_repository(
    name = "platforms",
    remote = "git@github.com:bazelbuild/platforms.git",
    tag = "0.0.10",
)

git_repository(
    name = "bazel_gazelle",
    remote = "git@github.com:bazelbuild/bazel-gazelle.git",
    tag = "v0.37.0",
)

git_repository(
    name = "bazel_features",
    remote = "git@github.com:bazel-contrib/bazel_features.git",
    tag = "v1.12.0",
)

git_repository(
    name = "rules_cc",
    remote = "git@github.com:bazelbuild/rules_cc.git",
    tag = "0.0.9",
)

#new_git_repository(
#name = "jemalloc",
#build_file = "//bazel_script:jemalloc.BUILD",
#commit = "21bcc0a8d49ab2944ae53c7e43f5c84fc8a34322",
#remote = "git@github.com:jemalloc/jemalloc.git",
#)

git_repository(
    name = "jemalloc",
    #build_file = "//bazel_script:jemalloc.BUILD",
    commit = "e63592f9cfa3e388978a3b19343649e408ca9bcf",
    remote = "git@github.com:xiedeacc/jemalloc.git",
)

load("@bazel_skylib//lib:versions.bzl", "versions")

versions.check("7.2.0")

load("@bazel_features//:deps.bzl", "bazel_features_deps")

bazel_features_deps()
