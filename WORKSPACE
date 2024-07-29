workspace(name = "bazel_template")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:local.bzl", "local_repository", "new_local_repository")
load("//bazel:common.bzl", "gen_local_config_git")

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
    tag = "0.11.1",
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
    tag = "1.0.1",
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
    build_file = "//bazel:cpplint.BUILD",
    commit = "7b88b68187e3516540fab3caa900988d2179ed24",
    remote = "git@github.com:cpplint/cpplint.git",
)

new_git_repository(
    name = "liburing",
    build_file = "//bazel:liburing.BUILD",
    commit = "7b3245583069bd481190c9da18f22e9fc8c3a805",
    remote = "git@github.com:axboe/liburing.git",
)

new_git_repository(
    name = "libaio",
    build_file = "//bazel:libaio.BUILD",
    commit = "b8eadc9f89e8f7ab0338eacda9f98a6caea76883",
    remote = "https://pagure.io/libaio.git",
)

http_archive(
    name = "nasm",
    build_file = "//bazel:nasm.BUILD",
    sha256 = "f5c93c146f52b4f1664fa3ce6579f961a910e869ab0dae431bd871bdd2584ef2",
    strip_prefix = "nasm-2.15.05",
    urls = [
        "https://mirror.bazel.build/www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/nasm-2.15.05-win64.zip",
        "https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/win64/nasm-2.15.05-win64.zip",
    ],
)

http_archive(
    name = "perl",
    build_file = "//bazel:perl.BUILD",
    sha256 = "aeb973da474f14210d3e1a1f942dcf779e2ae7e71e4c535e6c53ebabe632cc98",
    urls = [
        "https://mirror.bazel.build/strawberryperl.com/download/5.32.1.1/strawberry-perl-5.32.1.1-64bit.zip",
        "https://strawberryperl.com/download/5.32.1.1/strawberry-perl-5.32.1.1-64bit.zip",
    ],
)

new_git_repository(
    name = "xz",
    build_file = "//bazel:xz.BUILD",
    commit = "0232e66d5bc5b01a25a447c657e51747626488ab",
    remote = "git@github.com:tukaani-project/xz.git",
)

new_git_repository(
    name = "zlib",
    build_file = "//bazel:zlib.BUILD",
    remote = "git@github.com:madler/zlib.git",
    tag = "v1.3.1",
)

new_git_repository(
    name = "bzip2",
    build_file = "//bazel:bzip2.BUILD",
    commit = "66c46b8c9436613fd81bc5d03f63a61933a4dcc3",
    remote = "https://gitlab.com/bzip2/bzip2.git",
)

new_git_repository(
    name = "lz4",
    build_file = "//bazel:lz4.BUILD",
    commit = "5b0ccb8b62eba9f0ed4b46ff3680c442c3e58188",
    remote = "git@github.com:lz4/lz4.git",
)

new_git_repository(
    name = "zstd",
    build_file = "//bazel:zstd.BUILD",
    remote = "git@github.com:facebook/zstd.git",
    tag = "v1.5.6",
)

new_git_repository(
    name = "brotli",
    build_file = "//bazel:brotli.BUILD",
    remote = "git@github.com:google/brotli.git",
    tag = "v1.1.0",
)

new_git_repository(
    name = "libsodium",
    build_file = "//bazel:libsodium.BUILD",
    remote = "git@github.com:jedisct1/libsodium.git",
    tag = "1.0.20-RELEASE",
)

http_archive(
    name = "openssl",
    build_file = "//bazel:openssl.make.BUILD",
    sha256 = "777cd596284c883375a2a7a11bf5d2786fc5413255efab20c50d6ffe6d020b7e",
    strip_prefix = "openssl-3.3.1",
    urls = ["https://github.com/openssl/openssl/releases/download/openssl-3.3.1/openssl-3.3.1.tar.gz"],
)

new_git_repository(
    name = "c-ares",
    build_file = "//bazel:c-ares.BUILD",
    commit = "5e1c3a7575e458ae51863da9b8d3d5d3ec6ffab8",
    remote = "git@github.com:c-ares/c-ares.git",
)

