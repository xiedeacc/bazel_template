/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "folly/init/Init.h"
#include "gflags/gflags.h"
#include "gtest/gtest.h"
#include "src/common/logging.h"
// #include "lib/tcmalloc/malloc_extension.h"

int main(int argc, char** argv) {
  folly::Init init(&argc, &argv, false);
  // Tests log to the console only; no log files to collect afterwards.
  bazel_template::logging::Initialize(argv[0], "./log", false);
  ::testing::InitGoogleTest(&argc, argv);
  gflags::ParseCommandLineFlags(&argc, &argv, false);
  // tcmalloc::MallocExtension::ActivateGuardedSampling();
  return RUN_ALL_TESTS();
}
