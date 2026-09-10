load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("@bazel_template//bazel:common.bzl", "GLOBAL_COPTS", "GLOBAL_DEFINES", "GLOBAL_LINKOPTS", "GLOBAL_LOCAL_DEFINES")
load("@bazel_template//bazel:rules_fbthrift.bzl", "fbthrift_cpp_gen")

package(default_visibility = ["//visibility:public"])

COPTS = GLOBAL_COPTS + select({
    "@platforms//os:windows": [
        "/Iexternal/libsodium/src/libsodium/include",
        "/Iexternal/fbthrift",
        "/I$(GENDIR)/external/fbthrift",
        "/Iexternal/double-conversion",
        "/Iexternal/xxhash",
        "/Iexternal/com_googlesource_code_re2",
        "/Iexternal/fatal",
        "/I$(GENDIR)/external/folly",
        "/I$(GENDIR)/external/fizz",
        "/Iexternal/folly",
        "/Iexternal/fizz",
        "/Iexternal/wangle",
        "/Iexternal/mvfst",
    ],
    "//conditions:default": [
        "-std=c++20",
        "-isystem external/libsodium/src/libsodium/include",
        "-isystem external/fbthrift",
        "-isystem $(GENDIR)/external/fbthrift",
        "-isystem external/double-conversion",
        "-isystem external/xxhash",
        "-isystem external/com_googlesource_code_re2",
        "-isystem external/fatal",
        "-isystem $(GENDIR)/external/folly",
        "-isystem $(GENDIR)/external/fizz",
        "-isystem external/folly",
        "-isystem external/fizz",
        "-isystem external/wangle",
        "-isystem external/mvfst",
    ],
}) + select({
    "@platforms//os:linux": [
        "-isystem external/libunwind/include",
        "-I$(GENDIR)/external/libunwind/include",
    ],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LOCAL_DEFINES = GLOBAL_LOCAL_DEFINES + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
}) + select({
    "@platforms//os:linux": ["_LARGEFILE64_SOURCE"],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

LINKOPTS = GLOBAL_LINKOPTS + select({
    "@platforms//os:windows": [],
    "//conditions:default": [],
}) + select({
    "@platforms//os:linux": [],
    "@platforms//os:osx": [],
    "@platforms//os:windows": [],
    "//conditions:default": [],
})

DEFINES = GLOBAL_DEFINES

cc_binary(
    name = "compiler_generate_build_templates",
    srcs = ["thrift/compiler/generate/build_templates.cc"],
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
)

genrule(
    name = "templates_cc",
    srcs = ["thrift/compiler/generate/templates"],
    outs = ["thrift/compiler/generate/templates.cc"],
    cmd = "$(location :compiler_generate_build_templates) $(location thrift/compiler/generate/templates) > $@",
    tools = [":compiler_generate_build_templates"],
)

cc_library(
    name = "common",
    srcs = glob([
        "thrift/common/*.cc",
        "thrift/common/detail/*.cc",
    ]),
    hdrs = glob([
        "thrift/common/*.h",
        "thrift/common/detail/*.h",
    ]),
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@fmt",
        "@folly",
    ],
)

cc_library(
    name = "whisker",
    srcs = glob(
        [
            "thrift/compiler/whisker/**/*.cc",
        ],
        exclude = [
            "thrift/compiler/whisker/test/**",
            "thrift/compiler/whisker/tools/**",
        ],
    ),
    hdrs = glob(
        [
            "thrift/compiler/whisker/**/*.h",
        ],
        exclude = [
            "thrift/compiler/whisker/test/**",
            "thrift/compiler/whisker/tools/**",
        ],
    ),
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":common",
        ":compiler_base",
        "@fmt",
        "@folly",
    ],
)

