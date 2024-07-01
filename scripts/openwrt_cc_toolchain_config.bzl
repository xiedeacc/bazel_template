load("@bazel_tools//tools/cpp:unix_cc_toolchain_config.bzl", unix_cc_toolchain_config = "cc_toolchain_config")

def cc_toolchain_config(
        name,
        toolchain_identifier,
        target_system_name,
        tool_paths,
        compiler_configuration,
        sysroot_path,
        cxx_builtin_include_directories):
    unfiltered_compile_flags = [
        "-no-canonical-prefixes",
        "-Wno-builtin-macro-redefined",
        "-D__DATE__=\"redacted\"",
        "-D__TIMESTAMP__=\"redacted\"",
        "-D__TIME__=\"redacted\"",
    ]

    compile_flags = [
        "--target=" + target_system_name,
        "-U_FORTIFY_SOURCE",  # https://github.com/google/sanitizers/issues/247
        "-fstack-protector",
        "-fno-omit-frame-pointer",
        "-fcolor-diagnostics",
        "-Wall",
        "-Wthread-safety",
        "-Wself-assign",
    ]

    dbg_compile_flags = ["-g", "-fstandalone-debug"]

    opt_compile_flags = [
        "-g0",
        "-O2",
        "-D_FORTIFY_SOURCE=1",
        "-DNDEBUG",
        "-ffunction-sections",
        "-fdata-sections",
    ]

    link_flags = [
        "--target=" + target_system_name,
        "-lm",
        "-no-canonical-prefixes",
    ]

    link_libs = []
    archive_flags = []

    link_flags.extend([
        "-Wl,--build-id=md5",
        "-Wl,--hash-style=gnu",
        "-Wl,-z,relro,-z,now",
    ])

    cxx_flags = [
        "-std=c++17",
    ]

    link_flags.extend([
        "-l:libstdc++.a",
    ])

    opt_link_flags = ["-Wl,--gc-sections"]

    coverage_compile_flags = ["-fprofile-instr-generate", "-fcoverage-mapping"]
    coverage_link_flags = ["-fprofile-instr-generate"]

    if compiler_configuration["compile_flags"] != None:
        compile_flags = compiler_configuration["compile_flags"]
    if compiler_configuration["cxx_flags"] != None:
        cxx_flags = compiler_configuration["cxx_flags"]
    if compiler_configuration["link_flags"] != None:
        link_flags = compiler_configuration["link_flags"]
    if compiler_configuration["archive_flags"] != None:
        archive_flags = compiler_configuration["archive_flags"]
    if compiler_configuration["link_libs"] != None:
        link_libs = compiler_configuration["link_libs"]
    if compiler_configuration["opt_compile_flags"] != None:
        opt_compile_flags = compiler_configuration["opt_compile_flags"]
    if compiler_configuration["opt_link_flags"] != None:
        opt_link_flags = compiler_configuration["opt_link_flags"]
    if compiler_configuration["dbg_compile_flags"] != None:
        dbg_compile_flags = compiler_configuration["dbg_compile_flags"]
    if compiler_configuration["coverage_compile_flags"] != None:
        coverage_compile_flags = compiler_configuration["coverage_compile_flags"]
    if compiler_configuration["coverage_link_flags"] != None:
        coverage_link_flags = compiler_configuration["coverage_link_flags"]
    if compiler_configuration["unfiltered_compile_flags"] != None:
        unfiltered_compile_flags = compiler_configuration["unfiltered_compile_flags"]

    unix_cc_toolchain_config(
        name = name,
        cpu = "aarch64",
        compiler = "gcc",
        toolchain_identifier = toolchain_identifier,
        host_system_name = "local",
        target_system_name = target_system_name,
        target_libc = "musl",
        abi_version = "gcc",
        abi_libc_version = "musl",
        tool_paths = tool_paths,
        compile_flags = compile_flags,
        dbg_compile_flags = dbg_compile_flags,
        opt_compile_flags = opt_compile_flags,
        cxx_flags = cxx_flags,
        link_flags = link_flags,
        archive_flags = archive_flags,
        link_libs = link_libs,
        opt_link_flags = opt_link_flags,
        unfiltered_compile_flags = unfiltered_compile_flags,
        coverage_compile_flags = coverage_compile_flags,
        coverage_link_flags = coverage_link_flags,
        supports_start_end_lib = False,
        builtin_sysroot = sysroot_path,
        cxx_builtin_include_directories = cxx_builtin_include_directories,
    )
