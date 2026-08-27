/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_COMMON_LOGGING_H_
#define BAZEL_TEMPLATE_COMMON_LOGGING_H_

// folly, proxygen and grpc all pull in glog, and glog defines LOG and the
// CHECK family too. Including it here first consumes its include guard, so
// the macros defined at the bottom of this file are the ones that survive no
// matter where a translation unit includes this header. Nothing in this
// project logs through glog itself.
#include <cstdlib>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>

#include "glog/logging.h"

namespace bazel_template::logging {

enum class Severity {
  kInfo,
  kWarning,
  kError,
  kFatal,
};

void Initialize(const std::string& program_name,
                const std::string& log_dir = "./logs", bool write_logs = true);
void Shutdown();
std::string CommandLine(int argc, char** argv);

class LogMessage {
 public:
  LogMessage(const char* file, int line, Severity severity);
  LogMessage(const LogMessage&) = delete;
  LogMessage& operator=(const LogMessage&) = delete;
  LogMessage(LogMessage&&) = delete;
  LogMessage& operator=(LogMessage&&) = delete;
  ~LogMessage();

  std::ostream& stream() { return stream_; }

 private:
  const char* file_;
  int line_;
  Severity severity_;
  std::ostringstream stream_;
};

class FatalLogMessage {
 public:
  FatalLogMessage(const char* file, int line);
  FatalLogMessage(const FatalLogMessage&) = delete;
  FatalLogMessage& operator=(const FatalLogMessage&) = delete;
  FatalLogMessage(FatalLogMessage&&) = delete;
  FatalLogMessage& operator=(FatalLogMessage&&) = delete;
  [[noreturn]] ~FatalLogMessage();

  std::ostream& stream() { return stream_; }

 private:
  const char* file_;
  int line_;
  std::ostringstream stream_;
};

class CheckMessage {
 public:
  CheckMessage(bool condition, const char* file, int line, const char* expr);
  CheckMessage(const CheckMessage&) = delete;
  CheckMessage& operator=(const CheckMessage&) = delete;
  CheckMessage(CheckMessage&&) = delete;
  CheckMessage& operator=(CheckMessage&&) = delete;
  ~CheckMessage();

  std::ostream& stream() { return stream_; }

 private:
  bool failed_;
  const char* file_;
  int line_;
  std::ostringstream stream_;
};

}  // namespace bazel_template::logging

// A logging macro has to be a macro: it captures __FILE__ and __LINE__ at
// the call site, and CHECK has to leave its message unevaluated when the
// condition holds. A function can do neither.
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

#define LOG(level) BAZEL_TEMPLATE_LOG_##level(__FILE__, __LINE__)

#define BAZEL_TEMPLATE_LOG_INFO(file, line)                   \
  ::bazel_template::logging::LogMessage(                      \
      file, line, ::bazel_template::logging::Severity::kInfo) \
      .stream()
#define BAZEL_TEMPLATE_LOG_WARNING(file, line)                   \
  ::bazel_template::logging::LogMessage(                         \
      file, line, ::bazel_template::logging::Severity::kWarning) \
      .stream()
#define BAZEL_TEMPLATE_LOG_ERROR(file, line)                   \
  ::bazel_template::logging::LogMessage(                       \
      file, line, ::bazel_template::logging::Severity::kError) \
      .stream()
#define BAZEL_TEMPLATE_LOG_FATAL(file, line) \
  ::bazel_template::logging::FatalLogMessage(file, line).stream()

// glog also has DFATAL, which folly uses in its own headers: fatal in a
// debug build, a plain error otherwise. Overriding LOG globally means every
// severity our dependencies rely on has to exist here too.
#ifdef NDEBUG
#define BAZEL_TEMPLATE_LOG_DFATAL(file, line) \
  BAZEL_TEMPLATE_LOG_ERROR(file, line)
#else
#define BAZEL_TEMPLATE_LOG_DFATAL(file, line) \
  BAZEL_TEMPLATE_LOG_FATAL(file, line)
#endif

#define CHECK(condition)                                                   \
  ::bazel_template::logging::CheckMessage(static_cast<bool>(condition),    \
                                          __FILE__, __LINE__,              \
                                          "Check failed: " #condition " ") \
      .stream()

#define DCHECK(condition) CHECK(condition)

#define CHECK_OP(op_name, op, value1, value2)                \
  ::bazel_template::logging::CheckMessage(                   \
      ((value1)op(value2)), __FILE__, __LINE__,              \
      "Check failed: " #value1 " " #op_name " " #value2 " ") \
      .stream()

#define CHECK_EQ(value1, value2) CHECK_OP(==, ==, value1, value2)
#define CHECK_NE(value1, value2) CHECK_OP(!=, !=, value1, value2)
#define CHECK_LT(value1, value2) CHECK_OP(<, <, value1, value2)
#define CHECK_LE(value1, value2) CHECK_OP(<=, <=, value1, value2)
#define CHECK_GT(value1, value2) CHECK_OP(>, >, value1, value2)
#define CHECK_GE(value1, value2) CHECK_OP(>=, >=, value1, value2)
// NOLINTEND(cppcoreguidelines-macro-usage)

#endif  // BAZEL_TEMPLATE_COMMON_LOGGING_H_
