/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <exception>

#include "folly/init/Init.h"
#include "gflags/gflags.h"
#include "gtest/gtest.h"
#include "src/common/logging.h"
// #include "lib/tcmalloc/malloc_extension.h"

int main(int argc, char** argv) {
  // An exception escaping main() calls std::terminate before anything is
  // logged, which makes a startup failure look like a silent crash.
  try {
    folly::Init init(&argc, &argv, false);
    // Tests log to the console only; no log files to collect afterwards.
    bazel_template::logging::Initialize(argv[0], "./log", false);
    ::testing::InitGoogleTest(&argc, argv);
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    // tcmalloc::MallocExtension::ActivateGuardedSampling();
    return RUN_ALL_TESTS();
  } catch (const std::exception& e) {
    bazel_template::logging::ReportException("Test main failed", e.what());
    return 1;
  } catch (...) {
    bazel_template::logging::ReportException("Test main failed", nullptr);
    return 1;
  }
}
