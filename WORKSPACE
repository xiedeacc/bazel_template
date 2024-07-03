workspace(name = "bazel_template")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:local.bzl", "local_repository", "new_local_repository")
load("//scripts:common.bzl", "gen_local_config_git")

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

git_repository(
    name = "rules_foreign_cc",
    remote = "git@github.com:bazelbuild/rules_foreign_cc.git",
    tag = "0.10.1",
)

git_repository(
    name = "rules_proto",
    remote = "git@github.com:bazelbuild/rules_proto.git",
    tag = "6.0.0",
)

git_repository(
    name = "rules_perl",
    remote = "git@github.com:bazelbuild/rules_perl.git",
    tag = "0.2.3",
)

git_repository(
    name = "rules_python",
    remote = "git@github.com:bazelbuild/rules_python.git",
    tag = "0.33.0",
)

git_repository(
    name = "rules_java",
    remote = "git@github.com:bazelbuild/rules_java.git",
    tag = "7.6.1",
)

git_repository(
    name = "build_bazel_rules_swift",
    remote = "git@github.com:bazelbuild/rules_swift.git",
    tag = "1.18.0",
)

git_repository(
    name = "io_bazel_rules_go",
    remote = "git@github.com:bazelbuild/rules_go.git",
    tag = "v0.48.0",
)

git_repository(
    name = "rules_pkg",
    remote = "git@github.com:bazelbuild/rules_pkg.git",
    tag = "1.0.0",
)

git_repository(
    name = "io_bazel_rules_closure",
    remote = "git@github.com:bazelbuild/rules_closure.git",
    tag = "0.13.0",
)

git_repository(
    name = "contrib_rules_jvm",
    remote = "git@github.com:bazel-contrib/rules_jvm.git",
    tag = "v0.27.0",
)

git_repository(
    name = "rules_jvm_external",
    remote = "git@github.com:bazelbuild/rules_jvm_external.git",
    tag = "6.1",
)

git_repository(
    name = "io_bazel_rules_docker",
    remote = "git@github.com:bazelbuild/rules_docker.git",
    tag = "v0.25.0",
)

git_repository(
    name = "apple_rules_lint",
    remote = "git@github.com:apple/apple_rules_lint.git",
    tag = "0.3.2",
)

git_repository(
    name = "build_bazel_rules_apple",
    remote = "git@github.com:bazelbuild/rules_apple.git",
    tag = "3.5.1",
)

git_repository(
    name = "build_bazel_apple_support",
    remote = "git@github.com:bazelbuild/apple_support.git",
    tag = "1.15.1",
)

new_git_repository(
    name = "cpplint",
    build_file = "//scripts:cpplint.BUILD",
    commit = "7b88b68187e3516540fab3caa900988d2179ed24",
    remote = "git@github.com:cpplint/cpplint.git",
)

http_archive(
    name = "nasm",
    build_file = "//scripts:nasm.BUILD",
    sha256 = "f5c93c146f52b4f1664fa3ce6579f961a910e869ab0dae431bd871bdd2584ef2",
    strip_prefix = "nasm-2.15.05",
    urls = [
        "https://mirror.bazel.build/www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/nasm-2.15.05-win64.zip",
        "https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/nasm-2.15.05-win64.zip",
    ],
)

http_archive(
    name = "perl",
    build_file = "//scripts:perl.BUILD",
    sha256 = "aeb973da474f14210d3e1a1f942dcf779e2ae7e71e4c535e6c53ebabe632cc98",
    urls = [
        "https://mirror.bazel.build/strawberryperl.com/download/5.32.1.1/strawberry-perl-5.32.1.1-64bit.zip",
        "https://strawberryperl.com/download/5.32.1.1/strawberry-perl-5.32.1.1-64bit.zip",
    ],
)

new_git_repository(
    name = "xz",
    build_file = "//scripts:xz.BUILD",
    commit = "0232e66d5bc5b01a25a447c657e51747626488ab",
    remote = "git@github.com:tukaani-project/xz.git",
)

new_git_repository(
    name = "zlib",
    build_file = "//scripts:zlib.BUILD",
    remote = "git@github.com:madler/zlib.git",
    tag = "v1.3.1",
)

new_git_repository(
    name = "bzip2",
    build_file = "//scripts:bzip2.BUILD",
    commit = "66c46b8c9436613fd81bc5d03f63a61933a4dcc3",
    remote = "https://gitlab.com/bzip2/bzip2.git",
)

new_git_repository(
    name = "lz4",
    build_file = "//scripts:lz4.BUILD",
    commit = "5b0ccb8b62eba9f0ed4b46ff3680c442c3e58188",
    remote = "git@github.com:lz4/lz4.git",
)

new_git_repository(
    name = "zstd",
    build_file = "//scripts:zstd.BUILD",
    remote = "git@github.com:facebook/zstd.git",
    tag = "v1.5.6",
)

git_repository(
    name = "brotli",
    remote = "git@github.com:google/brotli.git",
    tag = "v1.1.0",
)

new_git_repository(
    name = "libsodium",
    build_file = "//scripts:libsodium.BUILD",
    remote = "git@github.com:jedisct1/libsodium.git",
    tag = "1.0.20-RELEASE",
)

