workspace(name = "bazel_template")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "bazel_skylib",
    sha256 = "de9d2cedea7103d20c93a5cc7763099728206bd5088342d0009315913a592cc0",
    strip_prefix = "bazel-skylib-1.4.2",
    urls = [
        "https://github.com/bazelbuild/bazel-skylib/archive/refs/tags/1.4.2.tar.gz",
    ],
)

http_archive(
    name = "platforms",
    sha256 = "1626b708a06989c2365f3101c9c937153e03ee39faaaeab98a2c204e9d015a0d",
    strip_prefix = "platforms-0.0.6",
    urls = [
        "https://github.com/bazelbuild/platforms/archive/refs/tags/0.0.6.tar.gz",
    ],
)

http_archive(
    name = "bazel_gazelle",
    sha256 = "f795b82792edeb03a7a060a908bc2cfc9702273b1387479fb99d208e83a3d7ad",
    strip_prefix = "bazel-gazelle-0.32.0",
    urls = [
        "https://github.com/bazelbuild/bazel-gazelle/archive/refs/tags/v0.32.0.tar.gz",
    ],
)

http_archive(
    name = "rules_cc",
    sha256 = "2037875b9a4456dce4a79d112a8ae885bbc4aad968e6587dca6e64f3a0900cdf",
    strip_prefix = "rules_cc-0.0.9",
    urls = [
        "https://github.com/bazelbuild/rules_cc/archive/refs/tags/0.0.9.tar.gz",
    ],
)

http_archive(
    name = "rules_foreign_cc",
    sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
    strip_prefix = "rules_foreign_cc-0.9.0",
    urls = [
        "https://github.com/bazelbuild/rules_foreign_cc/archive/refs/tags/0.9.0.tar.gz",
    ],
)

http_archive(
    name = "rules_proto",
    sha256 = "dc3fb206a2cb3441b485eb1e423165b231235a1ea9b031b4433cf7bc1fa460dd",
    strip_prefix = "rules_proto-5.3.0-21.7",
    urls = [
        "https://github.com/bazelbuild/rules_proto/archive/refs/tags/5.3.0-21.7.tar.gz",
    ],
)

http_archive(
    name = "rules_perl",
    sha256 = "391edb08802860ba733d402c6376cfe1002b598b90d2240d9d302ecce2289a64",
    strip_prefix = "rules_perl-7f10dada09fcba1dc79a6a91da2facc25e72bd7d",
    urls = [
        "https://github.com/bazelbuild/rules_perl/archive/7f10dada09fcba1dc79a6a91da2facc25e72bd7d.tar.gz",
    ],
)

http_archive(
    name = "rules_python",
    sha256 = "0a8003b044294d7840ac7d9d73eef05d6ceb682d7516781a4ec62eeb34702578",
    strip_prefix = "rules_python-0.24.0",
    urls = [
        "https://github.com/bazelbuild/rules_python/archive/refs/tags/0.24.0.tar.gz",
    ],
)

git_repository(
    name = "rules_java",
    remote = "https://github.com/bazelbuild/rules_java.git",
    tag = "6.4.0",
)

http_archive(
    name = "apple_rules_lint",
    sha256 = "119ecfbecc1befa66d1787b33f39c1250023f5546c73915b2181913628edf21b",
    strip_prefix = "apple_rules_lint-0.3.1",
    urls = [
        "https://github.com/apple/apple_rules_lint/archive/refs/tags/0.3.1.tar.gz",
    ],
)

http_archive(
    name = "contrib_rules_jvm",
    sha256 = "548f0583192ff79c317789b03b882a7be9b1325eb5d3da5d7fdcc4b7ca69d543",
    strip_prefix = "rules_jvm-0.9.0",
    urls = [
        "https://github.com/bazel-contrib/rules_jvm/archive/refs/tags/v0.9.0.tar.gz",
    ],
)

git_repository(
    name = "rules_jvm_external",
    remote = "https://github.com/bazelbuild/rules_jvm_external.git",
    tag = "5.2",
)

http_archive(
    name = "build_bazel_rules_swift",
    sha256 = "104b16612b1084918d58083b3ee2f6521eb202b4502019870da7351d10a4777f",
    strip_prefix = "rules_swift-1.9.1",
    urls = [
        "https://github.com/bazelbuild/rules_swift/archive/refs/tags/1.9.1.tar.gz",
    ],
)

