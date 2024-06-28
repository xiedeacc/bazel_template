load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
)

all_link_actions = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

def _impl(ctx):
    tool_paths = [
        tool_path(
            name = "gcc",
            path = "/usr/local/llvm-18/bin/clang",
        ),
        tool_path(
            name = "g++",
            path = "/usr/local/llvm-18/bin/clang++",
        ),
        tool_path(
            name = "ld",
            path = "/usr/local/llvm-18/bin/lld",
        ),
        tool_path(
            name = "ar",
            path = "/usr/local/llvm-18/bin/llvm-ar",
        ),
        tool_path(
            name = "cpp",
            path = "/usr/local/llvm-18/bin/clang++",
        ),
        tool_path(
            name = "gcov",
            path = "/usr/local/llvm-18/bin/llvm-cov",
        ),
        tool_path(
            name = "nm",
            path = "/usr/local/llvm-18/bin/llvm-nm",
        ),
        tool_path(
            name = "objdump",
            path = "/usr/local/llvm-18/bin/llvm-objdump",
        ),
        tool_path(
            name = "strip",
            path = "/usr/local/llvm-18/bin/llvm-strip",
        ),
    ]

    features = [
        feature(
            name = "default_linker_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = all_link_actions,
                    flag_groups = ([
                        flag_group(
                            flags = [
                                "-lpthread",
                                "-lm",
                                "-lc++",
                                "-lc++abi",
                            ],
                        ),
                    ]),
                ),
            ],
        ),
        feature(
            name = "default_compile_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = [
                        ACTION_NAMES.c_compile,
                        ACTION_NAMES.cpp_compile,
                    ],
                    flag_groups = ([
                        flag_group(
                            flags = [
                                "-U_FORTIFY_SOURCE",
                            ],
                        ),
                    ]),
                ),
            ],
        ),
        feature(
            name = "default_cc_compile_flags",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = [
                        ACTION_NAMES.cpp_compile,
                    ],
                    flag_groups = ([
                        flag_group(
                            flags = [
                                "-stdlib=libc++",
                            ],
                        ),
                    ]),
                ),
            ],
        ),
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,  # NEW
        cxx_builtin_include_directories = [
            "/usr/local/llvm-18/include/x86_64-unknown-linux-gnu/c++/v1",
            "/usr/local/llvm-18/include/c++/v1",
            "/usr/local/llvm-18/lib/clang/18/include",
            "/usr/local/include",
            "/usr/include/x86_64-linux-gnu",
            "/usr/include",
        ],
        toolchain_identifier = "local",
        host_system_name = "local",
        target_system_name = "local",
        target_cpu = "k8",
        target_libc = "unknown",
        compiler = "clang",
        abi_version = "unknown",
        abi_libc_version = "unknown",
        tool_paths = tool_paths,
    )

cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {},
    provides = [CcToolchainConfigInfo],
)
