"""Repository rule for shallow Git checkouts with shallow submodules."""

def _run(ctx, arguments, timeout = 600):
    result = ctx.execute(
        arguments,
        environment = {
            "GIT_LFS_SKIP_SMUDGE": "1",
            "GIT_TERMINAL_PROMPT": "0",
        },
        timeout = timeout,
        working_directory = str(ctx.path(".")),
    )
    if result.return_code != 0:
        fail(
            "command failed (exit {}):\n{}\nstdout:\n{}\nstderr:\n{}".format(
                result.return_code,
                " ".join(arguments),
                result.stdout,
                result.stderr,
            ),
        )

def _shallow_git_repository_impl(ctx):
    if bool(ctx.attr.commit) == bool(ctx.attr.tag):
        fail("exactly one of commit or tag must be set")

    _run(ctx, ["git", "init", "--quiet"])
    _run(ctx, ["git", "remote", "add", "origin", ctx.attr.remote])

    ref = ctx.attr.commit
    if ctx.attr.tag:
        ref = "refs/tags/{}".format(ctx.attr.tag)
    _run(ctx, ["git", "fetch", "--depth=1", "origin", ref], timeout = 1800)
    _run(ctx, ["git", "checkout", "--detach", "--quiet", "FETCH_HEAD"])

    if ctx.attr.recursive_init_submodules:
        _run(ctx, ["git", "submodule", "sync", "--recursive"])
        _run(
            ctx,
            [
                "git",
                "-c",
                "protocol.file.allow=always",
                "submodule",
                "update",
                "--init",
                "--recursive",
                "--depth=1",
                "--checkout",
                "--force",
            ],
            timeout = 3600,
        )

    for patch in ctx.attr.patches:
        if str(patch).endswith("aws-sdk-cpp-remove-version-file.patch") and not ctx.path("version").exists:
            continue
        ctx.patch(patch, strip = ctx.attr.patch_strip)

    if ctx.attr.build_file:
        ctx.delete("BUILD.bazel")
        ctx.symlink(ctx.path(ctx.attr.build_file), "BUILD.bazel")

shallow_git_repository = repository_rule(
    implementation = _shallow_git_repository_impl,
    attrs = {
        "build_file": attr.label(allow_single_file = True),
        "commit": attr.string(),
        "patch_strip": attr.int(default = 0),
        "patches": attr.label_list(allow_files = True),
        "recursive_init_submodules": attr.bool(default = False),
        "remote": attr.string(mandatory = True),
        "tag": attr.string(),
    },
)
