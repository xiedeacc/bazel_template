/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// bazel_template.common.defs: shared constants.
module;

#include <cstdint>

export module bazel_template.common.defs;

export namespace bazel_template::common {

inline constexpr int64_t NET_BUFFER_SIZE_BYTES = int64_t{4} * 1024 * 1024;
inline constexpr int64_t CALC_BUFFER_SIZE_BYTES = int64_t{64} * 1024;  // 64KB
inline constexpr int64_t SESSION_INTERVAL = int64_t{5} * 60 * 1000;    // 5min

inline constexpr int kSaltSize = 16;        // 16 bytes (128 bits)
inline constexpr int kDerivedKeySize = 32;  // 32 bytes (256 bits)
inline constexpr int kIterations = 100000;  // PBKDF2 iterations

}  // namespace bazel_template::common
