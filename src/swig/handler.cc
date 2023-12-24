/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/swig/handler.h"

namespace bazel_template {
namespace swig {

using namespace bazel_template::proto;

bool Handler::Init() {
  if (!service_impl_.Init()) {
    LOG(ERROR) << "ServiceImpl init failed, now exist...";
    return false;
  }
  return true;
}

std::string Handler::DoHandle(const std::string &req_json) {
  Request req;
  Response res;
  service_impl_.DoHandle(req, &res);
  return "";
}

}  // namespace swig
}  // namespace bazel_template
