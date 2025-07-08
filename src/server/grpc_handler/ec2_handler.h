/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_GRPC_HANDLER_EC2_HANDLER_H_
#define BAZEL_TEMPLATE_SERVER_GRPC_HANDLER_EC2_HANDLER_H_

#include <memory>
#include <string>

#include "aws/core/Aws.h"
#include "aws/ec2/EC2Client.h"
#include "aws/ec2/model/StartInstancesRequest.h"
#include "aws/ec2/model/StopInstancesRequest.h"
#include "glog/logging.h"
#include "src/async_grpc/rpc_handler.h"
#include "src/server/grpc_handler/meta.h"

namespace bazel_template {
namespace server {
namespace grpc_handler {

class EC2InstanceManagementHandler : public async_grpc::RpcHandler<EC2InstanceManagementMethod> {
 public:
  EC2InstanceManagementHandler() {
    // Initialize AWS SDK
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    aws_initialized_ = true;
  }

  ~EC2InstanceManagementHandler() {
    // Cleanup AWS SDK if we initialized it
    if (aws_initialized_) {
      Aws::SDKOptions options;
      Aws::ShutdownAPI(options);
    }
  }

  void OnRequest(const proto::EC2InstanceRequest& req) override {
    auto res = std::make_unique<proto::EC2InstanceResponse>();
    res->set_instance_id(req.instance_id());

    LOG(INFO) << "EC2 instance management request: " << req.op()
              << " for instance: " << req.instance_id();

    try {
      // Determine the operation based on the request op code
      switch (req.op()) {
        case proto::OpCode::OP_EC2_START:
          HandleStartInstance(req, res.get());
          break;
        case proto::OpCode::OP_EC2_STOP:
          HandleStopInstance(req, res.get());
          break;
        default:
          res->set_err_code(proto::ErrCode::FAIL);
          res->set_message("Invalid operation code for EC2 instance management");
          LOG(ERROR) << "Invalid operation code: " << req.op();
          break;
      }
    } catch (const std::exception& e) {
      LOG(ERROR) << "EC2 instance management operation failed: " << e.what();
      res->set_err_code(proto::ErrCode::FAIL);
      res->set_message(std::string("Operation failed: ") + e.what());
    }

    Send(std::move(res));
  }

  void OnReadsDone() override { Finish(grpc::Status::OK); }

 private:
  void HandleStartInstance(const proto::EC2InstanceRequest& req,
                          proto::EC2InstanceResponse* res) {
    Aws::EC2::EC2Client ec2_client;

    // Set region if specified
    if (!req.region().empty()) {
      ec2_client = Aws::EC2::EC2Client(req.region());
    }

    Aws::EC2::Model::StartInstancesRequest start_request;
    start_request.AddInstanceIds(req.instance_id());

    auto outcome = ec2_client.StartInstances(start_request);

    if (outcome.IsSuccess()) {
      res->set_err_code(proto::ErrCode::SUCCESS);
      res->set_status("starting");
      res->set_message("Instance start request submitted successfully");
      LOG(INFO) << "Successfully started instance: " << req.instance_id();
    } else {
      res->set_err_code(proto::ErrCode::FAIL);
      res->set_message("Failed to start instance: " +
                      outcome.GetError().GetMessage());
      LOG(ERROR) << "Failed to start instance: " << req.instance_id()
                 << " - " << outcome.GetError().GetMessage();
    }
  }

  void HandleStopInstance(const proto::EC2InstanceRequest& req,
                         proto::EC2InstanceResponse* res) {
    Aws::EC2::EC2Client ec2_client;

    // Set region if specified
    if (!req.region().empty()) {
      ec2_client = Aws::EC2::EC2Client(req.region());
    }

    Aws::EC2::Model::StopInstancesRequest stop_request;
    stop_request.AddInstanceIds(req.instance_id());

    auto outcome = ec2_client.StopInstances(stop_request);

    if (outcome.IsSuccess()) {
      res->set_err_code(proto::ErrCode::SUCCESS);
      res->set_status("stopping");
      res->set_message("Instance stop request submitted successfully");
      LOG(INFO) << "Successfully stopped instance: " << req.instance_id();
    } else {
      res->set_err_code(proto::ErrCode::FAIL);
      res->set_message("Failed to stop instance: " +
                      outcome.GetError().GetMessage());
      LOG(ERROR) << "Failed to stop instance: " << req.instance_id()
                 << " - " << outcome.GetError().GetMessage();
    }
  }

  bool aws_initialized_ = false;
};

}  // namespace grpc_handler
}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_GRPC_HANDLER_EC2_HANDLER_H_