cc_library(
    name = "compiler_generators",
    srcs = glob(
        [
            "thrift/compiler/generate/*.cc",
        ],
        exclude = [
            "thrift/compiler/generate/build_templates.cc",
            "thrift/compiler/generate/schema_populator.cc",
            "thrift/compiler/generate/t_ast_generator.cc",
        ],
    ) + [
        "thrift/compiler/generate/templates.cc",
    ],
    hdrs = glob(
        [
            "thrift/compiler/generate/**/*.h",
            "thrift/compiler/sema/**/*.h",
        ],
        exclude = [
            "thrift/compiler/generate/const_util.h",
            "thrift/compiler/generate/schema_populator.h",
            "thrift/compiler/generate/t_ast_generator.h",
        ],
    ),
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":compiler_ast",
        ":compiler_base",
        ":mustache",
        ":whisker",
        "@com_googlesource_code_re2//:re2",
        "@fmt",
        "@folly",
        "@mvfst",
        "@wangle",
    ],
    alwayslink = True,
)

cc_library(
    name = "compiler_base",
    srcs = glob(
        [
            "thrift/compiler/detail/*.cc",
            "thrift/compiler/parse/*.cc",
            "thrift/compiler/sema/*.cc",
        ],
        exclude = [
            "thrift/compiler/sema/test/**",
        ],
    ) + [
        "thrift/compiler/compiler.cc",
        "thrift/compiler/diagnostic.cc",
        "thrift/compiler/source_location.cc",
    ],
    hdrs = glob([
        "thrift/annotation/bundled_annotations.h",
        "thrift/conformance/if/bundled_conformance_if.h",
        "thrift/compiler/detail/**/*.h",
        "thrift/compiler/generate/**/*.h",
        "thrift/compiler/metrics/**/*.h",
        "thrift/compiler/parse/**/*.h",
        "thrift/compiler/sema/**/*.h",
        "thrift/lib/thrift/bundled_lib_thrift.h",
    ]) + [
        "thrift/compiler/compiler.h",
        "thrift/compiler/diagnostic.h",
        "thrift/compiler/source_location.h",
    ],
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":common",
        ":compiler",
        ":compiler_ast",
        ":mustache",
        "@com_googlesource_code_re2//:re2",
        "@fmt",
        "@folly",
        "@mvfst",
        "@wangle",
    ],
)

cc_library(
    name = "compiler",
    srcs = glob([
        "thrift/compiler/generate/cpp/*.cc",
        "thrift/compiler/generate/csharp/*.cc",
        "thrift/compiler/generate/go/*.cc",
        "thrift/compiler/generate/java/*.cc",
        "thrift/compiler/generate/python/*.cc",
        "thrift/compiler/generate/rust/*.cc",
        "thrift/compiler/generate/swift/*.cc",
    ]),
    hdrs = glob([
        "thrift/compiler/generate/cpp/*.h",
        "thrift/compiler/generate/csharp/*.h",
        "thrift/compiler/generate/go/*.h",
        "thrift/compiler/generate/java/*.h",
        "thrift/compiler/generate/python/*.h",
        "thrift/compiler/generate/rust/*.h",
        "thrift/compiler/generate/swift/*.h",
    ]) + [
        "thrift/compiler/detail/pluggable_functions.h",
        "thrift/compiler/diagnostic.h",
        "thrift/compiler/metrics/metric.h",
        "thrift/compiler/metrics/metrics.h",
        "thrift/compiler/source_location.h",
    ],
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":compiler_ast",
        "@com_googlesource_code_re2//:re2",
        "@fmt",
        "@folly",
        "@mvfst",
        "@wangle",
    ],
)

cc_library(
    name = "compiler_ast",
    srcs = glob(
        ["thrift/compiler/ast/*.cc"],
        exclude = [
            "thrift/compiler/ast/test/**",
            "thrift/compiler/ast/thrift_ast_debug_tree_utils.cc",
        ],
    ),
    hdrs = [
        "thrift/compiler/detail/overload.h",
        "thrift/compiler/source_location.h",
    ] + glob(
        [
            "thrift/compiler/ast/*.h",
        ],
        exclude = [
            "thrift/compiler/ast/thrift_ast_debug_tree_utils.h",
        ],
    ),
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@com_googlesource_code_re2//:re2",
        "@fmt",
        "@folly",
        "@mvfst",
        "@wangle",
    ],
)

