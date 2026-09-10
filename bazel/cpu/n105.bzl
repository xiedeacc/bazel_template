"""CPU instruction feature flags for Intel N105.

Defines minimal GCC/Clang feature list (no AVX) and MSVC tuning flags
to be wired via config_settings into GLOBAL_COPTS.
"""
n105_cpu_instruction_features_gcc_clang = [
    # Keep only non-baseline features for x86_64 and without AVX on N105
    "-msse4.2",
    "-msse4.1",
    "-mssse3",
    "-mpopcnt",
    "-maes",
    "-mpclmulqdq",
]

n105_cpu_instruction_features_msvc = [
    # N105 has no AVX/AVX2/AVX-512. On MSVC x64, SSE2 is baseline and
    # SSE[3|4.x]/SSSE3 do not have separate /arch switches.
    # Keep codegen conservative and just tune for Intel 64-bit.
    "/favor:INTEL64",
]
