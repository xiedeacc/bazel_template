/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_GRPC_HANDLERS_GET_RUNNING_SUM_HANDLER_H
#define BAZEL_TEMPLATE_SERVER_GRPC_HANDLERS_GET_RUNNING_SUM_HANDLER_H

#include "src/async_grpc/rpc_handler.h"
#include "src/proto/service.pb.h"
#include "src/server/grpc_handler/meta.h"

namespace bazel_template {
namespace server {
namespace grpc_handler {

class GetRunningSumHandler
    : public async_grpc::RpcHandler<GetRunningSumMethod> {
 public:
  void OnRequest(const proto::GetSumRequest& request) override {
    sum_ += request.input();

    // Respond twice to demonstrate bidirectional streaming.
    auto response = std::make_unique<proto::GetSumResponse>();
    response->set_output(sum_);
    Send(std::move(response));
    response = std::make_unique<proto::GetSumResponse>();
    response->set_output(sum_);
    Send(std::move(response));
  }

  void OnReadsDone() override { Finish(grpc::Status::OK); }

 private:
  int sum_ = 0;
};

}  // namespace grpc_handler
}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_GRPC_HANDLERS_GET_RUNNING_SUM_HANDLER_H