cc_library(
    name = "mustache",
    srcs = glob(["thrift/compiler/detail/mustache/**/*.cpp"]),
    hdrs = [
        "thrift/compiler/ast/uri.h",
        "thrift/compiler/source_location.h",
    ] + glob([
        "thrift/compiler/detail/mustache/**/*.h",
    ]),
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        "@com_googlesource_code_re2//:re2",
        "@fmt",
        "@folly",
        "@mvfst",
        "@wangle",
    ],
)

cc_binary(
    name = "thrift1",
    srcs = ["thrift/compiler/main.cc"],
    copts = COPTS,
    defines = DEFINES,
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":compiler_generators",
        "@folly",
        "@libsodium//:sodium",
        "@mvfst",
        "@openssl",
        "@wangle",
    ],
)

filegroup(
    name = "fbthrift_libraries",
    srcs = glob([
        "thrift/annotation/*.thrift",
        "thrift/lib/thrift/*.thrift",
    ]),
)

fbthrift_cpp_gen(
    name = "annotation_thrift_cpp",
    srcs = [
        "thrift/annotation/cpp.thrift",
        "thrift/annotation/go.thrift",
        "thrift/annotation/hack.thrift",
        "thrift/annotation/java.thrift",
        "thrift/annotation/python.thrift",
        "thrift/annotation/thrift.thrift",
    ],
    data = [":fbthrift_libraries"],
    gen_para = [
        "templates",
        "no_metadata",
        "include_prefix=thrift/annotation",
    ],
    includes = [
        ## buildifier: leave-alone
        "-I",
        "external/fbthrift",
        ## buildifier: leave-alone
    ],
    out_dir = "thrift/annotation",
    out_file_name = {
        "thrift/annotation/cpp.thrift": "cpp",
        "thrift/annotation/go.thrift": "go",
        "thrift/annotation/hack.thrift": "hack",
        "thrift/annotation/java.thrift": "java",
        "thrift/annotation/python.thrift": "python",
        "thrift/annotation/thrift.thrift": "thrift",
    },
    plugin = "mstch_cpp2",
)

fbthrift_cpp_gen(
    name = "lib_json_thrift_cpp",
    srcs = [
        "thrift/lib/thrift/RpcMetadata.thrift",
        "thrift/lib/thrift/serverdbginfo.thrift",
    ],
    data = [":fbthrift_libraries"],
    gen_para = [
        "json",
        "no_metadata",
        "include_prefix=thrift/lib/thrift",
    ],
    includes = [
        ## buildifier: leave-alone
        "-I",
        "external/fbthrift",
        ## buildifier: leave-alone
    ],
    out_dir = "thrift/lib/thrift",
    out_file_name = {
        "thrift/lib/thrift/RpcMetadata.thrift": "RpcMetadata",
        "thrift/lib/thrift/serverdbginfo.thrift": "serverdbginfo",
    },
)

