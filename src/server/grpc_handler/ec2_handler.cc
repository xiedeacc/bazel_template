/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

module;

#include <exception>
#include <memory>
#include <string>
#include <utility>

#include "aws/core/Aws.h"
#include "aws/core/client/ClientConfiguration.h"
#include "aws/ec2/EC2Client.h"
#include "aws/ec2/model/StartInstancesRequest.h"
#include "aws/ec2/model/StopInstancesRequest.h"
#include "src/common/logging.h"
#include "google/protobuf/message.h"
#include "grpc++/grpc++.h"
#include "grpc++/impl/codegen/proto_utils.h"
#include "src/async_grpc/rpc_service_method_traits.h"
#include "src/async_grpc/type_traits.h"
#include "src/proto/service.pb.h"
#include "src/server/grpc_handler/meta.h"

module bazel_template.server.grpc_handler;

namespace bazel_template::server::grpc_handler {

class EC2InstanceManagementHandler
    : public async_grpc::RpcHandler<EC2InstanceManagementMethod> {
 public:
  EC2InstanceManagementHandler() : aws_initialized_(true) {
    Aws::SDKOptions options;
    Aws::InitAPI(options);
  }

  // Owns AWS SDK state; neither copyable nor movable.
  EC2InstanceManagementHandler(const EC2InstanceManagementHandler&) = delete;
  EC2InstanceManagementHandler& operator=(const EC2InstanceManagementHandler&) =
      delete;
  EC2InstanceManagementHandler(EC2InstanceManagementHandler&&) = delete;
  EC2InstanceManagementHandler& operator=(EC2InstanceManagementHandler&&) =
      delete;

  ~EC2InstanceManagementHandler() override {
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
          res->set_message(
              "Invalid operation code for EC2 instance management");
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
  static void HandleStartInstance(const proto::EC2InstanceRequest& req,
                                  proto::EC2InstanceResponse* res) {
    // Set region if specified
    Aws::Client::ClientConfiguration config;
    if (!req.region().empty()) {
      config.region = req.region();
    }
    Aws::EC2::EC2Client ec2_client(config);

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
      LOG(ERROR) << "Failed to start instance: " << req.instance_id() << " - "
                 << outcome.GetError().GetMessage();
    }
  }

  static void HandleStopInstance(const proto::EC2InstanceRequest& req,
                                 proto::EC2InstanceResponse* res) {
    // Set region if specified
    Aws::Client::ClientConfiguration config;
    if (!req.region().empty()) {
      config.region = req.region();
    }
    Aws::EC2::EC2Client ec2_client(config);

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
      LOG(ERROR) << "Failed to stop instance: " << req.instance_id() << " - "
                 << outcome.GetError().GetMessage();
    }
  }

  bool aws_initialized_ = false;
};

void RegisterEC2Handler(async_grpc::Server::Builder& builder) {
  builder.RegisterHandler<EC2InstanceManagementHandler>();
}

}  // namespace bazel_template::server::grpc_handler
