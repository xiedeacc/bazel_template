def list_to_set(lst):
    return {item: True for item in lst}

def element_in_set(set_dict, element):
    return element in set_dict

def remove_prefix(s, prefix):
    if s.startswith(prefix):
        return s[len(prefix):]
    return s

def external_repo_root(path):
    if path.startswith("external/"):
        parts = path.split("/")
        if len(parts) >= 2:
            return "/".join(parts[:2])
    return ""

def add_include_arg(args, include_root):
    if include_root:
        args.extend(["-I", include_root])

def normalize_include_path(path, current_workspace_root):
    if not path.startswith("external/"):
        return path
    apparent_repo = path.split("/")[1]
    if current_workspace_root.endswith(apparent_repo) or current_workspace_root.endswith("+" + apparent_repo):
        return current_workspace_root
    return path

def _fbthrift_cpp_gen_impl(ctx):
    service_suffixs = [
        ".h",
        ".cpp",
        ".tcc",
        "AsyncClient.cpp",
        "AsyncClient.h",
        "_processmap_binary.cpp",
        "_processmap_compact.cpp",
        "_custom_protocol.h",
    ]
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
    if len(ctx.attr.out_file_name) != len(ctx.files.srcs):
        fail("output_files size not equal to srcs")
    for thrift_file in ctx.files.srcs:
        outputs = []
        workspace_root = ctx.label.workspace_root
        file_name = remove_prefix(thrift_file.path, workspace_root + "/")
        if file_name in ctx.attr.service_out_file_name:
            service_out_file_name = ctx.attr.service_out_file_name[file_name]
            for suffix in service_suffixs:
                output_file = ctx.actions.declare_file("{}/gen-cpp2/{}{}".format(ctx.attr.out_dir, service_out_file_name, suffix))
                outputs.append(output_file)
                output_files.append(output_file)

        out_file_name = ctx.attr.out_file_name[file_name]
        for suffix in suffixs:
            output_file = ctx.actions.declare_file("{}/gen-cpp2/{}{}".format(ctx.attr.out_dir, out_file_name, suffix))
            outputs.append(output_file)
            output_files.append(output_file)
        include_args = []
        add_include_arg(include_args, workspace_root)
        for data_file in ctx.files.data:
            add_include_arg(include_args, external_repo_root(data_file.path))

        raw_includes = ctx.attr.includes
        skip_next = False
        for i in range(len(raw_includes)):
            if skip_next:
                skip_next = False
                continue
            if raw_includes[i] == "-I" and i + 1 < len(raw_includes):
                add_include_arg(include_args, normalize_include_path(raw_includes[i + 1], workspace_root))
                skip_next = True
            else:
                include_args.append(raw_includes[i])

        ctx.actions.run(
            inputs = [thrift_file] + ctx.files.data,
            outputs = outputs,
            arguments = [
                "--gen",
                gen_para,
                "-o",
                "{}/external/{}/{}".format(outputs[0].root.path, ctx.label.workspace_name, ctx.attr.out_dir),
            ] + include_args + [
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
        "thrift": attr.label(executable = True, cfg = "exec", default = "@fbthrift//:thrift1"),
        "gen_para": attr.string_list(mandatory = True),
        "includes": attr.string_list(mandatory = True),
        "plugin": attr.string(mandatory = False, default = "mstch_cpp2"),
        "out_dir": attr.string(mandatory = True),
        "out_file_name": attr.string_dict(mandatory = True),
        "service_out_file_name": attr.string_dict(mandatory = False),
    },
)
