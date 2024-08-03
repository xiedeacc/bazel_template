/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "folly//init//Init.h"
#include "gflags/gflags.h"
#include "gtest/gtest.h"

int main(int argc, char **argv) {
  folly::Init init(&argc, &argv, false);
  ::testing::InitGoogleTest(&argc, argv);
  google::ParseCommandLineFlags(&argc, &argv, false);
  return RUN_ALL_TESTS();
}
