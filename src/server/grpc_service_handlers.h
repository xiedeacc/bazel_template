/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_GRPC_HANDLERS_H
#define BAZEL_TEMPLATE_SERVER_GRPC_HANDLERS_H

#include "glog/logging.h"
#include "google/protobuf/descriptor.h"
#include "grpc++/grpc++.h"
#include "src/async_grpc/async_client.h"
#include "src/async_grpc/client.h"
#include "src/async_grpc/execution_context.h"
#include "src/async_grpc/retry.h"
#include "src/async_grpc/rpc_handler.h"

namespace bazel_template {
namespace server {

class MathServerContext : public ExecutionContext {
 public:
  int additional_increment() { return 10; }
  std::promise<EchoResponder> echo_responder;
};

struct GetSumMethod {
  static constexpr const char* MethodName() { return "/src.proto.Math/GetSum"; }
  using IncomingType = Stream<proto::GetSumRequest>;
  using OutgoingType = proto::GetSumResponse;
};

struct GetRunningSumMethod {
  static constexpr const char* MethodName() {
    return "/src.proto.Math/GetRunningSum";
  }
  using IncomingType = Stream<proto::GetSumRequest>;
  using OutgoingType = Stream<proto::GetSumResponse>;
};

class GetRunningSumHandler : public RpcHandler<GetRunningSumMethod> {
 public:
  void OnRequest(const proto::GetSumRequest& request) override {
    sum_ += request.input();

    // Respond twice to demonstrate bidirectional streaming.
    auto response = common::make_unique<proto::GetSumResponse>();
    response->set_output(sum_);
    Send(std::move(response));
    response = common::make_unique<proto::GetSumResponse>();
    response->set_output(sum_);
    Send(std::move(response));
  }

  void OnReadsDone() override { Finish(::grpc::Status::OK); }

 private:
  int sum_ = 0;
};

class GetSumHandler : public RpcHandler<GetSumMethod> {
 public:
  void OnRequest(const proto::GetSumRequest& request) override {
    sum_ += GetContext<MathServerContext>()->additional_increment();
    sum_ += request.input();
  }

  void OnReadsDone() override {
    auto response = common::make_unique<proto::GetSumResponse>();
    response->set_output(sum_);
    Send(std::move(response));
  }

 private:
  int sum_ = 0;
};

}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_GRPC_HANDLERS_H
