/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/server/grpc_handler/route53_handler.h"

#include <memory>

#include "gtest/gtest.h"
#include "src/proto/service.pb.h"

namespace bazel_template {
namespace server {
namespace grpc_handler {

class Route53ManagementHandlerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    handler_ = std::make_unique<Route53ManagementHandler>();
  }

  void TearDown() override {
    handler_.reset();
  }

  std::unique_ptr<Route53ManagementHandler> handler_;
};

TEST_F(Route53ManagementHandlerTest, HandlerCreation) {
  EXPECT_NE(handler_, nullptr);
}

TEST_F(Route53ManagementHandlerTest, CreateUpdateARecordRequest) {
  proto::Route53Request req;
  req.set_op(proto::OpCode::OP_ROUTE53_UPDATE_A_RECORD);
  req.set_hosted_zone_id("Z1234567890ABC");
  req.set_domain_name("api.example.com");
  req.set_new_value("192.168.1.100");
  req.set_record_type("A");
  req.set_ttl(300);
  req.set_region("us-west-2");

  EXPECT_EQ(req.op(), proto::OpCode::OP_ROUTE53_UPDATE_A_RECORD);
  EXPECT_EQ(req.hosted_zone_id(), "Z1234567890ABC");
  EXPECT_EQ(req.domain_name(), "api.example.com");
  EXPECT_EQ(req.new_value(), "192.168.1.100");
  EXPECT_EQ(req.record_type(), "A");
  EXPECT_EQ(req.ttl(), 300);
  EXPECT_EQ(req.region(), "us-west-2");
}

TEST_F(Route53ManagementHandlerTest, CreateUpdateCNAMERecordRequest) {
  proto::Route53Request req;
  req.set_op(proto::OpCode::OP_ROUTE53_UPDATE_CNAME_RECORD);
  req.set_hosted_zone_id("Z1234567890ABC");
  req.set_domain_name("www.example.com");
  req.set_new_value("example.com");
  req.set_record_type("CNAME");
  req.set_ttl(600);
  req.set_region("us-east-1");

  EXPECT_EQ(req.op(), proto::OpCode::OP_ROUTE53_UPDATE_CNAME_RECORD);
  EXPECT_EQ(req.hosted_zone_id(), "Z1234567890ABC");
  EXPECT_EQ(req.domain_name(), "www.example.com");
  EXPECT_EQ(req.new_value(), "example.com");
  EXPECT_EQ(req.record_type(), "CNAME");
  EXPECT_EQ(req.ttl(), 600);
  EXPECT_EQ(req.region(), "us-east-1");
}

TEST_F(Route53ManagementHandlerTest, CreateResponse) {
  proto::Route53Response res;
  res.set_err_code(proto::ErrCode::SUCCESS);
  res.set_domain_name("api.example.com");
  res.set_record_type("A");
  res.set_new_value("192.168.1.100");
  res.set_change_id("C1234567890ABC");
  res.set_message("A record updated successfully");

  EXPECT_EQ(res.err_code(), proto::ErrCode::SUCCESS);
  EXPECT_EQ(res.domain_name(), "api.example.com");
  EXPECT_EQ(res.record_type(), "A");
  EXPECT_EQ(res.new_value(), "192.168.1.100");
  EXPECT_EQ(res.change_id(), "C1234567890ABC");
  EXPECT_EQ(res.message(), "A record updated successfully");
}

TEST_F(Route53ManagementHandlerTest, InvalidOperationCode) {
  proto::Route53Request req;
  req.set_op(proto::OpCode::OP_UNUSED);  // Invalid op code for Route53
  req.set_domain_name("api.example.com");

  EXPECT_EQ(req.op(), proto::OpCode::OP_UNUSED);
  EXPECT_EQ(req.domain_name(), "api.example.com");
}

}  // namespace grpc_handler
}  // namespace server
}  // namespace bazel_template 