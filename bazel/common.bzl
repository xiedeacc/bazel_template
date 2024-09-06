load("@bazel_tools//tools/cpp:toolchain_utils.bzl", "find_cpp_toolchain", "use_cpp_toolchain")

GLOBAL_COPTS = select({
    "@bazel_template//bazel:cross_compiling_for_osx_gcc": [
        "-mmacosx-version-min=10.15",
    ],
    "@bazel_template//bazel:cross_compiling_for_osx_clang": [
        "-stdlib=libc++",
        "-mmacosx-version-min=10.15",
    ],
    "@bazel_template//bazel:not_cross_compiling_on_osx": [
        "-stdlib=libc++",
        "-mmacosx-version-min=10.15",
    ],
    "@bazel_template//bazel:cross_compiling_for_windows_gcc": [
        "-stdlib=libc++",
        "-mmacosx-version-min=10.15",
    ],
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
        "/GS",
        "/W1",
        "/Wall",
        "/Zc:wchar_t",
        #"/Zi",
        "/O2",
        "/Ob2",
        "/Gm-",
        "/fp:precise",
        "/Zc:forScope",
        "/Gd",
        "/MD",
        "/diagnostics:column",
        "/nologo",
    ],
    "//conditions:default": [
        "-Wall",
        "-Wextra",
        "-O2",
        "-g",
    ],
})

GLOBAL_LOCAL_DEFINES = select({
    "@bazel_template//bazel:cross_compiling_for_osx_gcc": [],
    "@bazel_template//bazel:cross_compiling_for_osx_clang": [],
    "@bazel_template//bazel:not_cross_compiling_on_osx": [],
    "@bazel_template//bazel:cross_compiling_for_windows_gcc": [],
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
        "_MBCS",
        "WIN32",
        "_WINDOWS",
        "NDEBUG",
    ],
    "//conditions:default": [
        "-Wall",
        "-Wextra",
        "-O2",
        "-g",
    ],
})

GLOBAL_LINKOPTS = select({
    "@bazel_template//bazel:cross_compiling_for_osx": [
        "-stdlib=libc++",
        "-mmacosx-version-min=10.15",
    ],
    "@bazel_template//bazel:not_cross_compiling_on_osx": [
        "-stdlib=libc++",
        "-lc++abi",
        "-mmacosx-version-min=10.15",
    ],
    "@bazel_template//bazel:cross_compiling_for_windows": [
    ],
    "@bazel_template//bazel:not_cross_compiling_on_windows": [
    ],
    "//conditions:default": [],
})

def dict_union(x, y):
    z = {}
    z.update(x)
    z.update(y)
    return z

def _template_rule_impl(ctx):
    ctx.actions.expand_template(
        template = ctx.file.src,
        output = ctx.outputs.out,
        substitutions = ctx.attr.substitutions,
    )

template_rule = rule(
    attrs = {
        "src": attr.label(
            mandatory = True,
            allow_single_file = True,
        ),
        "substitutions": attr.string_dict(mandatory = True),
        "out": attr.output(mandatory = True),
    },
    output_to_genfiles = True,
    implementation = _template_rule_impl,
)

def _extract_symbols_impl(ctx):
    awk_script = ctx.file.awk_script
    obj_files = []
    for lib in ctx.attr.deps:
        cc_info = lib[CcInfo]
        for linker_input in cc_info.linking_context.linker_inputs.to_list():
            for library in linker_input.libraries:
                for obj_file in library.objects:
                    if "/external/jemalloc" in obj_file.path:
                        obj_files.append(obj_file)

    cc_toolchain = find_cpp_toolchain(ctx)
    output_files = []
    for obj_file in obj_files:
        output_file = ctx.actions.declare_file(obj_file.basename + ".sym")
        output_files.append(output_file)

        ctx.actions.run_shell(
            inputs = obj_files + [awk_script],
            outputs = [output_file],
            command = "{} {} | awk -f {} >> {}".format(cc_toolchain.nm_executable, obj_file.path, awk_script.path, output_file.path),
        )

    return [DefaultInfo(files = depset(output_files))]

