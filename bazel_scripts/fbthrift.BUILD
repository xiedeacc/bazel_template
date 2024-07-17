load("@rules_foreign_cc//foreign_cc:defs.bzl", "configure_make", "configure_make_variant")
load("//bazel_scripts:rules_fbthrift.bzl", "fbthrift_cpp_gen", "fbthrift_service_cpp_gen")

package(default_visibility = ["//visibility:public"])

COPTS = [
    "-isystem external/fbthrift",
    "-isystem $(BINDIR)/external/fbthrift",
    "-isystem external/double-conversion",
    "-isystem external/xxhash",
    "-isystem external/com_googlesource_code_re2",
    "-isystem external/fatal",
    "-isystem $(BINDIR)/external/folly",
    "-isystem $(BINDIR)/external/fizz",
    "-isystem external/folly",
    "-isystem external/fizz",
    "-isystem external/wangle",
    "-isystem external/mvfst",
    "-std=c++17",
    "-fsized-deallocation",
]

LOCAL_DEFINES = [
    "NDEBUG",
]

cc_binary(
    name = "compiler_generate_build_templates",
    srcs = ["thrift/compiler/generate/build_templates.cc"],
    copts = COPTS,
)

genrule(
    name = "templates_cc",
    srcs = ["thrift/compiler/generate/templates"],
    outs = ["thrift/compiler/generate/templates.cc"],
    cmd = "$(location :compiler_generate_build_templates) $(location thrift/compiler/generate/templates) > $@",
    tools = [":compiler_generate_build_templates"],
)