http_archive(
    name = "build_bazel_rules_apple",
    sha256 = "d6735ed25754dbcb4fce38e6d72c55b55f6afa91408e0b72f1357640b88bb49c",
    strip_prefix = "rules_apple-2.5.0",
    urls = [
        "https://github.com/bazelbuild/rules_apple/archive/refs/tags/2.5.0.tar.gz",
    ],
)

http_archive(
    name = "build_bazel_apple_support",
    sha256 = "c02a8c902f405e5ea12b815f426fbe429bc39a2628b290e50703d956d40f5542",
    strip_prefix = "apple_support-1.8.0",
    urls = [
        "https://github.com/bazelbuild/apple_support/archive/refs/tags/1.8.0.tar.gz",
    ],
)

http_archive(
    name = "io_bazel_rules_docker",
    sha256 = "91844808532e5ce316b3c010929493c0244f3d37593afd6de04f71821d5136d9",
    strip_prefix = "rules_docker-0.25.0",
    urls = [
        "https://github.com/bazelbuild/rules_docker/archive/refs/tags/v0.25.0.tar.gz",
    ],
)

http_archive(
    name = "io_bazel_rules_go",
    sha256 = "12c67b212c865b884a180bd02e573c7a58c60bc2a7fb095475e24dccd9ee28f8",
    strip_prefix = "rules_go-0.41.0",
    urls = [
        "https://github.com/bazelbuild/rules_go/archive/refs/tags/v0.41.0.tar.gz",
    ],
)

http_archive(
    name = "rules_pkg",
    sha256 = "360c23a88ceaf7f051abc99e2e6048cf7fe5d9af792690576554a88b2013612d",
    strip_prefix = "rules_pkg-0.9.1",
    url = "https://github.com/bazelbuild/rules_pkg/archive/refs/tags/0.9.1.tar.gz",
)

http_archive(
    name = "com_grail_bazel_compdb",
    sha256 = "4c8a45853a4239faa34a3f09d3b8dd0bc69e3674b1bf6bbfbb53c1ff18eea3c3",
    strip_prefix = "bazel-compilation-database-bc8a4da42f39b13fc298428837bf2a87e5bcd704",
    urls = [
        "https://github.com/grailbio/bazel-compilation-database/archive/bc8a4da42f39b13fc298428837bf2a87e5bcd704.tar.gz",
    ],
)

http_archive(
    name = "cpplint",
    build_file = "//bazel:cpplint.BUILD",
    sha256 = "cc3dc1b1d7383b5a9ec8a1f302321e5618aec540a197652a3a3dd90f02e8a5a4",
    strip_prefix = "cpplint-7b88b68187e3516540fab3caa900988d2179ed24",
    urls = [
        "https://github.com/cpplint/cpplint/archive/7b88b68187e3516540fab3caa900988d2179ed24.tar.gz",
    ],
)

http_archive(
    name = "jemalloc",
    sha256 = "7c2e08eda0980c5249e2c4e73f56296da3754e0c5b55c3ffabbd0409bde568bf",
    strip_prefix = "jemalloc-14397d2ebc59298bdabb11b31a1366aed380ac81",
    urls = [
        "https://github.com/xiedeacc/jemalloc/archive/14397d2ebc59298bdabb11b31a1366aed380ac81.tar.gz",
    ],
)

http_archive(
    name = "lzma",
    sha256 = "a5f59d0bd6bd0c0dc38bfd7fb4f1463358a562a812f1735d2bc9425f8169d0a7",
    strip_prefix = "xz-6814fd3df040e5fb985a8d8b6b89c5fd5e53f9f7",
    urls = [
        "https://github.com/xiedeacc/xz/archive/6814fd3df040e5fb985a8d8b6b89c5fd5e53f9f7.tar.gz",
    ],
)

http_archive(
    name = "zlib",
    sha256 = "46e7ef872204839e74ffed8cf9004b296a80caba756e21ac546881c727af4e5f",
    strip_prefix = "zlib-50c820653b6f0c93e4d9da3141f946a98a113fd0",
    urls = [
        "https://github.com/xiedeacc/zlib/archive/50c820653b6f0c93e4d9da3141f946a98a113fd0.tar.gz",
    ],
)