new_git_repository(
    name = "curl",
    build_file = "//bazel:curl.BUILD",
    commit = "2d5aea9c93bae110ffe5107ba2c118b8442b495d",
    remote = "git@github.com:curl/curl.git",
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
    repo_mapping = {
        "@gflags": "@com_github_gflags_gflags",
    },
    tag = "v0.5.0",
)

git_repository(
    name = "com_google_googletest",
    remote = "git@github.com:google/googletest.git",
    repo_mapping = {
        "@abseil-cpp": "@com_google_absl",
    },
    tag = "v1.14.0",
)

git_repository(
    name = "com_github_google_snappy",
    remote = "git@github.com:google/snappy.git",
    tag = "1.2.1",
)

git_repository(
    name = "com_googlesource_code_re2",
    remote = "git@github.com:google/re2.git",
    repo_mapping = {
        "@abseil-cpp": "@com_google_absl",
    },
    tag = "2024-07-02",
)

git_repository(
    name = "double-conversion",
    remote = "git@github.com:google/double-conversion.git",
    tag = "v3.3.0",
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
    name = "rules_proto",
    remote = "git@github.com:bazelbuild/rules_proto.git",
    repo_mapping = {
        "@abseil-cpp": "@com_google_absl",
        "@protobuf": "@com_google_protobuf",
    },
    tag = "6.0.2",
)

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "45ed6bf51f659c7db830fd15ddd4495dadc5afd1",
    remote = "git@github.com:nelhage/rules_boost.git",
    repo_mapping = {
        "@boringssl": "@openssl",
        "@org_lzma_lzma": "@xz",
        "@org_bzip_bzip2": "@bzip2",
    },
)

http_archive(
    name = "boost",
    build_file = "@com_github_nelhage_rules_boost//:boost.BUILD",
    patch_cmds = ["rm -f doc/pdf/BUILD"],
    patch_cmds_win = ["Remove-Item -Force doc/pdf/BUILD"],
    repo_mapping = {
        "@boringssl": "@openssl",
        "@org_lzma_lzma": "@xz",
        "@org_bzip_bzip2": "@bzip2",
        "@com_github_facebook_zstd": "@zstd",
    },
    sha256 = "0c6049764e80aa32754acd7d4f179fd5551d8172a83b71532ae093e7384e98da",
    strip_prefix = "boost-1.83.0",
    url = "https://github.com/boostorg/boost/releases/download/boost-1.83.0/boost-1.83.0.tar.gz",
)

http_archive(
    name = "jemalloc",
    build_file = "//bazel:jemalloc.make.BUILD",
    sha256 = "2db82d1e7119df3e71b7640219b6dfe84789bc0537983c3b7ac4f7189aecfeaa",
    strip_prefix = "jemalloc-5.3.0",
    url = "https://github.com/jemalloc/jemalloc/releases/download/5.3.0/jemalloc-5.3.0.tar.bz2",
)

#new_git_repository(
#    name = "jemalloc",
#    build_file = "//bazel:jemalloc.make.BUILD",
#    remote = "git@github.com:jemalloc/jemalloc.git",
#    #commit = "21bcc0a8d49ab2944ae53c7e43f5c84fc8a34322",
#    tag = "5.3.0",
#)

new_git_repository(
    name = "fmt",
    build_file = "//bazel:fmt.BUILD",
    remote = "git@github.com:fmtlib/fmt.git",
    tag = "9.1.0",
)

new_git_repository(
    name = "libdwarf",
    build_file = "//bazel:libdwarf.BUILD",
    remote = "git@github.com:davea42/libdwarf-code.git",
    tag = "v0.10.1",
)

new_git_repository(
    name = "libunwind",
    build_file = "//bazel:libunwind.BUILD",
    #tag = "v1.8.2",
    commit = "3c47821d681777e3cff33edb25c804d93102e1c6",
    remote = "git@github.com:libunwind/libunwind.git",
)

new_git_repository(
    name = "mbedtls",
    build_file = "//bazel:mbedtls.BUILD",
    remote = "git@github.com:Mbed-TLS/mbedtls.git",
    tag = "v3.6.0",
)

