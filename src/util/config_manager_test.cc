/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/config_manager.h"

#include "folly/init/Init.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

namespace bazel_template {
namespace util {

TEST(ConfigManager, Init) {
  EXPECT_TRUE(ConfigManager::Instance()->Init("./conf/base_config.json"));
  LOG(INFO) << ConfigManager::Instance()->ToString();
}

}  // namespace util
}  // namespace bazel_template