cc_library(
    name = "compiler_generators",
    srcs = [
        "thrift/compiler/generate/common.cc",
        "thrift/compiler/generate/json.cc",
        "thrift/compiler/generate/mstch_objects.cc",
        "thrift/compiler/generate/t_android_generator.cc",
        "thrift/compiler/generate/t_cocoa_generator.cc",
        "thrift/compiler/generate/t_concat_generator.cc",
        "thrift/compiler/generate/t_generator.cc",
        "thrift/compiler/generate/t_hack_generator.cc",
        "thrift/compiler/generate/t_java_deprecated_generator.cc",
        "thrift/compiler/generate/t_js_generator.cc",
        "thrift/compiler/generate/t_json_experimental_generator.cc",
        "thrift/compiler/generate/t_json_generator.cc",
        "thrift/compiler/generate/t_mstch_cpp2_generator.cc",
        "thrift/compiler/generate/t_mstch_generator.cc",
        "thrift/compiler/generate/t_mstch_go_generator.cc",
        "thrift/compiler/generate/t_mstch_html_generator.cc",
        "thrift/compiler/generate/t_mstch_java_generator.cc",
        "thrift/compiler/generate/t_mstch_py3_generator.cc",
        "thrift/compiler/generate/t_mstch_pyi_generator.cc",
        "thrift/compiler/generate/t_mstch_python_capi_generator.cc",
        "thrift/compiler/generate/t_mstch_python_generator.cc",
        "thrift/compiler/generate/t_mstch_rust_generator.cc",
        "thrift/compiler/generate/t_py_generator.cc",
        "thrift/compiler/generate/t_starlark_generator.cc",
        "thrift/compiler/generate/templates.cc",
    ],
    hdrs = [
        "thrift/compiler/generate/common.h",
        "thrift/compiler/generate/json.h",
        "thrift/compiler/generate/mstch_objects.h",
        "thrift/compiler/generate/t_concat_generator.h",
        "thrift/compiler/generate/t_generator.h",
        "thrift/compiler/generate/t_java_deprecated_generator.h",
        "thrift/compiler/generate/t_mstch_generator.h",
        "thrift/compiler/generate/templates.h",
        "thrift/compiler/sema/ast_validator.h",
    ],
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
        ":compiler_ast",
        ":compiler_base",
        ":mustache",
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
    srcs = [
        "thrift/compiler/compiler.cc",
        "thrift/compiler/detail/pluggable_functions.cc",
        "thrift/compiler/detail/system.cc",
        "thrift/compiler/diagnostic.cc",
        "thrift/compiler/lib/schematizer.cc",
        "thrift/compiler/parse/lexer.cc",
        "thrift/compiler/parse/parse_ast.cc",
        "thrift/compiler/parse/parser.cc",
        "thrift/compiler/parse/token.cc",
        "thrift/compiler/sema/const_checker.cc",
        "thrift/compiler/sema/explicit_include_validator.cc",
        "thrift/compiler/sema/patch_mutator.cc",
        "thrift/compiler/sema/scope_validator.cc",
        "thrift/compiler/sema/standard_mutator.cc",
        "thrift/compiler/sema/standard_validator.cc",
        "thrift/compiler/source_location.cc",
    ],
    hdrs = [
        "thrift/compiler/compiler.h",
        "thrift/compiler/detail/pluggable_functions.h",
        "thrift/compiler/detail/system.h",
        "thrift/compiler/diagnostic.h",
        "thrift/compiler/generate/t_generator.h",
        "thrift/compiler/lib/cpp2/util.h",
        "thrift/compiler/source_location.h",
    ] + glob([
        "thrift/compiler/sema/**/*.h",
        "thrift/compiler/parse/**/*.h",
    ]),
    copts = COPTS,
    local_defines = LOCAL_DEFINES,
    deps = [
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
    srcs = [
        "thrift/compiler/gen/cpp/gen.cc",
        "thrift/compiler/gen/cpp/namespace_resolver.cc",
        "thrift/compiler/gen/cpp/reference_type.cc",
        "thrift/compiler/gen/cpp/type_resolver.cc",
        "thrift/compiler/lib/cpp2/util.cc",
        "thrift/compiler/lib/go/util.cc",
        "thrift/compiler/lib/java/util.cc",
        "thrift/compiler/lib/py3/util.cpp",
        "thrift/compiler/lib/python/util.cc",
        "thrift/compiler/lib/rust/util.cc",
    ],
    hdrs = [
        "thrift/compiler/gen/cpp/gen.h",
        "thrift/compiler/gen/cpp/namespace_resolver.h",
        "thrift/compiler/gen/cpp/reference_type.h",
        "thrift/compiler/gen/cpp/type_resolver.h",
        "thrift/compiler/source_location.h",
    ] + glob([
        "thrift/compiler/lib/**/*.h",
    ]),
    copts = COPTS,
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
    srcs = [
        "thrift/compiler/ast/t_const.cc",
        "thrift/compiler/ast/t_const_value.cc",
        "thrift/compiler/ast/t_enum.cc",
        "thrift/compiler/ast/t_function.cc",
        "thrift/compiler/ast/t_interface.cc",
        "thrift/compiler/ast/t_named.cc",
        "thrift/compiler/ast/t_node.cc",
        "thrift/compiler/ast/t_package.cc",
        "thrift/compiler/ast/t_paramlist.cc",
        "thrift/compiler/ast/t_primitive_type.cc",
        "thrift/compiler/ast/t_program.cc",
        "thrift/compiler/ast/t_scope.cc",
        "thrift/compiler/ast/t_structured.cc",
        "thrift/compiler/ast/t_type.cc",
        "thrift/compiler/ast/t_typedef.cc",
    ],
    hdrs = [
        "thrift/compiler/lib/uri.h",
        "thrift/compiler/source_location.h",
    ] + glob([
        "thrift/compiler/ast/*.h",
    ]),
    copts = COPTS,
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
    srcs = [
        "thrift/compiler/detail/mustache/mstch.cpp",
        "thrift/compiler/detail/mustache/render_context.cpp",
        "thrift/compiler/detail/mustache/state/in_section.cpp",
        "thrift/compiler/detail/mustache/state/outside_section.cpp",
        "thrift/compiler/detail/mustache/template_type.cpp",
        "thrift/compiler/detail/mustache/token.cpp",
        "thrift/compiler/detail/mustache/utils.cpp",
    ],
    hdrs = [
        "thrift/compiler/lib/uri.h",
        "thrift/compiler/source_location.h",
    ] + glob([
        "thrift/compiler/detail/mustache/**/*.h",
    ]),
    copts = COPTS,
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
    deps = [
        ":compiler_generators",
        "@folly",
        "@mvfst",
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
        ".",
        ## buildifier: leave-alone
    ],
    out_dir = "thrift/lib/thrift",
    out_files = {
        "thrift/lib/thrift/frozen.thrift": "frozen",
        "thrift/lib/thrift/metadata.thrift": "metadata",
    },
    plugin = "mstch_cpp2",
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
        ".",
        ## buildifier: leave-alone
    ],
    out_dir = "thrift/annotation",
    out_files = {
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
    name = "lib_reflection_thrift_cpp",
    srcs = [
        "thrift/lib/thrift/reflection.thrift",
    ],
    data = [":fbthrift_libraries"],
    gen_para = [
        "templates",
        "no_metadata",
        "include_prefix=thrift/lib/thrift",
    ],
    includes = [
        ## buildifier: leave-alone
        "-I",
        ".",
        ## buildifier: leave-alone
    ],
    out_dir = "thrift/lib/thrift",
    out_files = {
        "thrift/lib/thrift/reflection.thrift": "reflection",
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
        ".",
        ## buildifier: leave-alone
    ],
    out_dir = "thrift/lib/thrift",
    out_files = {
        "thrift/lib/thrift/RpcMetadata.thrift": "RpcMetadata",
        "thrift/lib/thrift/serverdbginfo.thrift": "serverdbginfo",
    },
)

fbthrift_cpp_gen(
    name = "lib_rocket_thrift_cpp",
    srcs = [
        "thrift/lib/thrift/RocketUpgrade.thrift",
        "thrift/lib/thrift/any_rep.thrift",
        "thrift/lib/thrift/field_mask.thrift",
        "thrift/lib/thrift/id.thrift",
        "thrift/lib/thrift/patch.thrift",
        "thrift/lib/thrift/patch_op.thrift",
        "thrift/lib/thrift/protocol.thrift",
        "thrift/lib/thrift/protocol_detail.thrift",
        "thrift/lib/thrift/standard.thrift",
        "thrift/lib/thrift/type.thrift",
        "thrift/lib/thrift/type_rep.thrift",
    ],
    data = [":fbthrift_libraries"],
    gen_para = [
        "no_metadata",
        "include_prefix=thrift/lib/thrift",
    ],
    includes = [
        ## buildifier: leave-alone
        "-I",
        ".",
        ## buildifier: leave-alone
    ],
    out_dir = "thrift/lib/thrift",
    out_files = {
        "thrift/lib/thrift/RocketUpgrade.thrift": "RocketUpgrade",
        "thrift/lib/thrift/any_rep.thrift": "any_rep",
        "thrift/lib/thrift/field_mask.thrift": "field_mask",
        "thrift/lib/thrift/id.thrift": "id",
        "thrift/lib/thrift/patch.thrift": "patch",
        "thrift/lib/thrift/patch_op.thrift": "patch_op",
        "thrift/lib/thrift/protocol.thrift": "protocol",
        "thrift/lib/thrift/protocol_detail.thrift": "protocol_detail",
        "thrift/lib/thrift/standard.thrift": "standard",
        "thrift/lib/thrift/type.thrift": "type",
        "thrift/lib/thrift/type_rep.thrift": "type_rep",
    },
)

fbthrift_service_cpp_gen(
    name = "lib_rocket_service_thrift_cpp",
    srcs = [
        "thrift/lib/thrift/RocketUpgrade.thrift",
        "thrift/lib/thrift/metadata.thrift",
    ],
    data = [":fbthrift_libraries"],
    gen_para = [
        "no_metadata",
        "include_prefix=thrift/lib/thrift",
    ],
    includes = [
        ## buildifier: leave-alone
        "-I",
        ".",
        ## buildifier: leave-alone
    ],
    out_dir = "thrift/lib/thrift",
    out_files = {
        "thrift/lib/thrift/RocketUpgrade.thrift": "RocketUpgrade",
        "thrift/lib/thrift/metadata.thrift": "ThriftMetadataService",
    },
)

cc_library(
    name = "lib",
    srcs = [
        ":annotation_thrift_cpp",
        ":lib_json_thrift_cpp",
        ":lib_meta_thrift_cpp",
        ":lib_reflection_thrift_cpp",
        ":lib_rocket_service_thrift_cpp",
        ":lib_rocket_thrift_cpp",
    ] + glob(
        [
            "thrift/annotation/**/*.cpp",
            "thrift/lib/cpp/**/*.cpp",
            "thrift/lib/cpp2/**/*.cpp",
            "thrift/lib/thrift/**/*.cpp",
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
            "thrift/lib/cpp2/async/ServerGeneratorStream.cpp",
            "thrift/lib/cpp2/async/ServerRequestData.cpp",
            "thrift/lib/cpp2/async/ServerSinkBridge.cpp",
            "thrift/lib/cpp2/async/ThreadBoundAdaptorChannel.cpp",
            "thrift/conformance/cpp2/Any.cpp",
            "thrift/conformance/cpp2/AnyRef.cpp",
            "thrift/conformance/cpp2/AnyRegistry.cpp",
            "thrift/conformance/cpp2/AnySerializer.cpp",
            "thrift/conformance/cpp2/Protocol.cpp",
            "thrift/conformance/cpp2/ThriftTypeInfo.cpp",
            "thrift/conformance/if/gen-cpp2/*.cpp",
        ],
        exclude = [
            "thrift/lib/cpp2/protocol/Patch.cpp",  # conflict with fmt
            "thrift/lib/cpp2/async/*.cpp",  # depend proxygen
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
            "thrift/lib/cpp2/util/gtest/**",
            "thrift/**/*Test.cpp",
        ],
    ),
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
            "thrift/conformance/if/gen-cpp2/*.h",
            "thrift/conformance/if/gen-cpp2/*.tcc",
        ],
        exclude = [
            "thrift/**/test/**",
            "thrift/**/example/**",
        ],
    ),
    copts = COPTS,
    deps = [
        "@fatal",
        "@folly",
        "@mvfst",
        "@wangle",
        "@xxhash",
        "@yaml-cpp",
    ],
)
