#!/bin/bash
set -e

# Preprocess the assembly file
#/root/src/software/openwrt/openwrt-toolchain-23.05.3-rockchip-armv8_gcc-12.3.0_musl.Linux-x86_64/toolchain-aarch64_generic_gcc-12.3.0_musl/bin/aarch64-openwrt-linux-gcc -P "$1" -o "${1%.s}.i"
/bin/aarch64-linux-gnu-gcc-11 -Iexternal/libunwind/src/aarch64 -E "$1" -o "${1%.s}.i"

# Assemble the preprocessed file
#/root/src/software/openwrt/openwrt-toolchain-23.05.3-rockchip-armv8_gcc-12.3.0_musl.Linux-x86_64/toolchain-aarch64_generic_gcc-12.3.0_musl/bin/aarch64-openwrt-linux-as "${1%.s}.i" -o "${1%.s}.o"

# Link the object file
#/root/src/software/openwrt/openwrt-toolchain-23.05.3-rockchip-armv8_gcc-12.3.0_musl.Linux-x86_64/toolchain-aarch64_generic_gcc-12.3.0_musl/bin/aarch64-openwrt-linux-ld "${1%.s}.o" -o "${1%.s%.s}"
