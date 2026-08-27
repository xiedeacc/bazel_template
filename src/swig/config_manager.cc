/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/swig/config_manager.h"

namespace bazel_template::swig {

bool ConfigManager::Init(const std::string& base_config_path) {
  return util::ConfigManager::Instance()->Init(base_config_path);
}

}  // namespace bazel_template::swig
