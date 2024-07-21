load("@rules_cc//cc:defs.bzl", "cc_toolchain", "cc_toolchain_suite")
load(":clang_linux_aarch64_cc_toolchain_config.bzl", clang_linux_aarch64_cc_toolchain_config = "cc_toolchain_config")
load(":clang_linux_x86_64_cc_toolchain_config.bzl", clang_linux_x86_64_cc_toolchain_config = "cc_toolchain_config")

package(default_visibility = ["//visibility:public"])

filegroup(name = "empty")

######### clang_linux_x86_64 #########
toolchain(
    name = "clang_toolchain_for_linux_x86_64",
    exec_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    target_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    toolchain = ":clang_linux_x86_64_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)

cc_toolchain(
    name = "clang_linux_x86_64_toolchain",
    all_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
    toolchain_config = ":clang_linux_x86_64_toolchain_config",
    toolchain_identifier = "clang_linux_x86_64_toolchain",
)

clang_linux_x86_64_toolchain_root = "/usr/local/llvm/18"

clang_linux_x86_64_cc_toolchain_config(
    name = "clang_linux_x86_64_toolchain_config",
    abi_libc_version = "local",
    abi_version = "local",
    compile_flags = [
        "-v",
        "-fstack-protector",
        "-Wall",
        "-Wunused-but-set-parameter",
        "-Wno-free-nonheap-object",
        "-fno-omit-frame-pointer",
        "-stdlib=libc++",
    ],
    compiler = "clang",
    conly_flags = [],
    coverage_compile_flags = ["--coverage"],
    coverage_link_flags = ["--coverage"],
    cpu = "k8",
    cxx_builtin_include_directories = [
        "{}/include/x86_64-unknown-linux-gnu/c++/v1".format(clang_linux_x86_64_toolchain_root),
        "{}/include/c++/v1".format(clang_linux_x86_64_toolchain_root),
        "{}/lib/clang/18/include".format(clang_linux_x86_64_toolchain_root),
        "/usr/local/include",
        "/usr/include/x86_64-linux-gnu",
        "/usr/include",
    ],
    cxx_flags = ["-std=c++17"],
    dbg_compile_flags = ["-g"],
    host_system_name = "local",
    link_flags = [
        "-fuse-ld=lld",
        "-B{}/bin".format(clang_linux_x86_64_toolchain_root),
        "-L{}/lib".format(clang_linux_x86_64_toolchain_root),
        "-L{}/lib/x86_64-unknown-linux-gnu".format(clang_linux_x86_64_toolchain_root),
        "-L{}/lib/clang/18/lib/x86_64-unknown-linux-gnu".format(clang_linux_x86_64_toolchain_root),
        "-Wl,-rpath={}/lib".format(clang_linux_x86_64_toolchain_root),
        "-Wl,-rpath={}/lib/x86_64-unknown-linux-gnu".format(clang_linux_x86_64_toolchain_root),
        "-Wl,-rpath={}/lib/clang/18/lib/x86_64-unknown-linux-gnu".format(clang_linux_x86_64_toolchain_root),
        "-Wl,-no-as-needed",
        "-Wl,-z,relro,-z,now",
        "-rtlib=compiler-rt",
    ],
    link_libs = [
        "-Wl,--push-state,-as-needed",
        "-lc++",
        "-lc++abi",
        "-Wl,--pop-state",
        "-Wl,--push-state,-as-needed",
        "-lm",
        "-Wl,--pop-state",
        "-Wl,--push-state,-as-needed",
        "{}/lib/clang/18/lib/x86_64-unknown-linux-gnu/libclang_rt.builtins.a".format(clang_linux_x86_64_toolchain_root),
        "-Wl,--pop-state",
    ],
    opt_compile_flags = [
        "-g0",
        "-O2",
        "-ffunction-sections",
        "-fdata-sections",
    ],
    opt_link_flags = ["-Wl,--gc-sections"],
    supports_start_end_lib = True,
    target_libc = "local",
    target_system_name = "local",
    tool_paths = {
        "ar": "{}/bin/llvm-ar".format(clang_linux_x86_64_toolchain_root),
        "ld": "{}/bin/lld".format(clang_linux_x86_64_toolchain_root),
        "llvm-cov": "{}/bin/llvm-cov".format(clang_linux_x86_64_toolchain_root),
        "llvm-profdata": "{}/bin/llvm-profdata".format(clang_linux_x86_64_toolchain_root),
        "cpp": "{}/bin/clang++".format(clang_linux_x86_64_toolchain_root),
        "clang++": "{}/bin/clang++".format(clang_linux_x86_64_toolchain_root),
        "gcc": "{}/bin/clang".format(clang_linux_x86_64_toolchain_root),
        "clang": "{}/bin/clang".format(clang_linux_x86_64_toolchain_root),
        "dwp": "{}/bin/llvm-dwp".format(clang_linux_x86_64_toolchain_root),
        "gcov": "{}/bin/llvm-cov".format(clang_linux_x86_64_toolchain_root),
        "nm": "{}/bin/llvm-nm".format(clang_linux_x86_64_toolchain_root),
        "objcopy": "{}/bin/llvm-objcopy".format(clang_linux_x86_64_toolchain_root),
        "objdump": "{}/bin/llvm-objdump".format(clang_linux_x86_64_toolchain_root),
        "strip": "{}/bin/llvm-strip".format(clang_linux_x86_64_toolchain_root),
    },
    toolchain_identifier = "local",
    unfiltered_compile_flags = [
        "-Wno-builtin-macro-redefined",
        "-D__DATE__=\"redacted\"",
        "-D__TIMESTAMP__=\"redacted\"",
        "-D__TIME__=\"redacted\"",
    ],
)

######### clang_linux_aarch64 #########
toolchain(
    name = "clang_toolchain_for_linux_aarch64",
    exec_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    target_compatible_with = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
    toolchain = ":clang_linux_aarch64_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)

cc_toolchain(
    name = "clang_linux_aarch64_toolchain",
    all_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 0,
    toolchain_config = ":clang_linux_aarch64_toolchain_config",
    toolchain_identifier = "clang_linux_aarch64_toolchain",
)

clang_linux_aarch64_toolchain_root = "/usr/local/llvm/18"

clang_linux_aarch64_cc_toolchain_config(
    name = "clang_linux_aarch64_toolchain_config",
    abi_libc_version = "glibc",
    abi_version = "clang",
    builtin_sysroot = "/root/src/software/clang_sysroot",
    compile_flags = [
        "--target=aarch64-unknown-linux-gnu",
        "-v",
        "-fstack-protector",
        "-Wall",
        "-Wunused-but-set-parameter",
        "-Wno-free-nonheap-object",
        "-fno-omit-frame-pointer",
        "-nobuiltininc",
        #"-nostdinc",
        #"-nostdinc++",
        "-nostdlibinc",  # equivalent to combine -nostdinc and -nostdinc++
        "-isystem",
        "/root/src/software/clang_sysroot/include/aarch64-unknown-linux-gnu/c++/v1",
        "-isystem",
        "/root/src/software/clang_sysroot/include/c++/v1",
        "-isystem",
        "/root/src/software/clang_sysroot/lib/clang/18/include",
        "-isystem",
        "/root/src/software/clang_sysroot/usr/include",
        "-stdlib=libc++",
    ],
    compiler = "clang",
    conly_flags = [
    ],
    coverage_compile_flags = ["--coverage"],
    coverage_link_flags = ["--coverage"],
    cpu = "aarch64",
    cxx_builtin_include_directories = [
        "/root/src/software/clang_sysroot/include/aarch64-unknown-linux-gnu/c++/v1",
        "/root/src/software/clang_sysroot/include/c++/v1",
        "/root/src/software/clang_sysroot/lib/clang/18/include",
        "/root/src/software/clang_sysroot/usr/include",
    ],
    cxx_flags = [
        "-std=c++17",
    ],
    dbg_compile_flags = ["-g"],
    host_system_name = "local",
    link_flags = [
        "--target=aarch64-unknown-linux-gnu",
        "-v",
        "-L/root/src/software/clang_sysroot/lib",
        "-L/root/src/software/clang_sysroot/lib/aarch64-unknown-linux-gnu",
        "-L/root/src/software/clang_sysroot/lib/clang/18/lib/aarch64-unknown-linux-gnu",
        "-L/root/src/software/clang_sysroot/usr/lib",
        "-fuse-ld=lld",
        "-B{}/bin".format(clang_linux_aarch64_toolchain_root),
        "-Wl,-no-as-needed",
        "-Wl,-z,relro,-z,now",
        "-rtlib=compiler-rt",
    ],
    link_libs = [
        "-Wl,--push-state,-as-needed",
        "-lc++",
        "-lc++abi",
        "-Wl,--pop-state",
        "-Wl,--push-state,-as-needed",
        "-lm",
        "-Wl,--pop-state",
        "-Wl,--push-state,-as-needed",
        "/root/src/software/clang_sysroot/lib/clang/18/lib/aarch64-unknown-linux-gnu/libclang_rt.builtins.a",
        "-Wl,--pop-state",
    ],
    opt_compile_flags = [
        "-g0",
        "-O2",
        "-ffunction-sections",
        "-fdata-sections",
    ],
    opt_link_flags = ["-Wl,--gc-sections"],
    supports_start_end_lib = True,
    target_libc = "local",
    target_system_name = "local",
    tool_paths = {
        "ar": "{}/bin/llvm-ar".format(clang_linux_aarch64_toolchain_root),
        "ld": "{}/bin/lld".format(clang_linux_aarch64_toolchain_root),
        "llvm-cov": "{}/bin/llvm-cov".format(clang_linux_aarch64_toolchain_root),
        "llvm-profdata": "{}/bin/llvm-profdata".format(clang_linux_aarch64_toolchain_root),
        "cpp": "{}/bin/clang++".format(clang_linux_aarch64_toolchain_root),
        "clang++": "{}/bin/clang++".format(clang_linux_aarch64_toolchain_root),
        "gcc": "{}/bin/clang".format(clang_linux_aarch64_toolchain_root),
        "clang": "{}/bin/clang".format(clang_linux_aarch64_toolchain_root),
        "dwp": "{}/bin/llvm-dwp".format(clang_linux_aarch64_toolchain_root),
        "gcov": "{}/bin/llvm-cov".format(clang_linux_aarch64_toolchain_root),
        "nm": "{}/bin/llvm-nm".format(clang_linux_aarch64_toolchain_root),
        "objcopy": "{}/bin/llvm-objcopy".format(clang_linux_aarch64_toolchain_root),
        "objdump": "{}/bin/llvm-objdump".format(clang_linux_aarch64_toolchain_root),
        "strip": "{}/bin/llvm-strip".format(clang_linux_aarch64_toolchain_root),
    },
    toolchain_identifier = "local",
    unfiltered_compile_flags = [
        "-Wno-builtin-macro-redefined",
        "-D__DATE__=\"redacted\"",
        "-D__TIMESTAMP__=\"redacted\"",
        "-D__TIME__=\"redacted\"",
    ],
)
