/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/swig/config_manager.h"

#include <memory>
#include <string>

#include "gtest/gtest.h"
#include "rules_cc/cc/runfiles/runfiles.h"

namespace bazel_template {
namespace swig {

using rules_cc::cc::runfiles::Runfiles;

TEST(ConfigManager, Init) {
  std::string error;
  std::unique_ptr<Runfiles> runfiles(
      Runfiles::CreateForTest(BAZEL_CURRENT_REPOSITORY, &error));
  ASSERT_NE(runfiles, nullptr) << error;

  const std::string path =
      runfiles->Rlocation("bazel_template/conf/client_config.json");
  ASSERT_FALSE(path.empty());

  ConfigManager config_manager;
  EXPECT_TRUE(config_manager.Init(path));
  LOG(INFO) << config_manager.ToString();
}

}  // namespace swig
}  // namespace bazel_template