fbthrift_cpp_gen(
    name = "lib_rocket_thrift_cpp",
    srcs = [
        "thrift/lib/thrift/RocketUpgrade.thrift",
        "thrift/lib/thrift/any.thrift",
        "thrift/lib/thrift/any_patch.thrift",
        "thrift/lib/thrift/any_patch_detail.thrift",
        "thrift/lib/thrift/any_rep.thrift",
        "thrift/lib/thrift/ast.thrift",
        "thrift/lib/thrift/dynamic.thrift",
        "thrift/lib/thrift/field_mask.thrift",
        "thrift/lib/thrift/id.thrift",
        "thrift/lib/thrift/patch.thrift",
        "thrift/lib/thrift/patch_op.thrift",
        "thrift/lib/thrift/protocol.thrift",
        "thrift/lib/thrift/protocol_detail.thrift",
        "thrift/lib/thrift/record.thrift",
        "thrift/lib/thrift/schema.thrift",
        "thrift/lib/thrift/service_catalog.thrift",
        "thrift/lib/thrift/standard.thrift",
        "thrift/lib/thrift/thrift_catalog_service.thrift",
        "thrift/lib/thrift/type.thrift",
        "thrift/lib/thrift/type_id.thrift",
        "thrift/lib/thrift/type_rep.thrift",
        "thrift/lib/thrift/type_system.thrift",
    ],
    data = [":fbthrift_libraries"],
    gen_para = [
        "no_metadata",
        "include_prefix=thrift/lib/thrift",
    ],
    includes = [
        ## buildifier: leave-alone
        "-I",
        "external/fbthrift",
        ## buildifier: leave-alone
    ],
    out_dir = "thrift/lib/thrift",
    out_file_name = {
        "thrift/lib/thrift/RocketUpgrade.thrift": "RocketUpgrade",
        "thrift/lib/thrift/any.thrift": "any",
        "thrift/lib/thrift/any_patch.thrift": "any_patch",
        "thrift/lib/thrift/any_patch_detail.thrift": "any_patch_detail",
        "thrift/lib/thrift/any_rep.thrift": "any_rep",
        "thrift/lib/thrift/ast.thrift": "ast",
        "thrift/lib/thrift/dynamic.thrift": "dynamic",
        "thrift/lib/thrift/field_mask.thrift": "field_mask",
        "thrift/lib/thrift/id.thrift": "id",
        "thrift/lib/thrift/patch.thrift": "patch",
        "thrift/lib/thrift/patch_op.thrift": "patch_op",
        "thrift/lib/thrift/protocol.thrift": "protocol",
        "thrift/lib/thrift/protocol_detail.thrift": "protocol_detail",
        "thrift/lib/thrift/record.thrift": "record",
        "thrift/lib/thrift/schema.thrift": "schema",
        "thrift/lib/thrift/service_catalog.thrift": "service_catalog",
        "thrift/lib/thrift/standard.thrift": "standard",
        "thrift/lib/thrift/thrift_catalog_service.thrift": "thrift_catalog_service",
        "thrift/lib/thrift/type.thrift": "type",
        "thrift/lib/thrift/type_id.thrift": "type_id",
        "thrift/lib/thrift/type_rep.thrift": "type_rep",
        "thrift/lib/thrift/type_system.thrift": "type_system",
    },
    service_out_file_name = {
        "thrift/lib/thrift/RocketUpgrade.thrift": "RocketUpgrade",
    },
)

fbthrift_cpp_gen(
    name = "lib_meta_thrift_cpp",
    srcs = [
        "thrift/lib/thrift/frozen.thrift",
        "thrift/lib/thrift/metadata.thrift",
    ],
    data = [":fbthrift_libraries"],
    gen_para = ["include_prefix=thrift/lib/thrift"],
    includes = [
        ## buildifier: leave-alone
        "-I",
        "external/fbthrift",
        ## buildifier: leave-alone
    ],
    out_dir = "thrift/lib/thrift",
    out_file_name = {
        "thrift/lib/thrift/frozen.thrift": "frozen",
        "thrift/lib/thrift/metadata.thrift": "metadata",
    },
    plugin = "mstch_cpp2",
    service_out_file_name = {
        "thrift/lib/thrift/metadata.thrift": "ThriftMetadataService",
    },
)