new_git_repository(
    name = "libevent",
    build_file = "//bazel:libevent.BUILD",
    commit = "90b9520f3ca04dd1278c831e61a82859e3be090e",
    remote = "git@github.com:libevent/libevent.git",
)

http_archive(
    name = "libev",
    build_file = "//bazel:libev.BUILD",
    sha256 = "507eb7b8d1015fbec5b935f34ebed15bf346bed04a11ab82b8eee848c4205aea",
    strip_prefix = "libev-4.33",
    url = "http://dist.schmorp.de/libev/libev-4.33.tar.gz",
)

new_git_repository(
    name = "libuv",
    build_file = "//bazel:libuv.BUILD",
    remote = "git@github.com:libuv/libuv.git",
    tag = "v1.48.0",
)

http_archive(
    name = "libiberty",
    build_file = "//bazel:libiberty.BUILD",
    sha256 = "f6e4d41fd5fc778b06b7891457b3620da5ecea1006c6a4a41ae998109f85a800",
    strip_prefix = "binutils-2.42",
    url = "https://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.xz",
)

new_git_repository(
    name = "folly",
    build_file = "//bazel:folly.BUILD",
    patch_args = ["-p1"],
    patches = ["//bazel:folly.patch"],
    remote = "git@github.com:facebook/folly.git",
    tag = "v2024.07.08.00",
)

new_git_repository(
    name = "fizz",
    build_file = "//bazel:fizz.BUILD",
    remote = "git@github.com:facebookincubator/fizz.git",
    tag = "v2024.07.08.00",
)

new_git_repository(
    name = "mvfst",
    build_file = "//bazel:mvfst.BUILD",
    remote = "git@github.com:facebook/mvfst.git",
    tag = "v2024.07.08.00",
)

new_git_repository(
    name = "wangle",
    build_file = "//bazel:wangle.BUILD",
    remote = "git@github.com:facebook/wangle.git",
    tag = "v2024.07.08.00",
)

new_git_repository(
    name = "fatal",
    build_file = "//bazel:fatal.BUILD",
    remote = "git@github.com:facebook/fatal.git",
    tag = "v2024.07.08.00",
)

new_git_repository(
    name = "xxhash",
    build_file = "//bazel:xxhash.BUILD",
    commit = "d5fe4f54c47bc8b8e76c6da9146c32d5c720cd79",
    remote = "git@github.com:Cyan4973/xxHash.git",
)

git_repository(
    name = "yaml-cpp",
    commit = "1d8ca1f35eb3a9c9142462b28282a848e5d29a91",
    remote = "git@github.com:jbeder/yaml-cpp.git",
    repo_mapping = {
        "@abseil-cpp": "@com_google_absl",
    },
)

new_git_repository(
    name = "fbthrift",
    build_file = "//bazel:fbthrift.BUILD",
    remote = "git@github.com:facebook/fbthrift.git",
    tag = "v2024.07.08.00",
)

new_git_repository(
    name = "fb303",
    build_file = "//bazel:fb303.BUILD",
    remote = "git@github.com:facebook/fb303.git",
    tag = "v2024.07.08.00",
)

new_git_repository(
    name = "proxygen",
    build_file = "//bazel:proxygen.BUILD",
    remote = "git@github.com:facebook/proxygen.git",
    tag = "v2024.07.08.00",
)

new_git_repository(
    name = "smhasher",
    build_file = "//bazel:smhasher.BUILD",
    commit = "61a0530f28277f2e850bfc39600ce61d02b518de",
    remote = "git@github.com:aappleby/smhasher.git",
)

git_repository(
    name = "com_google_googleapis",
    commit = "ba245fa19c1e6f1f2a13055a437f0c815c061867",
    remote = "git@github.com:googleapis/googleapis.git",
)

http_archive(
    name = "opencensus_proto",
    sha256 = "e3d89f7f9ed84c9b6eee818c2e9306950519402bf803698b15c310b77ca2f0f3",
    strip_prefix = "opencensus-proto-0.4.1/src",
    urls = ["https://github.com/census-instrumentation/opencensus-proto/archive/v0.4.1.tar.gz"],
)

git_repository(
    name = "envoy_api",
    commit = "4118c17a2905aaf20554d0154bb8d0cd424163c4",
    remote = "git@github.com:envoyproxy/data-plane-api.git",
)

