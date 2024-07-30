def _get_external_root(ctx):
    gendir = ctx.var["GENDIR"] + "/"
    external_roots = []
    for file in ctx.files.src:
        path = file.path
        if path.startswith(gendir):
            path = path[len(gendir):]
        path = path.split("/")
        if path[0] == "external":
            external_roots += ["/".join(path[0:2])]
    roots = depset(external_roots)
    n = len(external_roots)
    if n > 1:
        fail("""
           You are attempting simultaneous compilation of protobuf source files that span multiple workspaces (%s).
           Decompose your library rules into smaller units having filesets that belong to only a single workspace at a time.
           Note that it is OK to *import* across multiple workspaces, but not compile them as file inputs to protoc.
           """ % roots)
    elif n == 1:
        return external_roots[0]
    else:
        return "."

def _check_if_protos_are_generated(ctx):
    generated_path = ctx.var["GENDIR"]
    for plfile in ctx.attr.src.files.to_list():
        if not plfile.path.startswith(generated_path):
            return False
        if not plfile.is_source:
            return False
    return True

def _get_offset_path(root, path):
    """Adjust path relative to offset"""

    if path.startswith("/"):
        fail("path argument must not be absolute: %s" % path)

    if not root:
        return path

    if root == ".":
        return path

    # "external/foobar/file.proto" --> "file.proto"  if path.startswith(root):
    if path.startswith(root):
        start = len(root)
        if not root.endswith("/"):
            start += 1
            return path[start:]

    depth = root.count("/") + 1
    return "../" * depth + path

def _proto_generate_impl(ctx):
    execdir = _get_external_root(ctx)
    print(execdir)
    if _check_if_protos_are_generated(ctx):
        external = "" if execdir == "." else "/" + execdir
        execdir = ctx.var["GENDIR"] + external
    print(execdir)

    protoc = _get_offset_path(execdir, ctx.executable.protoc.path)
    plugin = _get_offset_path(execdir, ctx.executable.plugin.path)
    dir_out = _get_offset_path(execdir, ctx.executable.plugin.dirname)

    out_files = [ctx.actions.declare_file(out) for out in ctx.attr.outs]
    path = _get_offset_path(execdir, ctx.files.src[0].path)

    all_files = ctx.attr.src.files.to_list()
    protoc_cmd = [protoc]
    protoc_cmd += ["--proto_path=" + ctx.attr.proto_path]
    protoc_cmd += ["--proto_path=" + "."]
    all_inputs = []

    for dep in ctx.attr.proto_deps:
        for pfile in dep.files.to_list():
            all_inputs = all_inputs + [pfile]
            ppath = _get_offset_path(execdir, pfile.path)
            rpath = "/".join(ppath.split("/")[4:])
            if rpath == "":
                continue
            protoc_cmd += ["-I" + rpath + "=" + ppath]

    protoc_cmd += ["--plugin=protoc-gen-PLUGIN=" + plugin]
    protoc_cmd += ["--PLUGIN_out=" + dir_out]
    protoc_cmd += [path]

    cmds = []
    if execdir != ".":
        cmds += ["cd %s" % execdir]
        cmds += [" ".join(protoc_cmd)]

    print(cmds)
    print(all_inputs)
    ctx.actions.run_shell(
        inputs = all_inputs + [ctx.executable.plugin] + [ctx.executable.protoc],
        outputs = out_files,
        command = " && ".join(cmds),
    )
    return struct(files = depset(out_files))

_proto_generate = rule(
    attrs = {
        "src": attr.label(
            mandatory = True,
            allow_single_file = True,
        ),
        "outs": attr.string_list(),
        "data": attr.label_list(
            allow_files = True,
        ),
        "plugin": attr.label(
            mandatory = True,
            executable = True,
            providers = ["files_to_run"],
            cfg = "target",
            allow_files = True,
        ),
        "proto_deps": attr.label_list(
            allow_files = True,
        ),
        "proto_path": attr.string(
            mandatory = True,
        ),
        "protoc": attr.label(
            default = Label("@com_google_protobuf//:protoc"),
            executable = True,
            cfg = "target",
        ),
    },
    output_to_genfiles = True,
    implementation = _proto_generate_impl,
)

def proto_generate(name, src, outs, data, plugin, proto_deps, proto_path, protoc):
    args = {}
    args.update({
        "name": name,
        "src": src,
        "outs": outs,
        "data": data,
        "plugin": plugin,
        "proto_deps": proto_deps,
        "proto_path": proto_path,
        "protoc": protoc,
    })
    return _proto_generate(**args)

def cc_proto_plugin(
        name,
        src,
        outs,
        data,
        plugin,
        proto_deps,
        proto_path,
        protoc,
        deps = [],
        **kwargs):
    proto_name = name + "_proto"
    proto_generate(proto_name, src, outs, data, plugin, proto_deps, proto_path, protoc)
    native.cc_library(
        name = name,
        srcs = [proto_name],
        deps = depset(deps + ["@com_google_protobuf//:protobuf"]).to_list(),
        **kwargs
    )
