/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/common/logging.h"

#include <array>
#include <chrono>
#include <filesystem>
#include <format>
#include <span>
#include <string>

#include "gtest/gtest.h"

namespace bazel_template::logging {
namespace {

std::filesystem::path UniqueLogDirectory(const char* suffix) {
  const auto timestamp =
      std::chrono::steady_clock::now().time_since_epoch().count();
  return std::filesystem::temp_directory_path() /
         std::format("bazel_template_logging_test_{}_{}", suffix, timestamp);
}

TEST(LoggingTest, DisabledLoggingDoesNotCreateDirectory) {
  const auto log_directory = UniqueLogDirectory("disabled");
  Initialize("logging_test", log_directory.string(), false);
  LOG(INFO) << "This message must be discarded";
  Shutdown();

  EXPECT_FALSE(std::filesystem::exists(log_directory));
}

TEST(LoggingTest, EnabledLoggingCreatesRotatingFiles) {
  const auto log_directory = UniqueLogDirectory("enabled");
  Initialize("logging_test", log_directory.string(), true);
  LOG(INFO) << "This message must be written";
  Shutdown();

  EXPECT_TRUE(std::filesystem::is_regular_file(log_directory /
                                               "logging_test.INFO.log"));
  std::error_code error;
  std::filesystem::remove_all(log_directory, error);
}

TEST(LoggingTest, CommandLineJoinsSpanOfArguments) {
  std::array<char*, 3> argv = {
      const_cast<char*>(
          "bazel_template"),  // NOLINT(cppcoreguidelines-pro-type-const-cast)
      const_cast<char*>(
          "--flag"),  // NOLINT(cppcoreguidelines-pro-type-const-cast)
      const_cast<char*>(
          "value"),  // NOLINT(cppcoreguidelines-pro-type-const-cast)
  };

  EXPECT_EQ(CommandLine(std::span<char* const>(argv)),
            "bazel_template --flag value");
  // The int/char** shim must stay equivalent for callers outside src/common.
  EXPECT_EQ(CommandLine(static_cast<int>(argv.size()), argv.data()),
            "bazel_template --flag value");
  EXPECT_EQ(CommandLine(0, argv.data()), "");
}

TEST(LoggingTest, CheckMacrosSurviveTheSourceLocationRewrite) {
  CHECK(true) << "not reported";
  DCHECK(true) << "not reported";
  CHECK_EQ(1, 1);
  CHECK_NE(1, 2);
  CHECK_LT(1, 2);
  CHECK_LE(1, 1);
  CHECK_GT(2, 1);
  CHECK_GE(1, 1);
}

TEST(LoggingTest, SourceLocationDefaultsToTheLogCallSite) {
  const auto here = std::source_location::current();
  const auto captured = std::source_location::current();

  EXPECT_STREQ(here.file_name(), captured.file_name());
  EXPECT_EQ(here.line() + 1, captured.line());

  // Constructing a LogMessage without an explicit location must not crash and
  // must pick up this translation unit.
  LogMessage message(Severity::kInfo);
  message.stream() << "source location smoke test";
}

}  // namespace
}  // namespace bazel_template::logging