http_archive(
    name = "org_bzip_bzip2",
    sha256 = "3da1097a8bad376860c6c675d9841dbb083f6459cee605e86f24978de079d143",
    strip_prefix = "bzip2-67a2430bf9f180e50c2bc50bb1fcc69155105328",
    urls = [
        "https://github.com/xiedeacc/bzip2/archive/67a2430bf9f180e50c2bc50bb1fcc69155105328.tar.gz",
    ],
)

http_archive(
    name = "lz4",
    sha256 = "8ab81fafb7ee6cddaf23fd1de4898a405c6edd739e20fe60818874bb7d59df29",
    strip_prefix = "lz4-72b88d853b968827356394ea25b902adf8d707d0",
    urls = [
        "https://github.com/xiedeacc/lz4/archive/72b88d853b968827356394ea25b902adf8d707d0.tar.gz",
    ],
)

http_archive(
    name = "com_github_facebook_zstd",
    sha256 = "e58ec647b3792c4e640ece396445f39c19a7b93c062e3ac8bc1ef3b7132369e2",
    strip_prefix = "zstd-df71f5eeadfacb8ac5ec139ea8b274a23b12dd0c",
    urls = [
        "https://github.com/xiedeacc/zstd/archive/df71f5eeadfacb8ac5ec139ea8b274a23b12dd0c.tar.gz",
    ],
)

http_archive(
    name = "openssl",
    build_file = "//bazel:openssl.BUILD",
    repo_mapping = {
        "@com_github_madler_zlib": "@zlib",
    },
    sha256 = "e2f8d84b523eecd06c7be7626830370300fbcc15386bf5142d72758f6963ebc6",
    strip_prefix = "openssl-1.1.1u",
    urls = [
        "https://github.com/openssl/openssl/releases/download/OpenSSL_1_1_1u/openssl-1.1.1u.tar.gz",
    ],
)

http_archive(
    name = "libsodium",
    repo_mapping = {
    },
    sha256 = "45faee83cb2221927123da8394c9b567f55f2d1e3c466b916e85ecd08c4b6c96",
    strip_prefix = "libsodium-c9244999525f980dd810ae9bb5c542aeeac92ecc",
    urls = [
        "https://github.com/xiedeacc/libsodium/archive/c9244999525f980dd810ae9bb5c542aeeac92ecc.tar.gz",
    ],
)

http_archive(
    name = "com_github_libevent_libevent",
    sha256 = "d3e78e05edfb6a18defbfc7f2415230cf1ce500635527499499856ba84dce70a",
    strip_prefix = "libevent-e017ba6d1001f1d91954391afc8a1324a90df63a",
    urls = [
        "https://github.com/xiedeacc/libevent/archive/e017ba6d1001f1d91954391afc8a1324a90df63a.tar.gz",
    ],
)

http_archive(
    name = "com_github_fmtlib_fmt",
    sha256 = "2534cd821b5a782df6a4fa7855fec9148d411503682ca0bee56605fa1613f8a4",
    strip_prefix = "fmt-6e54c9e689f8d4fff46e791043441cb3e45c057b",
    urls = [
        "https://github.com/xiedeacc/f6mt/archive/6e54c9e689f8d4fff46e791043441cb3e45c057b.tar.gz",
    ],
)

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = [
        "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
    ],
)

http_archive(
    name = "com_github_glog_glog",
    sha256 = "8a83bf982f37bb70825df71a9709fa90ea9f4447fb3c099e1d720a439d88bad6",
    strip_prefix = "glog-0.6.0",
    urls = [
        "https://github.com/google/glog/archive/v0.6.0.tar.gz",
    ],
)

new_git_repository(
    name = "com_github_google_snappy",
    build_file = "//bazel:snappy.BUILD",
    remote = "https://github.com/google/snappy.git",
    tag = "1.1.8",
)

http_archive(
    name = "com_google_absl",
    sha256 = "5366d7e7fa7ba0d915014d387b66d0d002c03236448e1ba9ef98122c13b35c36",
    strip_prefix = "abseil-cpp-20230125.3",
    urls = [
        "https://github.com/abseil/abseil-cpp/archive/20230125.3/abseil-cpp-20230125.3.tar.gz",
    ],
)

git_repository(
    name = "com_google_googletest",
    remote = "https://github.com/google/googletest.git",
    tag = "v1.13.0",
)

http_archive(
    name = "com_github_google_re2",
    sha256 = "05a996fade1105ee3f1cdb48c622bbbe54718bf32f8708ffccf3fb62a68100f2",
    strip_prefix = "re2-9dc7ae7b52a17b75e3f9249ea85ba578bf42f255",
    urls = [
        "https://github.com/google/re2/archive/9dc7ae7b52a17b75e3f9249ea85ba578bf42f255.tar.gz",
    ],
)