extract_symbols = rule(
    implementation = _extract_symbols_impl,
    attrs = {
        "deps": attr.label_list(
            allow_files = False,
            mandatory = True,
            #providers = ["CcInfo"],
        ),
        "awk_script": attr.label(allow_single_file = True),
        "_cc_toolchain": attr.label(default = Label("@bazel_tools//tools/cpp:current_cc_toolchain")),
    },
    toolchains = use_cpp_toolchain(),
    fragments = ["cpp"],
)

def _local_config_git_genrule_impl(ctx):
    ctx.actions.run_shell(
        outputs = [ctx.outputs.out],
        inputs = [f for t in ctx.attr.srcs for f in t.files.to_list()],
        tools = [ctx.executable.exec_tool],
        arguments = [f.path for t in ctx.attr.srcs for f in t.files.to_list()] +
                    [ctx.outputs.out.path],
        command = "%s %s" % (ctx.executable.exec_tool.path, ctx.attr.arguments),
        execution_requirements = {"no-remote-exec": ""},
        use_default_shell_env = True,
    )

_local_config_git_genrule_internal = rule(
    implementation = _local_config_git_genrule_impl,
    attrs = {
        "out": attr.output(),
        "exec_tool": attr.label(
            executable = True,
            #cfg = _local_exec_transition,
            cfg = "exec",
            allow_files = True,
        ),
        "arguments": attr.string(),
        "srcs": attr.label_list(
            allow_files = True,
        ),
    },
)

def _local_config_git_genrule(**kwargs):
    _local_config_git_genrule_internal(
        **kwargs
    )

def version_info(name, out, exec_tool):
    _local_config_git_genrule(
        name = name,
        out = out,
        exec_tool = exec_tool,
        srcs = [
            "@local_config_git//:gen/spec.json",
            "@local_config_git//:gen/head",
            "@local_config_git//:gen/branch_ref",
        ],
        arguments = "--generate \"$@\"",
    )

def os(repository_ctx):
    name = repository_ctx.os.name
    if name == "linux":
        return "linux"
    elif name == "mac os x":
        return "darwin"
    elif name.startswith("windows"):
        return "windows"
    fail("Unsupported OS: " + name)

def _get_python_bin_path(repository_ctx):
    if repository_ctx.attr.python_bin_path != "":
        return repository_ctx.attr.python_bin_path
    python_bin_path = repository_ctx.which("python3")
    if python_bin_path != None:
        return str(python_bin_path)
    python_bin_path = repository_ctx.which("python")
    if python_bin_path != None:
        return str(python_bin_path)
    fail("find python error, install python and put it into PATH")

def _gen_local_config_git_impl(repository_ctx):
    local_config_git_build = """
# Description:
# Exports generated files used to generate tensorflow/core/util/version_info.cc

package(default_visibility = ["//visibility:public"])

licenses(["notice"])

exports_files(
    glob(["gen/*"]),
)
    """

    if repository_ctx.os.name.startswith("windows") == "windows":
        repository_ctx.file("BUILD.bazel", executable = False)
        return None
    else:
        repository_ctx.file(
            "BUILD.bazel",
            content = local_config_git_build,
            executable = False,
        )

    workspace_root_path = str(repository_ctx.path(
        Label("//:BUILD"),
    ))[:-len("BUILD")]

    python_script_path = repository_ctx.path(
        Label("//bazel:gen_local_config_git.py"),
    )
    generated_files_path = repository_ctx.path("gen")

    r = repository_ctx.execute(
        ["test", "-f", "%s/.git/logs/HEAD" % workspace_root_path],
    )
    if r.return_code == 0:
        unused_var = repository_ctx.path(Label("//:.git/HEAD"))  # pylint: disable=unused-variable

    result = repository_ctx.execute([
        _get_python_bin_path(repository_ctx),
        python_script_path,
        "--configure",
        workspace_root_path,
        "--gen_root_path",
        generated_files_path,
    ], quiet = False)

    if not result.return_code == 0:
        fail(result.stderr)

gen_local_config_git = repository_rule(
    implementation = _gen_local_config_git_impl,
    attrs = {
        "python_bin_path": attr.string(mandatory = False),
    },
)