fbthrift_cpp_gen(
    name = "conformance_thrift_cpp",
    srcs = [
        "thrift/conformance/if/any.thrift",
        "thrift/conformance/if/conformance.thrift",
        "thrift/conformance/if/patch_data.thrift",
        "thrift/conformance/if/protocol.thrift",
        "thrift/conformance/if/serialization.thrift",
        "thrift/conformance/if/type.thrift",
    ],
    data = [":fbthrift_libraries"] + [
        "thrift/conformance/if/any.thrift",
        "thrift/conformance/if/conformance.thrift",
        "thrift/conformance/if/patch_data.thrift",
        "thrift/conformance/if/protocol.thrift",
        "thrift/conformance/if/serialization.thrift",
        "thrift/conformance/if/type.thrift",
    ],
    gen_para = [
        "no_metadata",
        "include_prefix=thrift/conformance/if",
    ],
    includes = [
        ## buildifier: leave-alone
        "-I",
        "external/fbthrift",
        ## buildifier: leave-alone
    ],
    out_dir = "thrift/conformance/if",
    out_file_name = {
        "thrift/conformance/if/any.thrift": "any",
        "thrift/conformance/if/conformance.thrift": "conformance",
        "thrift/conformance/if/patch_data.thrift": "patch_data",
        "thrift/conformance/if/protocol.thrift": "protocol",
        "thrift/conformance/if/serialization.thrift": "serialization",
        "thrift/conformance/if/type.thrift": "type",
    },
    service_out_file_name = {
        "thrift/conformance/if/conformance.thrift": "ConformanceService",
    },
)

fbthrift_cpp_gen(
    name = "schema_thrift_cpp",
    srcs = [
        "thrift/lib/cpp2/schema/syntax_graph.thrift",
    ],
    data = [":fbthrift_libraries"],
    gen_para = [
        "no_metadata",
        "include_prefix=thrift/lib/cpp2/schema",
    ],
    includes = [
        ## buildifier: leave-alone
        "-I",
        "external/fbthrift",
        ## buildifier: leave-alone
    ],
    out_dir = "thrift/lib/cpp2/schema",
    out_file_name = {
        "thrift/lib/cpp2/schema/syntax_graph.thrift": "syntax_graph",
    },
)

