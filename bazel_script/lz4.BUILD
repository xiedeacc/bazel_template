# Description:
#   LZ4 library

licenses(["notice"])  # BSD license

exports_files(["LICENSE"])

cc_library(
    name = "liblz4",
    srcs = glob([
        "lib/lz4.c",
        "lib/lz4file.c",
        "lib/lz4frame.c",
        "lib/lz4hc.c",
    ]),
    hdrs = [
        "lib/lz4.h",
        "lib/lz4file.h",
        "lib/lz4frame.h",
        "lib/lz4frame_static.h",
        "lib/lz4hc.h",
        "lib/xxhash.h",
    ],
    copts = [
        "-O3",
    ],
    includes = [
        "lib",
    ],
    linkopts = [],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    textual_hdrs = [
        "lib/xxhash.c",
        "lib/lz4.c",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "fuzz_util",
    srcs = [
        "ossfuzz/fuzz_data_producer.c",
        "ossfuzz/lz4_helpers.c",
    ],
    hdrs = [
        "ossfuzz/fuzz.h",
        "ossfuzz/fuzz_data_producer.h",
        "ossfuzz/fuzz_helpers.h",
        "ossfuzz/lz4_helpers.h",
    ],
    copts = [
        "-O3",
    ],
    includes = ["ossfuzz"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":liblz4",
    ],
)

cc_binary(
    name = "compress_frame_fuzzer",
    srcs = [
        "ossfuzz/compress_frame_fuzzer.c",
        "ossfuzz/standaloneengine.c",
    ],
    copts = [
        "-O3",
    ],
    includes = ["ossfuzz"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":fuzz_util",
    ],
)

cc_binary(
    name = "compress_fuzzer",
    srcs = [
        "ossfuzz/compress_fuzzer.c",
        "ossfuzz/standaloneengine.c",
    ],
    copts = [
        "-O3",
    ],
    includes = ["ossfuzz"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":fuzz_util",
    ],
)

cc_binary(
    name = "compress_hc_fuzzer",
    srcs = [
        "ossfuzz/compress_hc_fuzzer.c",
        "ossfuzz/standaloneengine.c",
    ],
    copts = [
        "-O3",
    ],
    includes = ["ossfuzz"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":fuzz_util",
    ],
)

cc_binary(
    name = "decompress_frame_fuzzer",
    srcs = [
        "ossfuzz/decompress_frame_fuzzer.c",
        "ossfuzz/standaloneengine.c",
    ],
    copts = [
        "-O3",
    ],
    includes = ["ossfuzz"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":fuzz_util",
    ],
)

cc_binary(
    name = "decompress_fuzzer",
    srcs = [
        "ossfuzz/decompress_fuzzer.c",
        "ossfuzz/standaloneengine.c",
    ],
    copts = [
        "-O3",
    ],
    includes = ["ossfuzz"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":fuzz_util",
    ],
)

cc_binary(
    name = "round_trip_frame_fuzzer",
    srcs = [
        "ossfuzz/round_trip_frame_fuzzer.c",
        "ossfuzz/standaloneengine.c",
    ],
    copts = [
        "-O3",
    ],
    includes = ["ossfuzz"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":fuzz_util",
    ],
)

cc_binary(
    name = "round_trip_frame_uncompressed_fuzzer",
    srcs = [
        "ossfuzz/round_trip_frame_uncompressed_fuzzer.c",
        "ossfuzz/standaloneengine.c",
    ],
    copts = [
        "-O3",
    ],
    includes = ["ossfuzz"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":fuzz_util",
    ],
)

cc_binary(
    name = "round_trip_fuzzer",
    srcs = [
        "ossfuzz/round_trip_fuzzer.c",
        "ossfuzz/standaloneengine.c",
    ],
    copts = [
        "-O3",
    ],
    includes = ["ossfuzz"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":fuzz_util",
    ],
)

cc_binary(
    name = "round_trip_hc_fuzzer",
    srcs = [
        "ossfuzz/round_trip_hc_fuzzer.c",
        "ossfuzz/standaloneengine.c",
    ],
    copts = [
        "-O3",
    ],
    includes = ["ossfuzz"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":fuzz_util",
    ],
)

cc_binary(
    name = "round_trip_stream_fuzzer",
    srcs = [
        "ossfuzz/round_trip_stream_fuzzer.c",
        "ossfuzz/standaloneengine.c",
    ],
    copts = [
        "-O3",
    ],
    includes = ["ossfuzz"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":fuzz_util",
    ],
)

cc_library(
    name = "lz4_util",
    srcs = [
        "programs/bench.c",
        "programs/datagen.c",
        "programs/lz4io.c",
    ],
    hdrs = [
        "programs/bench.h",
        "programs/datagen.h",
        "programs/lz4io.h",
        "programs/platform.h",
        "programs/util.h",
    ],
    copts = [
        "-O3",
    ],
    includes = ["programs"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":liblz4",
    ],
)

cc_binary(
    name = "lz4",
    srcs = [
        "programs/lz4cli.c",
    ],
    copts = [
        "-O3",
    ],
    includes = ["programs"],
    local_defines = [
        "XXH_PRIVATE_API=1",
        "LZ4LIB_VISIBILITY=",
    ],
    deps = [
        ":lz4_util",
    ],
)
