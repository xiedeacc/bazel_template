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
    "-isystem external/proxygen",
    "-isystem external/fb303",
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

sh_binary(
    name = "gen_HTTPCommonHeaders_sh",
    srcs = ["proxygen/lib/http/gen_HTTPCommonHeaders.sh"],
)

#genrule(
#name = "HTTPCommonHeaders",
#srcs = [
#"proxygen/lib/utils/perfect_hash_table_template.h",
#"proxygen/lib/utils/perfect_hash_table_template.cpp.gperf",
#"proxygen/lib/http/HTTPCommonHeaders.txt",
#"proxygen/lib/utils/gen_perfect_hash_table.sh",
#],
#outs = [
#"proxygen/lib/http/HTTPCommonHeaders.h",
#"proxygen/lib/http/HTTPCommonHeaders.cpp",
#],
#cmd = "$(location :gen_HTTPCommonHeaders_sh) $(location :proxygen/lib/http/HTTPCommonHeaders.txt) . proxygen/lib/http",
#tools = [":gen_HTTPCommonHeaders_sh"],
#)

cc_library(
    name = "proxygen",
    srcs = glob(
        [
            "proxygen/**/*.cpp",
        ],
        exclude = [
            "proxygen/lib/dns/*.cpp",  # c-ares version mismatch and some enum missed
            #"proxygen/lib/http/codec/compress/experimental/interop/QPACKInterop.cpp",
            "proxygen/lib/http/codec/compress/experimental/simulator/**/*.cpp",
            "proxygen/**/fuzzers/**/*.cpp",
            "proxygen/**/test/**/*.cpp",
            "proxygen/**/tests/**/*.cpp",
            "proxygen/**/samples/**/*.cpp",
        ],
    ),
    hdrs = glob(
        [
            "proxygen/**/*.h",
        ],
        exclude = [
        ],
    ),
    copts = COPTS,
    deps = [
        "@c-ares",
        "@fb303",
    ],
)

#cc_binary(
#name = "QPACKInterop",
#srcs = ["proxygen/lib/http/codec/compress/experimental/interop/QPACKInterop.cpp"],
#copts = COPTS,
#deps = [
#":proxygen",
#],
#)
