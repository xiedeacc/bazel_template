/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/swig/handler.h"

#include "gtest/gtest.h"

namespace bazel_template {
namespace swig {

TEST(Handler, DoHandle) {
  Handler handler;
  LOG(INFO) << handler.DoHandle("d7gae7a6tawoc032825653f44e5a1227");
}

}  // namespace swig
}  // namespace bazel_template