http_archive(
    name = "double-conversion",
    sha256 = "d745263abfc7f973b6d7e1890feda2a4323307797a2218345f22b29379b476d3",
    strip_prefix = "double-conversion-29e61b7f6ed92c8809d78f96c7cb59cd293adce8",
    urls = [
        "https://github.com/google/double-conversion/archive/29e61b7f6ed92c8809d78f96c7cb59cd293adce8.tar.gz",
    ],
)

http_archive(
    name = "libunwind",
    sha256 = "34db8d5e910376712e2ac5f3098dcace5fc1d4f3b84823bd8d67376a765b41b1",
    strip_prefix = "libunwind-2cfcc7ad6cefc777ea5c2ae861f089c2391166d0",
    urls = [
        "https://github.com/xiedeacc/libunwind/archive/2cfcc7ad6cefc777ea5c2ae861f089c2391166d0.tar.gz",
    ],
)

http_archive(
    name = "libdwarf",
    sha256 = "b85f5b7ba91ad61355bff6725fdc859d6a1ed6c126776428cafc5b3abd07a034",
    strip_prefix = "libdwarf-0da1734dfafcf0756c14e8ae6ee257310ed1c9b5",
    urls = [
        "https://github.com/xiedeacc/libdwarf/archive/0da1734dfafcf0756c14e8ae6ee257310ed1c9b5.tar.gz",
    ],
)

http_archive(
    name = "com_github_nelhage_rules_boost",
    repo_mapping = {
        "@boringssl": "@openssl",
    },
    sha256 = "a6df3aea9e1c69933e4d554cd81d553a030f752f7af1a93ac14730cc10e550f4",
    strip_prefix = "rules_boost-45ed6bf51f659c7db830fd15ddd4495dadc5afd1",
    urls = [
        "https://github.com/nelhage/rules_boost/archive/45ed6bf51f659c7db830fd15ddd4495dadc5afd1.tar.gz",
    ],
)

http_archive(
    name = "curl",
    build_file = "//bazel:curl.BUILD",
    sha256 = "01ae0c123dee45b01bbaef94c0bc00ed2aec89cb2ee0fd598e0d302a6b5e0a98",
    strip_prefix = "curl-7.69.1",
    urls = [
        "https://github.com/curl/curl/releases/download/curl-7_69_1/curl-7.69.1.tar.gz",
    ],
)

http_archive(
    name = "c-ares",
    sha256 = "9e9c3cbf08869c1547fccf97b0ee761b6b1a36d9a31152449b8d0a93063a1410",
    strip_prefix = "c-ares-c520142b68abf7762aa9d9c58258a2e82d3ec155",
    urls = [
        "https://github.com/xiedeacc/c-ares/archive/c520142b68abf7762aa9d9c58258a2e82d3ec155.tar.gz",
    ],
)

new_git_repository(
    name = "nacos_sdk",
    build_file = "//bazel:nacos_sdk.BUILD",
    commit = "c73bf483ca32ee7edc2eaa215268b84144493f62",
    remote = "git@github.com:xiedeacc/nacos-sdk-cpp.git",
    repo_mapping = {
        "@com_github_curl_curl": "@curl",
        "@com_github_madler_zlib": "@zlib",
    },
)

http_archive(
    name = "gperftools",
    build_file = "//bazel:gperftools.BUILD",
    sha256 = "83e3bfdd28b8bcf53222c3798d4d395d52dadbbae59e8730c4a6d31a9c3732d8",
    strip_prefix = "gperftools-2.10",
    urls = [
        "https://github.com/gperftools/gperftools/releases/download/gperftools-2.10/gperftools-2.10.tar.gz",
    ],
)

http_archive(
    name = "com_github_google_leveldb",
    build_file = "//bazel:leveldb.BUILD",
    sha256 = "9a37f8a6174f09bd622bc723b55881dc541cd50747cbd08831c2a82d620f6d76",
    strip_prefix = "leveldb-1.23",
    urls = [
        "https://github.com/google/leveldb/archive/refs/tags/1.23.tar.gz",
    ],
)

