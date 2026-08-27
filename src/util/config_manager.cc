/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/config_manager.h"

namespace bazel_template::util {

// folly::Singleton registers itself at static-init time; that registration is
// the whole point of the object, and it has no const form.
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
static folly::Singleton<ConfigManager> config_manager;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

std::shared_ptr<ConfigManager> ConfigManager::Instance() {
  return folly::Singleton<bazel_template::util::ConfigManager,
                          folly::detail::DefaultTag,
                          folly::detail::DefaultTag>::try_get();
}

}  // namespace bazel_template::util
