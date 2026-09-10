/*
 * Copyright 2016 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CPP_GRPC_COMMON_TIME_H_
#define CPP_GRPC_COMMON_TIME_H_

#include <chrono>
#include <cstdint>
#include <ostream>
#include <ratio>

namespace async_grpc::common {

inline constexpr std::int64_t kUtsEpochOffsetFromUnixEpochInSeconds =
    719162LL * 24LL * 60LL * 60LL;

struct UniversalTimeScaleClock {
  using rep = std::int64_t;
  using period = std::ratio<1, 10000000>;
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<UniversalTimeScaleClock>;
  static constexpr bool is_steady = true;
};

// Represents Universal Time Scale durations and timestamps which are 64-bit
// integers representing the 100 nanosecond ticks since the Epoch which is
// January 1, 1 at the start of day in UTC.
using Duration = UniversalTimeScaleClock::duration;
using Time = UniversalTimeScaleClock::time_point;

// Convenience functions to create common::Durations. Note that Duration is a
// std::chrono duration, so any std::chrono duration whose conversion is exact
// (std::chrono::seconds, milliseconds, ...) also converts implicitly.
[[nodiscard]] constexpr Duration FromSeconds(const double seconds) {
  return std::chrono::duration_cast<Duration>(
      std::chrono::duration<double>(seconds));
}

[[nodiscard]] constexpr Duration FromMilliseconds(
    const std::int64_t milliseconds) {
  return std::chrono::duration_cast<Duration>(
      std::chrono::milliseconds(milliseconds));
}

// Returns the given duration in seconds.
[[nodiscard]] constexpr double ToSeconds(const Duration duration) {
  return std::chrono::duration_cast<std::chrono::duration<double>>(duration)
      .count();
}

// Creates a time from a Universal Time Scale.
[[nodiscard]] constexpr Time FromUniversal(const std::int64_t ticks) {
  return Time(Duration(ticks));
}

// Outputs the Universal Time Scale timestamp for a given Time.
[[nodiscard]] constexpr std::int64_t ToUniversal(const Time time) {
  return time.time_since_epoch().count();
}

// For logging and unit tests, outputs the timestamp integer.
std::ostream& operator<<(std::ostream& os, Time time);

}  // namespace async_grpc::common

#endif  // CPP_GRPC_COMMON_TIME_H_
