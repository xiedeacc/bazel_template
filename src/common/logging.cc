/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/common/logging.h"

#include <filesystem>
#include <mutex>
#include <vector>

#include "spdlog/sinks/null_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace bazel_template::logging {
namespace {

constexpr size_t kMaxLogFileSize = 10 * 1024 * 1024;
constexpr size_t kMaxRotatedFiles = 10;

std::mutex g_logging_mutex;
std::shared_ptr<spdlog::logger> g_logger;

spdlog::level::level_enum ToSpdlogLevel(Severity severity) {
  switch (severity) {
    case Severity::kInfo:
      return spdlog::level::info;
    case Severity::kWarning:
      return spdlog::level::warn;
    case Severity::kError:
      return spdlog::level::err;
    case Severity::kFatal:
      return spdlog::level::critical;
  }
  return spdlog::level::info;
}

std::shared_ptr<spdlog::logger> Logger() {
  std::lock_guard<std::mutex> lock(g_logging_mutex);
  if (!g_logger) {
    auto sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    sink->set_level(spdlog::level::info);
    g_logger =
        std::make_shared<spdlog::logger>("bazel_template", std::move(sink));
    g_logger->set_level(spdlog::level::info);
    g_logger->set_pattern("%Y%m%d %H:%M:%S.%e %L %t %s:%#] %v");
    spdlog::set_default_logger(g_logger);
  }
  return g_logger;
}

void Log(Severity severity, const char* file, int line,
         const std::string& message) {
  auto logger = Logger();
  logger->log(spdlog::source_loc{file, line, nullptr}, ToSpdlogLevel(severity),
              message);
  if (severity == Severity::kFatal) {
    logger->flush();
  }
}

}  // namespace

void Initialize(const std::string& program_name, const std::string& log_dir,
                bool write_logs) {
  std::lock_guard<std::mutex> lock(g_logging_mutex);

  std::string basename =
      program_name.empty()
          ? "bazel_template"
          : std::filesystem::path(program_name).filename().string();
  if (basename.empty()) {
    basename = "bazel_template";
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
    const struct {
      const char* suffix;
      spdlog::level::level_enum level;
    } file_sinks[] = {
        {.suffix = "INFO", .level = spdlog::level::info},
        {.suffix = "WARNING", .level = spdlog::level::warn},
        {.suffix = "ERROR", .level = spdlog::level::err},
        {.suffix = "FATAL", .level = spdlog::level::critical},
    };

    for (const auto& sink_config : file_sinks) {
      const auto path = std::filesystem::path(log_dir) /
                        (basename + "." + sink_config.suffix + ".log");
      auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
          path.string(), kMaxLogFileSize, kMaxRotatedFiles);
      sink->set_level(sink_config.level);
      sinks.push_back(sink);
    }
  }

  auto logger =
      std::make_shared<spdlog::logger>(basename, sinks.begin(), sinks.end());
  logger->set_level(spdlog::level::info);
  logger->set_pattern("%Y%m%d %H:%M:%S.%e %L %t %s:%#] %v");
  logger->flush_on(spdlog::level::info);
  spdlog::set_default_logger(logger);
  g_logger = logger;
}

void Shutdown() {
  std::lock_guard<std::mutex> lock(g_logging_mutex);
  if (g_logger) {
    g_logger->flush();
  }
  spdlog::shutdown();
  g_logger.reset();
}

std::string CommandLine(int argc, char** argv) {
  std::ostringstream oss;
  for (int i = 0; i < argc; ++i) {
    if (i != 0) {
      oss << ' ';
    }
    oss << argv[i];
  }
  return oss.str();
}

LogMessage::LogMessage(const char* file, int line, Severity severity)
    : file_(file), line_(line), severity_(severity) {}

LogMessage::~LogMessage() {
  Log(severity_, file_, line_, stream_.str());
  if (severity_ == Severity::kFatal) {
    std::abort();
  }
}

FatalLogMessage::FatalLogMessage(const char* file, int line)
    : file_(file), line_(line) {}

FatalLogMessage::~FatalLogMessage() {
  Log(Severity::kFatal, file_, line_, stream_.str());
  std::abort();
}

CheckMessage::CheckMessage(bool condition, const char* file, int line,
                           const char* expr)
    : failed_(!condition), file_(file), line_(line) {
  if (failed_) {
    stream_ << expr;
  }
}

CheckMessage::~CheckMessage() {
  if (!failed_) {
    return;
  }
  Log(Severity::kFatal, file_, line_, stream_.str());
  std::abort();
}

}  // namespace bazel_template::logging
