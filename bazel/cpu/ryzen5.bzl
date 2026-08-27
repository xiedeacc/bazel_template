"""CPU instruction feature flags for AMD Ryzen 5.

Provides GCC/Clang and MSVC feature lists, selected via config_settings
and consumed by GLOBAL_COPTS in bazel/common.bzl.
"""
ryzen5_cpu_instruction_features_gcc_clang = [
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
]

# Optional MSVC equivalents (Ryzen 5 supports AVX/AVX2; enable those and favor AMD64)
ryzen5_cpu_instruction_features_msvc = [
    "/arch:AVX2",
    "/arch:AVX",
    "/favor:AMD64",
]
