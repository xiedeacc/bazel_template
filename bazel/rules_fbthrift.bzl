def list_to_set(lst):
    return {item: True for item in lst}

# Check if an element is in the set-like dictionary
def element_in_set(set_dict, element):
    return element in set_dict

def remove_prefix(s, prefix):
    if s.startswith(prefix):
        return s[len(prefix):]
    return s

def _fbthrift_cpp_gen_impl(ctx):
    suffixs = [
        "_clients.h",
        "_constants.cpp",
        "_constants.h",
        "_data.cpp",
        "_data.h",
        "_for_each_field.h",
        "_handlers.h",
        "_metadata.h",
        "_types.cpp",
        "_types.h",
        "_types.tcc",
        "_types_custom_protocol.h",
        "_types_fwd.h",
        "_visit_by_thrift_field_metadata.h",
        "_visit_union.h",
        "_visitation.h",
    ]

    gen_para_dict = list_to_set(ctx.attr.gen_para)
    if not element_in_set(gen_para_dict, "no_metadata"):
        suffixs.append("_metadata.cpp")

    gen_para = ctx.attr.plugin + ":" + ",".join(ctx.attr.gen_para)
    output_files = []
    if len(ctx.attr.out_files) != len(ctx.files.srcs):
        fail("output_files size not equal to srcs")
    for thrift_file in ctx.files.srcs:
        file_name = ctx.attr.out_files[remove_prefix(thrift_file.path, "external/{}/".format(ctx.label.workspace_name))]
        outputs = []
        for suffix in suffixs:
            output_file = ctx.actions.declare_file("{}/gen-cpp2/{}{}".format(ctx.attr.out_dir, file_name, suffix))
            outputs.append(output_file)
            output_files.append(output_file)
        ctx.actions.run(
            inputs = [thrift_file] + ctx.files.data,
            outputs = outputs,
            arguments = [
                "--gen",
                gen_para,
                "-o",
                "{}/external/{}/{}".format(outputs[0].root.path, ctx.label.workspace_name, ctx.attr.out_dir),
            ] + ctx.attr.includes + [
                thrift_file.path,
            ],
            executable = ctx.executable.thrift,
        )

    return DefaultInfo(files = depset(output_files))

fbthrift_cpp_gen = rule(
    implementation = _fbthrift_cpp_gen_impl,
    attrs = {
        "srcs": attr.label_list(allow_files = [".thrift"], mandatory = True),
        "data": attr.label_list(mandatory = True, allow_files = [".thrift", ".h"]),
        "thrift": attr.label(executable = True, cfg = "host", default = "@fbthrift//:thrift1"),
        "gen_para": attr.string_list(mandatory = True),
        "includes": attr.string_list(mandatory = True),
        "plugin": attr.string(mandatory = False, default = "mstch_cpp2"),
        "out_dir": attr.string(mandatory = True),
        "out_files": attr.string_dict(mandatory = True),
    },
)

def _fbthrift_service_cpp_gen_impl(ctx):
    suffixs = [
        ".h",
        ".cpp",
        ".tcc",
        "AsyncClient.cpp",
        "AsyncClient.h",
        "_processmap_binary.cpp",
        "_processmap_compact.cpp",
        "_custom_protocol.h",
    ]

    gen_para = ctx.attr.plugin + ":" + ",".join(ctx.attr.gen_para)
    output_files = []
    if len(ctx.attr.out_files) != len(ctx.files.srcs):
        fail("output_files size not equal to srcs")
    for thrift_file in ctx.files.srcs:
        file_name = ctx.attr.out_files[remove_prefix(thrift_file.path, "external/{}/".format(ctx.label.workspace_name))]
        outputs = []
        for suffix in suffixs:
            output_file = ctx.actions.declare_file("{}/gen-cpp2/{}{}".format(ctx.attr.out_dir, file_name, suffix))
            outputs.append(output_file)
            output_files.append(output_file)
        ctx.actions.run(
            inputs = [thrift_file] + ctx.files.data,
            outputs = outputs,
            arguments = [
                "--gen",
                gen_para,
                "-o",
                "{}/external/{}/{}".format(outputs[0].root.path, ctx.label.workspace_name, ctx.attr.out_dir),
            ] + ctx.attr.includes + [
                thrift_file.path,
            ],
            executable = ctx.executable.thrift,
        )

    return DefaultInfo(files = depset(output_files))

fbthrift_service_cpp_gen = rule(
    implementation = _fbthrift_service_cpp_gen_impl,
    attrs = {
        "srcs": attr.label_list(allow_files = [".thrift"], mandatory = True),
        "data": attr.label_list(mandatory = True, allow_files = [".thrift", ".h"]),
        "thrift": attr.label(executable = True, cfg = "host", default = "@fbthrift//:thrift1"),
        "gen_para": attr.string_list(mandatory = True),
        "includes": attr.string_list(mandatory = True),
        "plugin": attr.string(mandatory = False, default = "mstch_cpp2"),
        "out_dir": attr.string(mandatory = True),
        "out_files": attr.string_dict(mandatory = True),
    },
)
