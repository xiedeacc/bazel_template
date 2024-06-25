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
    name = "rules_cc",
    remote = "git@github.com:bazelbuild/rules_cc.git",
    tag = "0.0.9",
)

new_git_repository(
    name = "libsodium",
    build_file = "//bazel_script:libsodium.BUILD",
    remote = "git@github.com:jedisct1/libsodium.git",
    tag = "1.0.20-RELEASE",
)

load("@bazel_skylib//lib:versions.bzl", "versions")

versions.check("7.2.0")
