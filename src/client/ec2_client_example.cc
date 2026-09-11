/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// Example gRPC client for the EC2 instance management RPC.

#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

#include "grpc++/grpc++.h"
#include "src/proto/service.grpc.pb.h"

namespace {

using bazel_template::proto::EC2InstanceRequest;
using bazel_template::proto::EC2InstanceResponse;
using bazel_template::proto::MEGAService;
using bazel_template::proto::OpCode;

class EC2Client {
 public:
  explicit EC2Client(const std::shared_ptr<grpc::Channel>& channel)
      : stub_(MEGAService::NewStub(channel)) {}

  // Manage an EC2 instance (start or stop based on the op code).
  bool ManageInstance(const std::string& instance_id, OpCode operation,
                      const std::string& region) {
    EC2InstanceRequest request;
    request.set_op(operation);
    request.set_instance_id(instance_id);
    if (!region.empty()) {
      request.set_region(region);
    }

    EC2InstanceResponse response;
    grpc::ClientContext context;
    const grpc::Status status =
        stub_->EC2InstanceManagement(&context, request, &response);
    const std::string_view operation_name =
        operation == OpCode::OP_EC2_START ? "Start" : "Stop";
    if (!status.ok()) {
      std::cout << operation_name
                << " request failed: " << status.error_message() << '\n';
      return false;
    }
    std::cout << operation_name
              << " request successful for instance: " << response.instance_id()
              << '\n'
              << "Status: " << response.status() << '\n'
              << "Message: " << response.message() << '\n';
    return response.err_code() == bazel_template::proto::ErrCode::SUCCESS;
  }

  bool StartInstance(const std::string& instance_id,
                     const std::string& region) {
    return ManageInstance(instance_id, OpCode::OP_EC2_START, region);
  }

  bool StopInstance(const std::string& instance_id, const std::string& region) {
    return ManageInstance(instance_id, OpCode::OP_EC2_STOP, region);
  }

 private:
  std::unique_ptr<MEGAService::Stub> stub_;
};

int Run(int argc, char** argv) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0]
              << " <server_address:port> <instance_id> [region] [start|stop]\n"
              << "Example: " << argv[0]
              << " localhost:50051 i-1234567890abcdef0 us-west-2 start\n";
    return 1;
  }

  const std::string server_address = argv[1];
  const std::string instance_id = argv[2];
  const std::string region = argc > 3 ? argv[3] : "";
  const std::string operation = argc > 4 ? argv[4] : "start";

  auto channel =
      grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
  EC2Client client(channel);

  bool success = false;
  if (operation == "start") {
    std::cout << "Starting EC2 instance: " << instance_id << '\n';
    success = client.StartInstance(instance_id, region);
  } else if (operation == "stop") {
    std::cout << "Stopping EC2 instance: " << instance_id << '\n';
    success = client.StopInstance(instance_id, region);
  } else {
    std::cout << "Invalid operation. Use 'start' or 'stop'.\n";
    return 1;
  }

  std::cout << (success ? "Operation completed successfully.\n"
                        : "Operation failed.\n");
  return success ? 0 : 1;
}

// noexcept: reporting from a catch handler must not throw again, which is
// what keeps main() exception-safe.
void ReportFailure(const char* what) noexcept {
  try {
    std::cerr << "Failed" << (what == nullptr ? "" : ": ")
              << (what == nullptr ? "" : what) << '\n';
  } catch (...) {  // NOLINT(bugprone-empty-catch)
  }
}

}  // namespace

int main(int argc, char** argv) {
  try {
    return Run(argc, argv);
  } catch (const std::exception& e) {
    ReportFailure(e.what());
    return 1;
  } catch (...) {
    ReportFailure(nullptr);
    return 1;
  }
}
