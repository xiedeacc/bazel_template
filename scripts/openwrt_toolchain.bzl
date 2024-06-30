def _download_openwrt(rctx):
    urls = [rctx.attr.url]
    sha256sum = rctx.attr.sha256sum
    strip_prefix = None
    res = rctx.download_and_extract(
        urls,
        sha256sum = sha256sum,
        stripPrefix = strip_prefix,
    )
    print(res.sha256)
    return rctx.attr

def _openwrt_repo_impl(rctx):
    rctx.file(
        "BUILD.bazel",
        content = rctx.read(Label("//scripts:BUILD.openwrt_repo")),
        executable = False,
    )
    updated_attrs = _download_openwrt(rctx)
    return updated_attrs

openwrt = repository_rule(
    attrs = {
        "arch": attr.string(
            mandatory = True,
        ),
        "url": attr.string(
            mandatory = True,
        ),
        "sha256sum": attr.string(
            mandatory = True,
        ),
    },
    implementation = _openwrt_repo_impl,
)

def _is_absolute_path(path):
    if path.startswith("/") and not path.startswith("//"):
        return True
    if len(path) > 2 and path[1] == ":" and (path[2] == "\\" or path[2] == "/"):
        return True
    return False

def openwrt_toolchain_setup(name, **kwargs):
    if not kwargs.get("arch_urls"):
        fail("must set arch_urls")
    if not kwargs.get("arch_sha256sum"):
        fail("must set arch_sha256sum")

    arch_urls = kwargs.get("arch_urls")
    arch_sha256sum = kwargs.get("arch_sha256sum")

    for arch, url in arch_urls.items():
        if _is_absolute_path(url):
            continue
        if not arch_sha256sum.get(arch):
            fail("each arch must has url and sha256sum")

    for arch, url in arch_urls.items():
        if _is_absolute_path(url):
            continue
        repo_attrs = {}
        repo_attrs["arch"] = arch
        repo_attrs["url"] = url
        repo_attrs["sha256sum"] = arch_sha256sum.get(arch)
        openwrt(name = "openwrt_" + arch, **repo_attrs)
