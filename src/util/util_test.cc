/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace bazel_template::util {

TEST(Util, MemLeak) {
  char* arr = new char[8];
  snprintf(arr, 8, "%s", "test");
  LOG(INFO) << arr;
  LOG(INFO) << "TEST";
}

}  // namespace bazel_template::util
