/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/config_manager.h"

namespace bazel_template::util {

static folly::Singleton<ConfigManager> config_manager;

std::shared_ptr<ConfigManager> ConfigManager::Instance() {
  return folly::Singleton<bazel_template::util::ConfigManager,
                          folly::detail::DefaultTag,
                          folly::detail::DefaultTag>::try_get();
}

}  // namespace bazel_template::util
