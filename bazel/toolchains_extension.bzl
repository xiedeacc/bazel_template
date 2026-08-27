load("//bazel:toolchains.bzl", "cc_toolchains_register")

def _cc_toolchains_extension(module_ctx):
    cc_toolchains_register()

cc_toolchains_extension = module_extension(
    implementation = _cc_toolchains_extension,
)
