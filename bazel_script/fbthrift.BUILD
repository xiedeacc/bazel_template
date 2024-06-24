package(default_visibility = ["//visibility:public"])

cc_library(
    name = "lib",
    srcs = [
    ] + glob(
        [
            "thrift/lib/**/*.cc",
            "thrift/lib/**/*.cpp",
        ],
        exclude = [
            "thrift/lib/cpp2/transport/http2/common/H2Channel.cpp",
            "thrift/lib/cpp2/util/gtest/**",
            "thrift/lib/thrift/detail/protocol.cpp",
            "thrift/lib/cpp2/protocol/detail/index.cpp",
            "thrift/lib/cpp2/transport/core/testutil/**",
            "thrift/lib/cpp2/transport/http2/common/testutil/**",
            "thrift/lib/cpp2/transport/http2/common/HTTP2RoutingHandler.cpp",
            "thrift/lib/cpp2/protocol/FieldPath.cpp",
            "thrift/lib/cpp2/transport/http2/common/SingleRpcChannel.cpp",
            "thrift/lib/cpp2/transport/util/ConnectionThread.cpp",
            "thrift/lib/cpp2/op/Testing.cpp",
            "thrift/lib/cpp2/protocol/ApplyPatchToSerializedDataBenchmark.cpp",
            "thrift/lib/cpp2/protocol/FieldMask.cpp",
            "thrift/lib/cpp2/protocol/detail/FieldMask.cpp",
            "thrift/lib/cpp2/protocol/Object.cpp",
            "thrift/lib/cpp2/protocol/detail/Object.cpp",
            "thrift/lib/cpp2/transport/http2/server/**",
            "thrift/lib/cpp2/transport/http2/client/**",
            "thrift/lib/cpp2/async/tests/**",
            "thrift/lib/cpp2/protocol/FieldMaskRef.cpp",
            "thrift/lib/cpp2/protocol/Patch.cpp",
            "thrift/lib/**/*Test.cpp",
            "thrift/lib/cpp2/async/HTTPClientChannel.cpp",
            "thrift/lib/cpp2/reflection/demo/**",
            "thrift/lib/android_lite/**",
            "thrift/lib/go/**",
            "thrift/lib/hack/**",
            "thrift/lib/java/**",
            "thrift/lib/javadeprecated/**",
            "thrift/lib/py/**",
            "thrift/lib/py3/**",
            "thrift/lib/python/**",
            "thrift/lib/rust/**",
            "thrift/lib/**/test",
            "thrift/lib/**/test/**",
            "thrift/lib/**/benchmark",
            "thrift/lib/**/benchmark/**",
            "thrift/lib/**/example",
            "thrift/lib/**/example/**",
            "thrift/lib/**/*_test.cc",
        ],
    ),
    hdrs = [
        "thrift/conformance/cpp2/AnySerializer.h",
        "thrift/conformance/cpp2/AnyRef.h",
        "thrift/conformance/cpp2/internal/AnyRegistry.h",
        "thrift/conformance/cpp2/AnyRegistry.h",
    ] + glob(
        [
            #"thrift/conformance/cpp2/**/*.h",
            "thrift/lib/**/*.h",
            "thrift/lib/**/*.tcc",
        ],
        exclude = [
            "thrift/lib/cpp2/async/tests/**",
            "thrift/lib/cpp2/protocol/Object.h",
            "thrift/lib/cpp2/protocol/FieldMask.h",
            "thrift/lib/cpp2/protocol/FieldPath.h",
            "thrift/lib/cpp2/protocol/detail/Object.h",
            "thrift/lib/cpp2/protocol/detail/FieldMask.h",
            "thrift/lib/cpp2/protocol/FieldMaskRef.h",
            "thrift/lib/cpp2/reflection/demo/**",
            "thrift/lib/android_lite/**",
            "thrift/lib/go/**",
            "thrift/lib/hack/**",
            "thrift/lib/java/**",
            "thrift/lib/javadeprecated/**",
            "thrift/lib/py/**",
            "thrift/lib/py3/**",
            "thrift/lib/python/**",
            "thrift/lib/rust/**",
            "thrift/lib/**/test",
            "thrift/lib/**/test/**",
            "thrift/lib/**/benchmark",
            "thrift/lib/**/benchmark/**",
            "thrift/lib/**/example",
            "thrift/lib/**/example/**",
        ],
    ),
    copts = [
        "-I.",
        "-std=c++17",
    ],
    includes = [
        "external/com_github_google_re2",
        "external/folly",
    ],
    linkopts = [
        "-pthread",
        "-ldl",
        "-lstdc++",
        "-lgcc",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_facebook_zstd//:libzstd",
        "@com_github_fmtlib_fmt//:fmt",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_glog_glog//:glog",
        "@com_github_google_re2//:re2",
        "@fizz",
        "@folly",
        "@mvfst",
        "@wangle",
    ],
)

cc_library(
    name = "compilerlib",
    srcs = [
    ] + glob(
        [
            "thrift/compiler/**/*.cc",
            "thrift/compiler/**/*.cpp",
        ],
        exclude = [
            "thrift/compiler/main.cc",
            "thrift/compiler/codemod/**",
            "thrift/compiler/generate/t_ast_generator.cc",
            "thrift/compiler/generate/build_templates.cc",
            "thrift/compiler/**/test/**",
            "thrift/compiler/**/benchmark/**",
            "thrift/compiler/**/example/**",
            "thrift/compiler/**/*_test.cc",
        ],
    ),
    hdrs = [
    ] + glob(
        [
            "thrift/compiler/**/*.h",
        ],
        exclude = [
            "thrift/compiler/lib/const_util.h",
            "thrift/compiler/gen/testing.h",
            "thrift/compiler/**/test",
            "thrift/compiler/**/test/**",
            "thrift/compiler/**/benchmark",
            "thrift/compiler/**/benchmark/**",
            "thrift/compiler/**/example",
            "thrift/compiler/**/example/**",
        ],
    ),
    copts = [
        "-I.",
        "-std=c++17",
    ],
    includes = [
        "external/com_github_google_re2",
        "external/folly",
    ],
    linkopts = [
        "-pthread",
        "-lstdc++",
        "-lgcc",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":lib",
        "@com_github_facebook_zstd//:libzstd",
        "@com_github_fmtlib_fmt//:fmt",
        "@com_github_gflags_gflags//:gflags",
        "@com_github_glog_glog//:glog",
        "@com_github_google_re2//:re2",
        "@fizz",
        "@folly",
        "@mvfst",
        "@wangle",
    ],
)

cc_binary(
    name = "compiler_generate_build_templates",
    srcs = [
        "thrift/compiler/generate/build_templates.cc",
    ],
    copts = [
        "-std=c++17",
    ],
    includes = [
    ],
    linkopts = [
        "-lc",
        "-pthread",
        "-ldl",
        "-lstdc++",
        "-lgcc",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":compilerlib",
    ],
)

cc_binary(
    name = "thrift1",
    srcs = [
        "thrift/compiler/main.cc",
    ],
    copts = [
        "-I.",
        "-std=c++17",
    ],
    includes = [
    ],
    linkopts = [
        "-lc",
        "-pthread",
        "-ldl",
        "-lstdc++",
        "-lgcc",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":compilerlib",
    ],
)