http_archive(
    name = "openssl",
    build_file = "//scripts:openssl.make.BUILD",
    sha256 = "777cd596284c883375a2a7a11bf5d2786fc5413255efab20c50d6ffe6d020b7e",
    strip_prefix = "openssl-3.3.1",
    urls = ["https://github.com/openssl/openssl/releases/download/openssl-3.3.1/openssl-3.3.1.tar.gz"],
)

git_repository(
    name = "com_google_absl",
    remote = "git@github.com:abseil/abseil-cpp.git",
    tag = "20240116.2",
)

git_repository(
    name = "com_github_gflags_gflags",
    remote = "git@github.com:gflags/gflags.git",
    tag = "v2.2.2",
)

git_repository(
    name = "com_github_glog_glog",
    remote = "git@github.com:google/glog.git",
    tag = "v0.7.1",
)

git_repository(
    name = "com_google_googletest",
    remote = "git@github.com:google/googletest.git",
    tag = "v1.14.0",
)

new_git_repository(
    name = "curl",
    build_file = "//scripts:curl.BUILD",
    remote = "git@github.com:curl/curl.git",
)

git_repository(
    name = "com_google_protobuf",
    remote = "git@github.com:protocolbuffers/protobuf.git",
    repo_mapping = {
        "@com_github_google_glog": "@com_github_glog_glog",
        "@com_github_curl_curl": "@curl",
    },
    tag = "v27.1",
)

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "45ed6bf51f659c7db830fd15ddd4495dadc5afd1",
    remote = "git@github.com:nelhage/rules_boost.git",
    repo_mapping = {
        "@boringssl": "@openssl",
    },
)

register_toolchains(
    "//toolchain:clang_toolchain_for_linux_x86_64",
    "//toolchain:clang_toolchain_for_linux_aarch64",
    #"//toolchain:gcc_toolchain_for_linux_x86_64",
)

local_repository(
    name = "toolchains_openwrt",
    path = "../toolchains_openwrt",
)

load("@toolchains_openwrt//toolchain:toolchains_openwrt.bzl", "openwrt_toolchain_setup")

openwrt_toolchain_setup(
    name = "openwrt_repo_setup",
    toolchains = {
        "rockchip": {
            "armv8": {
                "arch": "aarch64",
                "url": "/root/src/software/openwrt/openwrt-toolchain-23.05.3-rockchip-armv8_gcc-12.3.0_musl.Linux-x86_64",
                "sha256sum": "fa88b24029a0bfd5ee9f854670f731406e41debe3303d9cc6123f0a157e719c3",
            },
        },
    },
)

register_toolchains("@openwrt_toolchain_config_rockchip_armv8//:cc-toolchain-rockchip_armv8")

#git_repository(
#name = "toolchains_llvm",
#remote = "git@github.com:xiedeacc/toolchains_llvm.git",
#tag = "1.0.0",
#)

local_repository(
    name = "toolchains_llvm",
    path = "../toolchains_llvm",
)

load("@toolchains_llvm//toolchain:rules.bzl", "llvm_toolchain")

llvm_toolchain(
    name = "llvm_toolchain",
    absolute_paths = True,
    #llvm_versions = {
    ##"": "15.0.6",
    #"ubuntu-18.04-x86_64": "15.0.4",
    #},
    llvm_version = "18.1.8",
    toolchain_roots = {"": "/usr/local/llvm-18"},
)

load("@llvm_toolchain//:toolchains.bzl", "llvm_register_toolchains")

llvm_register_toolchains()

git_repository(
    name = "hedron_compile_commands",
    commit = "e43e8eaeed3e252ac7c02983f4b1792bdff2e2f0",
    remote = "git@github.com:xiedeacc/bazel-compile-commands-extractor.git",
)

load("@bazel_skylib//lib:versions.bzl", "versions")

versions.check("7.2.0")

load("@bazel_features//:deps.bzl", "bazel_features_deps")
load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
load("@io_bazel_rules_closure//closure:repositories.bzl", "rules_closure_dependencies", "rules_closure_toolchains")
load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")
load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
load("@rules_java//java:repositories.bzl", "rules_java_dependencies", "rules_java_toolchains")
load("@rules_perl//perl:deps.bzl", "perl_register_toolchains")
load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies")
load("@rules_proto//proto:toolchains.bzl", "rules_proto_toolchains")
load("@rules_python//python:repositories.bzl", "py_repositories")

bazel_features_deps()

rules_foreign_cc_dependencies()

rules_java_dependencies()

py_repositories()

go_rules_dependencies()

rules_pkg_dependencies()

rules_proto_dependencies()

rules_proto_toolchains()

boost_deps()

rules_java_toolchains()

go_register_toolchains(version = "1.18")

perl_register_toolchains()

rules_closure_dependencies()

rules_closure_toolchains()

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
load("@hedron_compile_commands//:workspace_setup_transitive.bzl", "hedron_compile_commands_setup_transitive")
load("@hedron_compile_commands//:workspace_setup_transitive_transitive.bzl", "hedron_compile_commands_setup_transitive_transitive")
load("@hedron_compile_commands//:workspace_setup_transitive_transitive_transitive.bzl", "hedron_compile_commands_setup_transitive_transitive_transitive")

hedron_compile_commands_setup()

hedron_compile_commands_setup_transitive()

hedron_compile_commands_setup_transitive_transitive()

hedron_compile_commands_setup_transitive_transitive_transitive()

gen_local_config_git(name = "local_config_git")
