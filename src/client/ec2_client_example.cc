/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <iostream>
#include <memory>
#include <string>

#include "grpc++/grpc++.h"
#include "src/proto/service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using bazel_template::proto::MEGAService;
using bazel_template::proto::EC2InstanceRequest;
using bazel_template::proto::EC2InstanceResponse;
using bazel_template::proto::OpCode;

class EC2Client {
 public:
  EC2Client(std::shared_ptr<Channel> channel)
      : stub_(MEGAService::NewStub(channel)) {}

  // Manage EC2 instance (start or stop based on op code)
  bool ManageInstance(const std::string& instance_id, 
                     OpCode operation,
                     const std::string& region = "") {
    EC2InstanceRequest request;
    request.set_op(operation);
    request.set_instance_id(instance_id);
    if (!region.empty()) {
      request.set_region(region);
    }

    EC2InstanceResponse response;
    ClientContext context;

    Status status = stub_->EC2InstanceManagement(&context, request, &response);

    if (status.ok()) {
      std::string operation_name = (operation == OpCode::OP_EC2_START) ? "Start" : "Stop";
      std::cout << operation_name << " request successful for instance: " 
                << response.instance_id() << std::endl;
      std::cout << "Status: " << response.status() << std::endl;
      std::cout << "Message: " << response.message() << std::endl;
      return response.err_code() == bazel_template::proto::ErrCode::SUCCESS;
    } else {
      std::string operation_name = (operation == OpCode::OP_EC2_START) ? "Start" : "Stop";
      std::cout << operation_name << " request failed: " << status.error_message() 
                << std::endl;
      return false;
    }
  }

  // Convenience methods for start and stop operations
  bool StartInstance(const std::string& instance_id, 
                    const std::string& region = "") {
    return ManageInstance(instance_id, OpCode::OP_EC2_START, region);
  }

  bool StopInstance(const std::string& instance_id, 
                   const std::string& region = "") {
    return ManageInstance(instance_id, OpCode::OP_EC2_STOP, region);
  }

 private:
  std::unique_ptr<MEGAService::Stub> stub_;
};

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] 
              << " <server_address:port> <instance_id> [region] [start|stop]"
              << std::endl;
    std::cout << "Example: " << argv[0] 
              << " localhost:50051 i-1234567890abcdef0 us-west-2 start"
              << std::endl;
    return 1;
  }

  std::string server_address = argv[1];
  std::string instance_id = argv[2];
  std::string region = (argc > 3) ? argv[3] : "";
  std::string operation = (argc > 4) ? argv[4] : "start";

  // Create a gRPC channel
  auto channel = grpc::CreateChannel(server_address, 
                                   grpc::InsecureChannelCredentials());
  EC2Client client(channel);

  bool success = false;
  if (operation == "start") {
    std::cout << "Starting EC2 instance: " << instance_id << std::endl;
    success = client.StartInstance(instance_id, region);
  } else if (operation == "stop") {
    std::cout << "Stopping EC2 instance: " << instance_id << std::endl;
    success = client.StopInstance(instance_id, region);
  } else {
    std::cout << "Invalid operation. Use 'start' or 'stop'." << std::endl;
    return 1;
  }

  if (success) {
    std::cout << "Operation completed successfully." << std::endl;
    return 0;
  } else {
    std::cout << "Operation failed." << std::endl;
    return 1;
  }
} 