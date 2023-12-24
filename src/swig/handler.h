/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/
#pragma once

#include "src/server/service_impl.h"

namespace bazel_template {
namespace swig {

class Handler {
 public:
  bool Init();
  std::string ToString() { return "Handler"; }
  std::string DoHandle(const std::string& req_json);

 private:
  server::ServiceImpl service_impl_;
};

}  // namespace swig
}  // namespace bazel_template
