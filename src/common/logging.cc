/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/common/logging.h"

#include <array>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <memory>
#include <mutex>
#include <source_location>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "spdlog/sinks/null_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace bazel_template::logging {
namespace {

constexpr std::string_view kLogPattern = "%Y%m%d %H:%M:%S.%e %L %t %s:%#] %v";

// The logger is process-wide by nature: Initialize() installs it and every
// LOG() reads it. g_logging_mutex serialises both.
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
std::mutex g_logging_mutex;
std::shared_ptr<spdlog::logger> g_logger;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

constexpr spdlog::level::level_enum ToSpdlogLevel(Severity severity) {
  using enum Severity;
  switch (severity) {
    case kInfo:
      return spdlog::level::info;
    case kWarning:
      return spdlog::level::warn;
    case kError:
      return spdlog::level::err;
    case kFatal:
      return spdlog::level::critical;
  }
  return spdlog::level::info;
}

std::shared_ptr<spdlog::logger> Logger() {
  const std::scoped_lock lock(g_logging_mutex);
  if (!g_logger) {
    auto sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    sink->set_level(spdlog::level::info);
    g_logger = std::make_shared<spdlog::logger>(std::string(kDefaultLoggerName),
                                                std::move(sink));
    g_logger->set_level(spdlog::level::info);
    g_logger->set_pattern(std::string(kLogPattern));
    spdlog::set_default_logger(g_logger);
  }
  return g_logger;
}

void Log(Severity severity, const std::source_location& location,
         std::string_view message) {
  auto logger = Logger();
  logger->log(spdlog::source_loc{location.file_name(),
                                 static_cast<int>(location.line()),
                                 location.function_name()},
              ToSpdlogLevel(severity), message);
  if (severity == Severity::kFatal) {
    logger->flush();
  }
}

}  // namespace

void Initialize(std::string_view program_name, std::string_view log_dir,
                bool write_logs) {
  const std::scoped_lock lock(g_logging_mutex);

  std::string basename =
      program_name.empty()
          ? std::string(kDefaultLoggerName)
          : std::filesystem::path(program_name).filename().string();
  if (basename.empty()) {
    basename = std::string(kDefaultLoggerName);
  }
  std::vector<spdlog::sink_ptr> sinks;

  if (!write_logs) {
    sinks.push_back(std::make_shared<spdlog::sinks::null_sink_mt>());
  }

  if (write_logs) {
    auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);
    sinks.push_back(console_sink);
    std::filesystem::create_directories(log_dir);
    struct FileSink {
      std::string_view suffix;
      spdlog::level::level_enum level;
    };
    constexpr std::array<FileSink, 4> kFileSinks = {{
        {.suffix = "INFO", .level = spdlog::level::info},
        {.suffix = "WARNING", .level = spdlog::level::warn},
        {.suffix = "ERROR", .level = spdlog::level::err},
        {.suffix = "FATAL", .level = spdlog::level::critical},
    }};

    for (const auto& sink_config : kFileSinks) {
      const auto path = std::filesystem::path(log_dir) /
                        std::format("{}.{}.log", basename, sink_config.suffix);
      auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
          path.string(), kMaxLogFileSize, kMaxRotatedFiles);
      sink->set_level(sink_config.level);
      sinks.push_back(sink);
    }
  }

  auto logger =
      std::make_shared<spdlog::logger>(basename, sinks.begin(), sinks.end());
  logger->set_level(spdlog::level::info);
  logger->set_pattern(std::string(kLogPattern));
  logger->flush_on(spdlog::level::info);
  spdlog::set_default_logger(logger);
  g_logger = logger;
}

void Shutdown() {
  const std::scoped_lock lock(g_logging_mutex);
  if (g_logger) {
    g_logger->flush();
  }
  spdlog::shutdown();
  g_logger.reset();
}

std::string CommandLine(std::span<char* const> args) {
  std::string result;
  for (const auto* arg : args) {
    if (arg == nullptr) {
      continue;
    }
    if (!result.empty()) {
      result.push_back(' ');
    }
    result.append(arg);
  }
  return result;
}

void ReportException(std::string_view context, const char* what) noexcept {
  // Not LOG(ERROR): this is the implementation unit of the module that
  // src/common/logging.h imports, and a module cannot import itself.
  try {
    if (what != nullptr) {
      LogMessage(Severity::kError).stream()
          << std::format("{}: {}", context, what);
    } else {
      LogMessage(Severity::kError).stream()
          << std::format("{}: unknown exception", context);
    }
  } catch (...) {  // NOLINT(bugprone-empty-catch)
  }
}

LogMessage::LogMessage(Severity severity, const std::source_location& location)
    : location_(location), severity_(severity) {}

// Destructors are noexcept. Formatting and the sink can throw; a lost log
// line is the lesser evil, so the failure is swallowed.
LogMessage::~LogMessage() {
  try {
    Log(severity_, location_, stream_.str());
  } catch (...) {  // NOLINT(bugprone-empty-catch)
  }
  if (severity_ == Severity::kFatal) {
    std::abort();
  }
}

FatalLogMessage::FatalLogMessage(const std::source_location& location)
    : location_(location) {}

FatalLogMessage::~FatalLogMessage() {
  try {
    Log(Severity::kFatal, location_, stream_.str());
  } catch (...) {  // NOLINT(bugprone-empty-catch)
  }
  std::abort();
}

CheckMessage::CheckMessage(bool condition, std::string_view expr,
                           const std::source_location& location)
    : location_(location), failed_(!condition) {
  if (failed_) {
    stream_ << expr;
  }
}

CheckMessage::~CheckMessage() {
  if (!failed_) {
    return;
  }
  try {
    Log(Severity::kFatal, location_, stream_.str());
  } catch (...) {  // NOLINT(bugprone-empty-catch)
  }
  std::abort();
}

}  // namespace bazel_template::logging
