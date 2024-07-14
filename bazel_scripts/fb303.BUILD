package(default_visibility = ["//visibility:public"])

config_setting(
    name = "windows_x86_64",
    constraint_values = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
    visibility = ["//visibility:public"],
)

config_setting(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
)

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
)

platform(
    name = "linux_aarch64_platform",
    constraint_values = [
        "@platforms//cpu:aarch64",
        "@platforms//os:linux",
    ],
)

platform(
    name = "linux_x86_64_platform",
    constraint_values = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
)

COPTS = [
    "-isystem .",
    "-isystem external/double-conversion",
    "-isystem external/xxhash",
    "-isystem external/com_googlesource_code_re2",
    "-isystem external/fatal",
    "-isystem $(BINDIR)/external/folly",
    "-isystem $(BINDIR)/external/fizz",
    "-isystem external/folly",
    "-isystem external/fizz",
    "-isystem external/wangle",
    "-isystem external/mvfst",
    "-std=c++17",
    "-fsized-deallocation",
]

LOCAL_DEFINES = [
    "NDEBUG",
]

cc_library(
    name = "fb303",
    srcs = glob(
        [
            "fb303/**/*.cpp",
        ],
        exclude = [
            "fb303/**/test/*.cpp",
        ],
    ),
    hdrs = [
    ] + glob(
        [
            "fb303/**/*.h",
        ],
        exclude = [
        ],
    ),
    copts = COPTS,
    deps = [
        "@fbthrift//:lib",
        "@yaml-cpp",
    ],
)
