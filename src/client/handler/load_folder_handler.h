/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_CLIENT_HANDLER_LOAD_FOLDER_HANDLER_H_
#define BAZEL_TEMPLATE_CLIENT_HANDLER_LOAD_FOLDER_HANDLER_H_

#include <memory>
#include <string>

#include "folly/Singleton.h"

namespace bazel_template {
namespace client {
namespace handler {

class FolderLoader {
 private:
  friend class folly::Singleton<FolderLoader>;
  FolderLoader() = default;

 public:
  // Load folder structure from WebSocket message
  // @param msg The complete WebSocket message including op_code
  void Load(const std::string& /* msg */) {}

 public:
  static std::shared_ptr<FolderLoader> Instance();
};

}  // namespace handler
}  // namespace client
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_CLIENT_HANDLER_LOAD_FOLDER_HANDLER_H_
