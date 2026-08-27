"""ARM Neoverse-11 CPU feature flags for GCC/Clang.

This targets ARMv8.2-A class cores with the following extensions enabled:
fp asimd evtstrm aes pmull sha1 sha2 crc32 atomics fphp asimdhp cpuid asimdrdm
lrcpc dcpop asimddp
"""

neoverse11_cpu_instruction_features_gcc_clang = [
    "-mcpu=neoverse-n1",
    # Explicitly encode baseline to be safe across toolchains
    "-march=armv8.2-a+crypto+crc+fp+simd+lse",
]

# MSVC (Windows/ARM64) does not use fine-grained /arch sub-switches for
# these extensions; keep this list empty unless specific tuning is required.
neoverse11_cpu_instruction_features_msvc = [
]