git_repository(
    name = "com_envoyproxy_protoc_gen_validate",
    remote = "git@github.com:envoyproxy/protoc-gen-validate.git",
    tag = "v1.0.4",
)

git_repository(
    name = "cel-spec",
    remote = "git@github.com:google/cel-spec.git",
    tag = "v0.15.0",
)

git_repository(
    name = "com_github_cncf_xds",
    commit = "024c85f92f20cab567a83acc50934c7f9711d124",
    remote = "git@github.com:cncf/xds.git",
    repo_mapping = {
        "@dev_cel": "@cel-spec",
    },
)

load("@com_google_googleapis//:repository_rules.bzl", "switched_rules_by_language")

switched_rules_by_language(
    name = "com_google_googleapis_imports",
    cc = True,
    grpc = True,
)

git_repository(
    name = "com_github_grpc_grpc",
    remote = "https://github.com/grpc/grpc.git",
    repo_mapping = {
        "@com_github_cares_cares": "@c-ares",
        "@boringssl": "@openssl",
    },
    tag = "v1.65.1",
)

new_git_repository(
    name = "gperftools",
    #build_file = "//bazel:gperftools.make.BUILD",
    build_file = "//bazel:gperftools.BUILD",
    #tag = "gperftools-2.15",
    commit = "285908e8c7cfa98659127a23532c060f8dcbd148",
    remote = "git@github.com:gperftools/gperftools.git",
    repo_mapping = {
        "@abseil-cpp": "@com_google_absl",
    },
)

register_toolchains(
    "//toolchain:clang_toolchain_for_linux_aarch64",
)

new_local_repository(
    name = "clang_sysroot",
    build_file_content =
        """
filegroup(
  name = "sysroot",
  srcs = glob(["*/**"]),
  visibility = ["//visibility:public"],
)
        """,
    path = "/root/src/software/clang_sysroot",
)

new_local_repository(
    name = "gcc_sysroot",
    build_file_content =
        """
filegroup(
  name = "sysroot",
  srcs = glob(["*/**"]),
  visibility = ["//visibility:public"],
)
        """,
    path = "/root/src/software/gcc_sysroot",
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

git_repository(
    name = "hedron_compile_commands",
    commit = "e43e8eaeed3e252ac7c02983f4b1792bdff2e2f0",
    remote = "git@github.com:xiedeacc/bazel-compile-commands-extractor.git",
)

load("@bazel_skylib//lib:versions.bzl", "versions")

versions.check("7.2.0")

load("@bazel_features//:deps.bzl", "bazel_features_deps")
load("@io_bazel_rules_closure//closure:repositories.bzl", "rules_closure_dependencies", "rules_closure_toolchains")
load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")
load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
load("@rules_java//java:repositories.bzl", "rules_java_dependencies", "rules_java_toolchains")
load("@rules_perl//perl:deps.bzl", "perl_register_toolchains")
load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")
load("@rules_proto//proto:toolchains.bzl", "rules_proto_toolchains")
load("@rules_python//python:repositories.bzl", "py_repositories")

bazel_features_deps()

rules_foreign_cc_dependencies()

rules_java_dependencies()

py_repositories()

go_rules_dependencies()

rules_pkg_dependencies()

rules_java_toolchains()

go_register_toolchains(version = "1.22.1")

perl_register_toolchains()

rules_closure_dependencies()

rules_closure_toolchains()

rules_proto_toolchains()

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
load("@hedron_compile_commands//:workspace_setup_transitive.bzl", "hedron_compile_commands_setup_transitive")
load("@hedron_compile_commands//:workspace_setup_transitive_transitive.bzl", "hedron_compile_commands_setup_transitive_transitive")
load("@hedron_compile_commands//:workspace_setup_transitive_transitive_transitive.bzl", "hedron_compile_commands_setup_transitive_transitive_transitive")

hedron_compile_commands_setup()

hedron_compile_commands_setup_transitive()

hedron_compile_commands_setup_transitive_transitive()

hedron_compile_commands_setup_transitive_transitive_transitive()

gen_local_config_git(name = "local_config_git")
