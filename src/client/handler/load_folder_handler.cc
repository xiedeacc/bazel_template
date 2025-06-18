/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/client/handler/load_folder_handler.h"

namespace bazel_template {
namespace client {
namespace handler {

static folly::Singleton<FolderLoader> folder_loader;

std::shared_ptr<FolderLoader> FolderLoader::Instance() {
  return folder_loader.try_get();
}

}  // namespace handler
}  // namespace client
}  // namespace bazel_template
