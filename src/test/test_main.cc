/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "folly/init/Init.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "gtest/gtest.h"
// #include "lib/tcmalloc/malloc_extension.h"

int main(int argc, char **argv) {
  folly::Init init(&argc, &argv, false);
  // google::InitGoogleLogging(argv[0]);
  google::SetStderrLogging(google::GLOG_INFO);
  ::testing::InitGoogleTest(&argc, argv);
  google::ParseCommandLineFlags(&argc, &argv, false);
  // tcmalloc::MallocExtension::ActivateGuardedSampling();
  return RUN_ALL_TESTS();
}
