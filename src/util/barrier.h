/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/
#pragma once

namespace bazel_template {
namespace util {

#if defined _MSC_VER
#include <intrin.h>
inline void CompileBarrier() {
  volatile int n = 0;
  n = 0;
}
// MemoryBarrier() is already defined in intrin.h
// You need to explicitly declare the function as
// intrinsic (even if you included <intrin.h>). E.g.
// MSVC 2005 has bugs when including <intrin.h>.
// https://developer.mozilla.org/en-US/docs/Developer_Guide/Build_Instructions/Intrin.h
extern "C" void _ReadBarrier(void);
extern "C" void _WriteBarrier(void);
#pragma intrinsic(_ReadWriteBarrier)
#pragma intrinsic(_WriteBarrier)
inline void MemoryReadBarrier() { _ReadBarrier(); }
inline void MemoryWriteBarrier() { _WriteBarrier(); }
#elif defined __GNUC__
#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) > 40100
inline void CompilerBarrier() { __sync_synchronize(); }
#else
inline void CompilerBarrier() { __asm__ __volatile__("" : : : "memory"); }
#endif
#if defined __i386__ || defined __x86_64__
inline void MemoryBarrier() { __asm__ __volatile__("mfence" : : : "memory"); }
inline void MemoryReadBarrier() { __asm__ __volatile__("lfence" ::: "memory"); }
inline void MemoryWriteBarrier() {
  __asm__ __volatile__("sfence" ::: "memory");
}
#else
inline void MemoryBarrier() { CompilerBarrier(); }
inline void MemoryReadBarrier() { MemoryBarrier(); }
inline void MemoryWriteBarrier() { MemoryBarrier(); }
#endif
#else
inline void CompilerBarrier() {
  volatile int n = 0;
  n = 0;
}
inline void MemoryBarrier() { CompilerBarrier(); }
inline void MemoryReadBarrier() { MemoryBarrier(); }
inline void MemoryWriteBarrier() { MemoryBarrier(); }
#endif

}  // namespace util
}  // namespace bazel_template
