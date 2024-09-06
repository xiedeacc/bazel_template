/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace bazel_template {
namespace util {

TEST(Util, MemLeak) {
  char* arr = new char[8];
  snprintf(arr, sizeof(arr), "%s", "test");
  LOG(INFO) << arr;
  LOG(INFO) << "TEST";
}

}  // namespace util
}  // namespace bazel_template
