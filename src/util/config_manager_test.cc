/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/config_manager.h"

#include <memory>
#include <string>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "rules_cc/cc/runfiles/runfiles.h"

namespace bazel_template {
namespace util {

using rules_cc::cc::runfiles::Runfiles;

TEST(ConfigManager, Init) {
  std::string error;
  std::unique_ptr<Runfiles> runfiles(
      Runfiles::CreateForTest(BAZEL_CURRENT_REPOSITORY, &error));
  ASSERT_NE(runfiles, nullptr) << error;

  const std::string path =
      runfiles->Rlocation("bazel_template/conf/server_config.json");
  ASSERT_FALSE(path.empty());

  EXPECT_TRUE(ConfigManager::Instance()->Init(path));
  LOG(INFO) << ConfigManager::Instance()->ToString();
}

}  // namespace util
}  // namespace bazel_template
