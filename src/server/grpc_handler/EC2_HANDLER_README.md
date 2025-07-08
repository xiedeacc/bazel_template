# EC2 Instance Management Handler

This module provides a unified gRPC handler for managing EC2 instances (start/stop operations) using the AWS SDK for C++.

## Features

- Start EC2 instances
- Stop EC2 instances
- Unified interface for both operations
- Support for different AWS regions
- Error handling and logging
- Thread-safe operations

## Prerequisites

1. **AWS Credentials**: Make sure you have AWS credentials configured. You can set them up using:
   - AWS CLI: `aws configure`
   - Environment variables: `AWS_ACCESS_KEY_ID`, `AWS_SECRET_ACCESS_KEY`, `AWS_DEFAULT_REGION`
   - IAM roles (if running on EC2)

2. **AWS SDK C++**: The project already includes AWS SDK C++ dependencies.

## API

### Proto Definitions

The EC2 instance management is defined in `src/proto/service.proto`:

```protobuf
service MEGAService {
  rpc EC2InstanceManagement(EC2InstanceRequest) returns (EC2InstanceResponse) {}
}

message EC2InstanceRequest {
  OpCode op = 1;
  string instance_id = 2;  // The EC2 instance ID
  string region = 3;       // AWS region (optional)
}

message EC2InstanceResponse {
  ErrCode err_code = 1;
  string instance_id = 2;     // The EC2 instance ID
  string status = 3;          // Current instance status
  string message = 4;         // Additional information or error message
}
```

### Operation Codes

- `OP_EC2_START = 4`: Start an EC2 instance
- `OP_EC2_STOP = 5`: Stop an EC2 instance

## Usage

### Server Side

The `EC2InstanceManagementHandler` is automatically registered with the gRPC server. The handler will:

1. Initialize the AWS SDK if not already initialized
2. Create an EC2 client
3. Execute the requested operation (start/stop) based on the op code
4. Return the result with appropriate error codes

### Client Side

Use the provided example client (`src/client/ec2_client_example.cc`) to interact with the EC2 handler:

```bash
# Build the example client
bazel build //src/client:ec2_client_example

# Start an EC2 instance
./bazel-bin/src/client/ec2_client_example localhost:50051 i-1234567890abcdef0 us-west-2 start

# Stop an EC2 instance
./bazel-bin/src/client/ec2_client_example localhost:50051 i-1234567890abcdef0 us-west-2 stop
```

### Programmatic Usage

```cpp
#include "grpc++/grpc++.h"
#include "src/proto/service.grpc.pb.h"

// Create a gRPC channel
auto channel = grpc::CreateChannel("localhost:50051", 
                                 grpc::InsecureChannelCredentials());
auto stub = bazel_template::proto::MEGAService::NewStub(channel);

// Create a start request
bazel_template::proto::EC2InstanceRequest request;
request.set_op(bazel_template::proto::OpCode::OP_EC2_START);
request.set_instance_id("i-1234567890abcdef0");
request.set_region("us-west-2");

// Send the request
bazel_template::proto::EC2InstanceResponse response;
grpc::ClientContext context;
auto status = stub->EC2InstanceManagement(&context, request, &response);

if (status.ok() && response.err_code() == bazel_template::proto::ErrCode::SUCCESS) {
    std::cout << "Instance started successfully" << std::endl;
} else {
    std::cout << "Failed to start instance: " << response.message() << std::endl;
}

// Create a stop request
request.set_op(bazel_template::proto::OpCode::OP_EC2_STOP);
status = stub->EC2InstanceManagement(&context, request, &response);

if (status.ok() && response.err_code() == bazel_template::proto::ErrCode::SUCCESS) {
    std::cout << "Instance stopped successfully" << std::endl;
} else {
    std::cout << "Failed to stop instance: " << response.message() << std::endl;
}
```

## Handler Implementation

The `EC2InstanceManagementHandler` class:

- Inherits from `async_grpc::RpcHandler<EC2InstanceManagementMethod>`
- Uses a switch statement to handle different operation codes
- Provides separate private methods for start and stop operations
- Handles AWS SDK initialization and cleanup
- Includes comprehensive error handling and logging

```cpp
class EC2InstanceManagementHandler : public async_grpc::RpcHandler<EC2InstanceManagementMethod> {
  // Handles both start and stop operations based on request.op()
  void OnRequest(const proto::EC2InstanceRequest& req) override;
  
private:
  void HandleStartInstance(const proto::EC2InstanceRequest& req, 
                          proto::EC2InstanceResponse* res);
  void HandleStopInstance(const proto::EC2InstanceRequest& req, 
                         proto::EC2InstanceResponse* res);
};
```

## Error Handling

The handler returns appropriate error codes:

- `SUCCESS = 0`: Operation completed successfully
- `FAIL = 2`: Operation failed (check the message field for details)

Common error scenarios:
- Invalid instance ID
- Insufficient permissions
- Instance already in the requested state
- Network connectivity issues
- AWS service errors
- Invalid operation codes

## Testing

Run the EC2 handler tests:

```bash
bazel test //src/server/grpc_handler:ec2_handler_test
```

## Security Considerations

1. **AWS Credentials**: Ensure AWS credentials are properly secured and have minimal required permissions
2. **Network Security**: Use TLS for production deployments
3. **Access Control**: Implement proper authentication and authorization for the gRPC service
4. **Instance Permissions**: The AWS credentials should have EC2 permissions for the instances you want to manage

## Dependencies

- AWS SDK for C++ (aws-cpp-sdk-core, aws-cpp-sdk-ec2)
- gRPC
- Protocol Buffers
- glog (for logging)

## Configuration

The handler uses the default AWS configuration. You can customize the AWS client configuration by modifying the `EC2InstanceManagementHandler` class to accept custom configuration options.

## Benefits of Unified Interface

1. **Simplified API**: Single endpoint for all EC2 instance management operations
2. **Consistent Error Handling**: Unified error handling across all operations
3. **Easier Maintenance**: Single handler class to maintain
4. **Extensible**: Easy to add new EC2 operations (reboot, terminate, etc.) in the future
5. **Reduced Code Duplication**: Shared AWS SDK initialization and cleanup logic 