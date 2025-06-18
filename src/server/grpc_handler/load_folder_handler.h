/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_GRPC_HANDLER_LOAD_FOLDER_HANDLER_H_
#define BAZEL_TEMPLATE_SERVER_GRPC_HANDLER_LOAD_FOLDER_HANDLER_H_

#include <memory>

#include "glog/logging.h"
#include "src/async_grpc/rpc_handler.h"
#include "src/server/grpc_handler/meta.h"

namespace bazel_template {
namespace server {
namespace grpc_handler {

class LoadFolderHandler : public async_grpc::RpcHandler<LoadFolderMethod> {
 public:
  void OnRequest(const proto::LoadFolderRequest& req) override {
    auto res = std::make_unique<proto::LoadFolderResponse>();
    LOG(INFO) << "request: " << req.folder_id();
    Send(std::move(res));
  }

  void OnReadsDone() override { Finish(grpc::Status::OK); }
};

}  // namespace grpc_handler
}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_GRPC_HANDLER_LOAD_FOLDER_HANDLER_H_
