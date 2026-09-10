/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_COMMON_LOGGING_H_
#define BAZEL_TEMPLATE_COMMON_LOGGING_H_

// Logging is deliberately a header, not a C++20 module. Its public API is the
// LOG()/CHECK*() macro family, and macros cannot be exported from a module.
// The types the macros expand to have to be visible wherever the macros are
// used, which includes inline code in headers, and a header must not import a
// module (an import that reaches a module unit through an #include is
// ill-formed, and gcc enforces it). So everything lives here, attached to the
// global module, and a module unit simply includes this header in its global
// module fragment.
//
// folly, proxygen and grpc all pull in glog, which defines LOG and the CHECK
// family too. Including it here first consumes its include guard, so the
// macros at the bottom of this file are the ones that survive regardless of
// where a translation unit includes this header. Nothing here logs through
// glog itself.
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <source_location>
#include <span>
#include <sstream>
#include <string>
#include <string_view>

#include "glog/logging.h"

namespace bazel_template::logging {

enum class Severity : std::uint8_t {
  kInfo,
  kWarning,
  kError,
  kFatal,
};

inline constexpr std::string_view kDefaultLogDir = "./logs";
inline constexpr std::string_view kDefaultLoggerName = "bazel_template";
inline constexpr std::size_t kMaxLogFileSize = std::size_t{10} * 1024 * 1024;
inline constexpr std::size_t kMaxRotatedFiles = 10;

void Initialize(std::string_view program_name,
                std::string_view log_dir = kDefaultLogDir,
                bool write_logs = true);
void Shutdown();

[[nodiscard]] std::string CommandLine(std::span<char* const> args);

// TODO(cpp20): remove after callers migrate to the std::span overload.
[[nodiscard]] inline std::string CommandLine(int argc, char** argv) {
  return CommandLine(std::span<char* const>(
      argv, argc > 0 ? static_cast<std::size_t>(argc) : std::size_t{0}));
}

// Reports a failure from a context that must not throw -- a catch handler in
// main, a destructor, or a noexcept override. Logging allocates and can
// itself throw, which is what this swallows. `what` stays a raw pointer
// because callers pass exception_ptr-less nullptr for "unknown exception".
void ReportException(std::string_view context, const char* what) noexcept;

class LogMessage {
 public:
  explicit LogMessage(Severity severity, const std::source_location& location =
                                             std::source_location::current());
  LogMessage(const LogMessage&) = delete;
  LogMessage& operator=(const LogMessage&) = delete;
  LogMessage(LogMessage&&) = delete;
  LogMessage& operator=(LogMessage&&) = delete;
  ~LogMessage();

  std::ostream& stream() { return stream_; }

 private:
  std::source_location location_;
  Severity severity_;
  std::ostringstream stream_;
};

class FatalLogMessage {
 public:
  explicit FatalLogMessage(
      const std::source_location& location = std::source_location::current());
  FatalLogMessage(const FatalLogMessage&) = delete;
  FatalLogMessage& operator=(const FatalLogMessage&) = delete;
  FatalLogMessage(FatalLogMessage&&) = delete;
  FatalLogMessage& operator=(FatalLogMessage&&) = delete;
  [[noreturn]] ~FatalLogMessage();

  std::ostream& stream() { return stream_; }

 private:
  std::source_location location_;
  std::ostringstream stream_;
};

class CheckMessage {
 public:
  CheckMessage(
      bool condition, std::string_view expr,
      const std::source_location& location = std::source_location::current());
  CheckMessage(const CheckMessage&) = delete;
  CheckMessage& operator=(const CheckMessage&) = delete;
  CheckMessage(CheckMessage&&) = delete;
  CheckMessage& operator=(CheckMessage&&) = delete;
  ~CheckMessage();

  std::ostream& stream() { return stream_; }

 private:
  std::source_location location_;
  bool failed_;
  std::ostringstream stream_;
};

}  // namespace bazel_template::logging

// A logging macro has to be a macro: std::source_location::current() as a
// default argument records the point of the macro expansion, and CHECK has to
// leave its message unevaluated when the condition holds. A function can do
// neither.
// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#ifdef LOG
#undef LOG
#endif
#ifdef CHECK
#undef CHECK
#endif
#ifdef DCHECK
#undef DCHECK
#endif
#ifdef CHECK_OP
#undef CHECK_OP
#endif
#ifdef CHECK_EQ
#undef CHECK_EQ
#endif
#ifdef CHECK_NE
#undef CHECK_NE
#endif
#ifdef CHECK_LT
#undef CHECK_LT
#endif
#ifdef CHECK_LE
#undef CHECK_LE
#endif
#ifdef CHECK_GT
#undef CHECK_GT
#endif
#ifdef CHECK_GE
#undef CHECK_GE
#endif

#define LOG(level) BAZEL_TEMPLATE_LOG_##level()

#define BAZEL_TEMPLATE_LOG_INFO()                 \
  ::bazel_template::logging::LogMessage(          \
      ::bazel_template::logging::Severity::kInfo) \
      .stream()
#define BAZEL_TEMPLATE_LOG_WARNING()                 \
  ::bazel_template::logging::LogMessage(             \
      ::bazel_template::logging::Severity::kWarning) \
      .stream()
#define BAZEL_TEMPLATE_LOG_ERROR()                 \
  ::bazel_template::logging::LogMessage(           \
      ::bazel_template::logging::Severity::kError) \
      .stream()
#define BAZEL_TEMPLATE_LOG_FATAL() \
  ::bazel_template::logging::FatalLogMessage().stream()

// glog also has DFATAL, which folly uses in its own headers: fatal in a debug
// build, a plain error otherwise. Overriding LOG globally means every
// severity our dependencies rely on has to exist here too.
#ifdef NDEBUG
#define BAZEL_TEMPLATE_LOG_DFATAL() BAZEL_TEMPLATE_LOG_ERROR()
#else
#define BAZEL_TEMPLATE_LOG_DFATAL() BAZEL_TEMPLATE_LOG_FATAL()
#endif

#define CHECK(condition)                                                   \
  ::bazel_template::logging::CheckMessage(static_cast<bool>(condition),    \
                                          "Check failed: " #condition " ") \
      .stream()

#define DCHECK(condition) CHECK(condition)

#define CHECK_OP(op_name, op, value1, value2)                           \
  ::bazel_template::logging::CheckMessage(((value1)op(value2)),         \
                                          "Check failed: " #value1      \
                                          " " #op_name " " #value2 " ") \
      .stream()

#define CHECK_EQ(value1, value2) CHECK_OP(==, ==, value1, value2)
#define CHECK_NE(value1, value2) CHECK_OP(!=, !=, value1, value2)
#define CHECK_LT(value1, value2) CHECK_OP(<, <, value1, value2)
#define CHECK_LE(value1, value2) CHECK_OP(<=, <=, value1, value2)
#define CHECK_GT(value1, value2) CHECK_OP(>, >, value1, value2)
#define CHECK_GE(value1, value2) CHECK_OP(>=, >=, value1, value2)
// NOLINTEND(cppcoreguidelines-macro-usage)

#endif  // BAZEL_TEMPLATE_COMMON_LOGGING_H_
