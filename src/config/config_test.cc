/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "gtest/gtest.h"
#include "src/util/config_util.h"

namespace bazel_template {
namespace config {

using std::string;
using namespace bazel_template::util;

TEST(BaseConfig, Make) {
  BaseConfigPtr config(new BaseConfig());
  config->Make(
      Util::LoadContent(string("test_data/config_test/base_config.json")));
  ASSERT_EQ("http://127.0.0.1:80", config->config.nacos_sdk_addr());
  ASSERT_EQ("wx-pre", config->config.nacos_namespace());
  ASSERT_EQ("nads-bazel_template", config->config.nacos_service_name());
}

TEST(Config, Nacos) {
  ConfigUtil::Instance().Init("test_data/config_test/base_config.json");
  auto base_config = ConfigUtil::Instance().BaseConfig();
  LOG(INFO) << base_config->ToString();
}

}  // namespace config
}  // namespace bazel_template
