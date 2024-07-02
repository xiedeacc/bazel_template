def _is_absolute_path(path):
    return path and path[0] == "/" and (len(path) == 1 or path[1] != "/")

def _download_openwrt(rctx):
    urls = [rctx.attr.url]
    res = rctx.download_and_extract(
        urls,
        sha256sum = rctx.attr.sha256sum,
        stripPrefix = rctx.attr.strip_prefix,
    )
    if rctx.attr.sha256sum != res.sha256:
        fail("need sha256sum:{}, but get:{}".format(rctx.attr.sha256sum, res.sha256))
    print(res.sha256)
    return rctx.attr

def openwrt_toolchain_repo_impl(rctx):
    print(rctx.attr)
    rctx.file(
        "BUILD.bazel",
        content = rctx.read(Label("//scripts:BUILD.openwrt_repo")),
        executable = False,
    )
    if _is_absolute_path(rctx.attr.url):
        return rctx.aatr

    updated_attrs = _download_openwrt(rctx)
    return updated_attrs
