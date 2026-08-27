"""CPU instruction feature flags for AMD Ryzen 9.

Provides feature lists for GCC/Clang and MSVC to be selected via
config_settings in bazel/BUILD and wired through GLOBAL_COPTS.
"""
ryzen9_cpu_instruction_features_gcc_clang = [
    # Keep only meaningful, non-baseline features common on Ryzen 5
    "-mavx2",
    "-mavx",
    "-mbmi1",
    "-mbmi2",
    "-mfma",
    "-mf16c",
    "-maes",
    "-mpclmulqdq",
    "-mvaes",
    "-mvpclmulqdq",
    "-mpopcnt",
    "-msse4_2",
    "-msse4_1",
    "-mssse3",
    "-msse4a",
    "-mavx512f",
    "-mavx512vl",
    "-mavx512dq",
    "-mavx512bw",
    "-mavx512cd",
    "-mavx512er",
    "-mavx512ifma",
    "-mavx512pf",
    "-mavx512vbmi",
    "-mavx512vbmi2",
    "-mavx512vnni",
    "-mavx512bitalg",
    "-mavx512vpopcntdq",
]

# Optional MSVC equivalents (Ryzen 5 supports AVX/AVX2; enable those and favor AMD64)
ryzen9_cpu_instruction_features_msvc = [
    "/arch:AVX2",
    "/arch:AVX",
    "/arch:AVX512",
    "/favor:AMD64",
]
