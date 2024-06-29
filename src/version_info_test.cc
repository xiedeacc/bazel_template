
#include "glog/logging.h"
#include "gtest/gtest.h"
#include <iostream>
#include <string>

TEST(VersionInfo, Info) {
  std::string git_ver = GIT_VERSION;
  LOG(INFO) << git_ver;
}
