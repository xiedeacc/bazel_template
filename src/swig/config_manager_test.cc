/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/swig/config_manager.h"

#include "gtest/gtest.h"

namespace bazel_template {
namespace swig {

TEST(ConfigManager, Init) {
  ConfigManager config_manager;
  EXPECT_TRUE(config_manager.Init("./conf/base_config.json"));
  LOG(INFO) << config_manager.ToString();
}

}  // namespace swig
}  // namespace bazel_template
