/*******************************************************************************
 * Copyright (c) 2022 Copyright 2022- xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_GRPC_SERVER_BASE_H
#define BAZEL_TEMPLATE_GRPC_SERVER_BASE_H

#include "grpcpp/grpcpp.h"
#include "src/common/grpc_framework/server_impl.h"
#include "src/proto/service.grpc.pb.h"
#include "src/proto/service.pb.h"

namespace bazel_template {
namespace server {

class GrpcServerBase
    : public bazel_template::common::grpc_framework::ServerImpl<
          proto::Service> {
 public:
  GrpcServerBase(int port) : ServerImpl(port) {}

  virtual void OnServerStatusRead(const proto::ServerStatusRequest *request,
                                  proto::ServerStatusResponse *response) = 0;

  virtual void OnIpInfoRead(const proto::IpInfoRequest *request,
                            proto::IpInfoResponse *response) = 0;
};

}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_GRPC_SERVER_BASE_H
