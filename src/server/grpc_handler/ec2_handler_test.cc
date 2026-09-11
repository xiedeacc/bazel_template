/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <memory>

#include "grpc++/grpc++.h"
#include "gtest/gtest.h"
#include "src/proto/service.pb.h"

import bazel_template.async_grpc;
import bazel_template.server.grpc_handler;

namespace bazel_template {
namespace server {
namespace grpc_handler {

class EC2InstanceManagementHandlerTest : public ::testing::Test {};

TEST_F(EC2InstanceManagementHandlerTest, HandlersRegister) {
  async_grpc::Server::Builder builder;
  RegisterHandlers(builder);
}

TEST_F(EC2InstanceManagementHandlerTest, CreateStartRequest) {
  proto::EC2InstanceRequest req;
  req.set_op(proto::OpCode::OP_EC2_START);
  req.set_instance_id("i-1234567890abcdef0");
  req.set_region("us-west-2");

  EXPECT_EQ(req.op(), proto::OpCode::OP_EC2_START);
  EXPECT_EQ(req.instance_id(), "i-1234567890abcdef0");
  EXPECT_EQ(req.region(), "us-west-2");
}

TEST_F(EC2InstanceManagementHandlerTest, CreateStopRequest) {
  proto::EC2InstanceRequest req;
  req.set_op(proto::OpCode::OP_EC2_STOP);
  req.set_instance_id("i-1234567890abcdef0");
  req.set_region("us-east-1");

  EXPECT_EQ(req.op(), proto::OpCode::OP_EC2_STOP);
  EXPECT_EQ(req.instance_id(), "i-1234567890abcdef0");
  EXPECT_EQ(req.region(), "us-east-1");
}

TEST_F(EC2InstanceManagementHandlerTest, CreateResponse) {
  proto::EC2InstanceResponse res;
  res.set_err_code(proto::ErrCode::SUCCESS);
  res.set_instance_id("i-1234567890abcdef0");
  res.set_status("running");
  res.set_message("Instance is running");

  EXPECT_EQ(res.err_code(), proto::ErrCode::SUCCESS);
  EXPECT_EQ(res.instance_id(), "i-1234567890abcdef0");
  EXPECT_EQ(res.status(), "running");
  EXPECT_EQ(res.message(), "Instance is running");
}

TEST_F(EC2InstanceManagementHandlerTest, InvalidOperationCode) {
  proto::EC2InstanceRequest req;
  req.set_op(proto::OpCode::OP_UNUSED);  // Invalid op code for EC2
  req.set_instance_id("i-1234567890abcdef0");

  EXPECT_EQ(req.op(), proto::OpCode::OP_UNUSED);
  EXPECT_EQ(req.instance_id(), "i-1234567890abcdef0");
}

}  // namespace grpc_handler
}  // namespace server
}  // namespace bazel_template