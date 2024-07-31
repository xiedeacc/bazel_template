/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/server/version_info.h"

#include <iostream>
#include <string>

#include "glog/logging.h"
#include "gtest/gtest.h"

TEST(VersionInfo, Info) {
  std::string git_ver = GIT_VERSION;
  LOG(INFO) << git_ver;
}
