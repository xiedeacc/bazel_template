/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SWIG_CONFIG_MANAGER_H_
#define BAZEL_TEMPLATE_SWIG_CONFIG_MANAGER_H_

#include <map>
#include <string>

#include "src/util/config_manager.h"

namespace bazel_template {
namespace swig {

using std::map;
using std::string;

class ConfigManager {
 public:
  bool Init(const string& base_config_path);
  string ToString() { return util::ConfigManager::Instance()->ToString(); }
};

}  // namespace swig
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SWIG_CONFIG_MANAGER_H_