http_archive(
    name = "com_github_google_crc32c",
    build_file = "//bazel:crc32c.BUILD",
    sha256 = "ac07840513072b7fcebda6e821068aa04889018f24e10e46181068fb214d7e56",
    strip_prefix = "crc32c-1.1.2",
    urls = ["https://github.com/google/crc32c/archive/refs/tags/1.1.2.tar.gz"],
)

http_archive(
    name = "brpc",
    repo_mapping = {
        "@com_github_google_glog": "@com_github_glog_glog",
        "@com_github_curl_curl": "@curl",
        "@com_github_madler_zlib": "@zlib",
    },
    sha256 = "b9d638b76725552ed11178c650d7fc95e30f252db7972a93dc309a0698c7d2b8",
    strip_prefix = "brpc-1.3.0",
    urls = [
        "https://github.com/apache/brpc/archive/refs/tags/1.3.0.tar.gz",
    ],
)

new_git_repository(
    name = "swig",
    build_file = "//bazel:swig.BUILD",
    patch_args = ["-p1"],
    patches = ["//bazel:swig.patch"],
    remote = "https://github.com/swig/swig.git",
    tag = "v4.1.1",
)

new_git_repository(
    name = "pcre2",
    build_file = "//bazel:pcre2.BUILD",
    remote = "https://github.com/PCRE2Project/pcre2.git",
    tag = "pcre2-10.42",
)

git_repository(
    name = "com_google_benchmark",
    remote = "https://github.com/google/benchmark.git",
    tag = "v1.8.1",
)

http_archive(
    name = "spdlog",
    build_file = "//bazel:spdlog.BUILD",
    sha256 = "4dccf2d10f410c1e2feaff89966bfc49a1abb29ef6f08246335b110e001e09a9",
    strip_prefix = "spdlog-1.12.0",
    urls = [
        "https://github.com/gabime/spdlog/archive/refs/tags/v1.12.0.tar.gz",
    ],
)

new_git_repository(
    name = "rapidjson",
    build_file = "//bazel:rapidjson.BUILD",
    commit = "f9d53419e912910fd8fa57d5705fa41425428c35",
    remote = "git@github.com:Tencent/rapidjson.git",
)

load("@bazel_skylib//lib:versions.bzl", "versions")

versions.check("5.2.0")

load("@com_grail_bazel_compdb//:deps.bzl", "bazel_compdb_deps")
load("@rules_pkg//:deps.bzl", "rules_pkg_dependencies")
load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
load("@rules_perl//perl:deps.bzl", "perl_register_toolchains")
load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")
load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

bazel_compdb_deps()

rules_pkg_dependencies()

rules_foreign_cc_dependencies()

rules_proto_dependencies()

rules_proto_toolchains()

perl_register_toolchains()

go_rules_dependencies()

go_register_toolchains(version = "1.18")

boost_deps()

load("@rules_java//java:repositories.bzl", "rules_java_dependencies", "rules_java_toolchains")

rules_java_dependencies()

rules_java_toolchains()

load("@rules_jvm_external//:repositories.bzl", "rules_jvm_external_deps")

rules_jvm_external_deps()

load("@rules_jvm_external//:setup.bzl", "rules_jvm_external_setup")

rules_jvm_external_setup()

load("@rules_jvm_external//:defs.bzl", "maven_install")

JUNIT_PLATFORM_VERSION = "1.9.2"

JUNIT_JUPITER_VERSION = "5.9.2"

maven_install(
    artifacts = [
        "net.java.dev.jna:jna:aar:5.13.0",
        "com.google.truth:truth:0.32",
        "org.junit.platform:junit-platform-launcher:%s" % JUNIT_PLATFORM_VERSION,
        "org.junit.platform:junit-platform-reporting:%s" % JUNIT_PLATFORM_VERSION,
        "org.junit.jupiter:junit-jupiter-api:%s" % JUNIT_JUPITER_VERSION,
        "org.junit.jupiter:junit-jupiter-params:%s" % JUNIT_JUPITER_VERSION,
        "org.junit.jupiter:junit-jupiter-engine:%s" % JUNIT_JUPITER_VERSION,
    ],
    repositories = [
        "https://maven.aliyun.com/repository/central",
    ],
)

load("@contrib_rules_jvm//:repositories.bzl", "contrib_rules_jvm_deps")

contrib_rules_jvm_deps()

load("@contrib_rules_jvm//:setup.bzl", "contrib_rules_jvm_setup")

contrib_rules_jvm_setup()