cc_library(
    name = "fbthrift",
    srcs = [
        ":annotation_thrift_cpp",
        ":conformance_thrift_cpp",
        ":lib_json_thrift_cpp",
        ":lib_meta_thrift_cpp",
        ":lib_rocket_thrift_cpp",
        ":schema_thrift_cpp",
    ] + glob(
        [
            "thrift/annotation/**/*.cpp",
            "thrift/lib/cpp/**/*.cpp",
            "thrift/lib/cpp2/**/*.cpp",
            "thrift/lib/thrift/**/*.cpp",
        ],
        exclude = [
            "thrift/lib/cpp2/protocol/Patch.cpp",  # conflict with fmt
            "thrift/lib/cpp2/async/*.cpp",  # depend proxygen
            "thrift/lib/cpp2/fast_thrift/**/*.cpp",  # experimental subsystem
            "thrift/lib/cpp2/transport/http2/client/H2ClientConnection.cpp",  # depend proxygen
            "thrift/lib/cpp2/transport/http2/client/ThriftTransactionHandler.cpp",  # depend proxygen
            "thrift/lib/cpp2/transport/http2/common/H2Channel.cpp",  # depend proxygen
            "thrift/lib/cpp2/transport/http2/common/HTTP2RoutingHandler.cpp",  # depend proxygen
            "thrift/lib/cpp2/transport/http2/common/SingleRpcChannel.cpp",  # depend proxygen
            "thrift/lib/cpp2/transport/http2/server/ThriftRequestHandler.cpp",  # depend proxygen
            "thrift/lib/cpp2/transport/util/ConnectionThread.cpp",  # depend proxygen
            "thrift/lib/cpp2/type/AnyDebugWriter.cpp",  # -std=c++20
            "thrift/**/demo/**",
            "thrift/**/test/**",
            "thrift/**/tests/**",
            "thrift/**/testutil/**",
            "thrift/**/example/**",
            "thrift/**/bench/**",
            "thrift/**/benchmark/**",
            "thrift/**/benchmarks/**",
            "thrift/lib/cpp2/util/gtest/**",
            "thrift/**/*Test.cpp",
        ],
    ) + [
        "thrift/conformance/cpp2/Any.cpp",
        "thrift/conformance/cpp2/AnyRef.cpp",
        "thrift/conformance/cpp2/AnyRegistry.cpp",
        "thrift/conformance/cpp2/AnySerializer.cpp",
        "thrift/conformance/cpp2/Protocol.cpp",
        "thrift/conformance/cpp2/ThriftTypeInfo.cpp",
        "thrift/lib/cpp2/async/AsyncClient.cpp",
        "thrift/lib/cpp2/async/AsyncProcessor.cpp",
        "thrift/lib/cpp2/async/AsyncProcessorHelper.cpp",
        "thrift/lib/cpp2/async/ClientChannel.cpp",
        "thrift/lib/cpp2/async/ClientSinkBridge.cpp",
        "thrift/lib/cpp2/async/ClientStreamBridge.cpp",
        "thrift/lib/cpp2/async/Cpp2Channel.cpp",
        "thrift/lib/cpp2/async/DuplexChannel.cpp",
        "thrift/lib/cpp2/async/FramingHandler.cpp",
        "thrift/lib/cpp2/async/HeaderChannel.cpp",
        "thrift/lib/cpp2/async/HeaderChannelTrait.cpp",
        "thrift/lib/cpp2/async/HeaderClientChannel.cpp",
        "thrift/lib/cpp2/async/HeaderServerChannel.cpp",
        "thrift/lib/cpp2/async/HibernatingRequestChannel.cpp",
        "thrift/lib/cpp2/async/Interaction.cpp",
        "thrift/lib/cpp2/async/MultiplexAsyncProcessor.cpp",
        "thrift/lib/cpp2/async/PooledRequestChannel.cpp",
        "thrift/lib/cpp2/async/ReconnectingRequestChannel.cpp",
        "thrift/lib/cpp2/async/RequestCallback.cpp",
        "thrift/lib/cpp2/async/RequestChannel.cpp",
        "thrift/lib/cpp2/async/ResponseChannel.cpp",
        "thrift/lib/cpp2/async/RetryingRequestChannel.cpp",
        "thrift/lib/cpp2/async/RocketClientChannel.cpp",
        "thrift/lib/cpp2/async/RpcOptions.cpp",
        "thrift/lib/cpp2/async/RpcTypes.cpp",
        "thrift/lib/cpp2/async/ServerRequestData.cpp",
        "thrift/lib/cpp2/async/ServerSinkBridge.cpp",
    ],
    hdrs = [
        "thrift/conformance/cpp2/Any.h",
        "thrift/conformance/cpp2/AnyRef.h",
        "thrift/conformance/cpp2/AnyRegistry.h",
        "thrift/conformance/cpp2/AnySerializer.h",
        "thrift/conformance/cpp2/AnyStructSerializer.h",
        "thrift/conformance/cpp2/Protocol.h",
        "thrift/conformance/cpp2/ThriftTypeInfo.h",
        "thrift/conformance/cpp2/internal/AnyRegistry.h",
        "thrift/conformance/cpp2/internal/AnyStructSerializer.h",
    ] + glob(
        [
            "thrift/annotation/**/*.h",
            "thrift/annotation/**/*.tcc",
            "thrift/lib/cpp2/**/*.h",
            "thrift/lib/cpp/**/*.h",
            "thrift/lib/thrift/**/*.h",
            "thrift/lib/thrift/**/*.tcc",
        ],
        exclude = [
            "thrift/**/test/**",
            "thrift/**/example/**",
            "thrift/**/bench/**",
            "thrift/**/benchmark/**",
            "thrift/**/benchmarks/**",
        ],
    ),
    copts = COPTS,
    defines = DEFINES,
    includes = ["."],
    linkopts = LINKOPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":common",
        "@fatal",
        "@folly",
        "@mvfst",
        "@wangle",
        "@xxhash",
        "@yaml-cpp",
    ],
)
