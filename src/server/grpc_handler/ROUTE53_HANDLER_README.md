# Route53 DNS Management Handler

This module provides a unified gRPC handler for managing Route53 DNS records (A and CNAME records) using the AWS SDK for C++.

## Features

- Update A records (IP address mapping)
- Update CNAME records (domain name aliases)
- Support for different AWS regions
- Configurable TTL values
- Error handling and logging
- Thread-safe operations
- Change tracking with Route53 change IDs

## Prerequisites

1. **AWS Credentials**: Make sure you have AWS credentials configured with Route53 permissions:
   - AWS CLI: `aws configure`
   - Environment variables: `AWS_ACCESS_KEY_ID`, `AWS_SECRET_ACCESS_KEY`, `AWS_DEFAULT_REGION`
   - IAM roles (if running on EC2)

2. **Route53 Permissions**: Your AWS credentials need the following Route53 permissions:
   - `route53:ChangeResourceRecordSets`
   - `route53:GetChange`
   - `route53:ListResourceRecordSets`

3. **AWS SDK C++**: The project includes AWS SDK C++ dependencies.

## API

### Proto Definitions

The Route53 DNS management is defined in `src/proto/service.proto`:

```protobuf
service MEGAService {
  rpc Route53Management(Route53Request) returns (Route53Response) {}
}

message Route53Request {
  OpCode op = 1;
  string hosted_zone_id = 2;  // Route53 hosted zone ID
  string domain_name = 3;     // Domain name to update (e.g., "api.example.com")
  string new_value = 4;       // New IP address (for A record) or domain (for CNAME)
  string record_type = 5;     // Record type: "A" or "CNAME"
  int32 ttl = 6;              // TTL in seconds (optional, defaults to 300)
  string region = 7;          // AWS region (optional, defaults to configured region)
}

message Route53Response {
  ErrCode err_code = 1;
  string domain_name = 2;     // The domain name that was updated
  string record_type = 3;     // The record type that was updated
  string new_value = 4;       // The new value that was set
  string change_id = 5;       // Route53 change ID for tracking
  string message = 6;         // Additional information or error message
}
```

### Operation Codes

- `OP_ROUTE53_UPDATE_A_RECORD = 6`: Update an A record (IP address)
- `OP_ROUTE53_UPDATE_CNAME_RECORD = 7`: Update a CNAME record (domain alias)

## Usage

### Server Side

The `Route53ManagementHandler` is automatically registered with the gRPC server. The handler will:

1. Initialize the AWS SDK if not already initialized
2. Create a Route53 client
3. Execute the requested operation (A/CNAME record update) based on the op code
4. Return the result with appropriate error codes and change tracking

### Client Side

Use the provided example client (`src/client/route53_client_example.cc`) to interact with the Route53 handler:

```bash
# Build the example client
bazel build //src/client:route53_client_example

# Update an A record
./bazel-bin/src/client/route53_client_example localhost:50051 Z1234567890ABC api.example.com 192.168.1.100 A 300 us-west-2

# Update a CNAME record
./bazel-bin/src/client/route53_client_example localhost:50051 Z1234567890ABC www.example.com example.com CNAME 600 us-east-1
```

### Programmatic Usage

```cpp
#include "grpc++/grpc++.h"
#include "src/proto/service.grpc.pb.h"

// Create a gRPC channel
auto channel = grpc::CreateChannel("localhost:50051", 
                                 grpc::InsecureChannelCredentials());
auto stub = bazel_template::proto::MEGAService::NewStub(channel);

// Create an A record update request
bazel_template::proto::Route53Request request;
request.set_op(bazel_template::proto::OpCode::OP_ROUTE53_UPDATE_A_RECORD);
request.set_hosted_zone_id("Z1234567890ABC");
request.set_domain_name("api.example.com");
request.set_new_value("192.168.1.100");
request.set_record_type("A");
request.set_ttl(300);
request.set_region("us-west-2");

// Send the request
bazel_template::proto::Route53Response response;
grpc::ClientContext context;
auto status = stub->Route53Management(&context, request, &response);

if (status.ok() && response.err_code() == bazel_template::proto::ErrCode::SUCCESS) {
    std::cout << "A record updated successfully" << std::endl;
    std::cout << "Change ID: " << response.change_id() << std::endl;
} else {
    std::cout << "Failed to update A record: " << response.message() << std::endl;
}

// Create a CNAME record update request
request.set_op(bazel_template::proto::OpCode::OP_ROUTE53_UPDATE_CNAME_RECORD);
request.set_domain_name("www.example.com");
request.set_new_value("example.com");
request.set_record_type("CNAME");
status = stub->Route53Management(&context, request, &response);

if (status.ok() && response.err_code() == bazel_template::proto::ErrCode::SUCCESS) {
    std::cout << "CNAME record updated successfully" << std::endl;
    std::cout << "Change ID: " << response.change_id() << std::endl;
} else {
    std::cout << "Failed to update CNAME record: " << response.message() << std::endl;
}
```

## Handler Implementation

The `Route53ManagementHandler` class:

- Inherits from `async_grpc::RpcHandler<Route53ManagementMethod>`
- Uses a switch statement to handle different operation codes
- Provides separate private methods for A and CNAME record updates
- Handles AWS SDK initialization and cleanup
- Includes comprehensive error handling and logging
- Uses UPSERT action to create or update records

```cpp
class Route53ManagementHandler : public async_grpc::RpcHandler<Route53ManagementMethod> {
  // Handles both A and CNAME record updates based on request.op()
  void OnRequest(const proto::Route53Request& req) override;
  
private:
  void HandleUpdateARecord(const proto::Route53Request& req, 
                          proto::Route53Response* res);
  void HandleUpdateCNAMERecord(const proto::Route53Request& req, 
                              proto::Route53Response* res);
};
```

## DNS Record Types

### A Records
- Maps a domain name to an IPv4 address
- Example: `api.example.com` → `192.168.1.100`
- Use `OP_ROUTE53_UPDATE_A_RECORD` operation

### CNAME Records
- Maps a domain name to another domain name (alias)
- Example: `www.example.com` → `example.com`
- Use `OP_ROUTE53_UPDATE_CNAME_RECORD` operation

## Error Handling

The handler returns appropriate error codes:

- `SUCCESS = 0`: Operation completed successfully
- `FAIL = 2`: Operation failed (check the message field for details)

Common error scenarios:
- Invalid hosted zone ID
- Insufficient Route53 permissions
- Invalid domain name format
- Network connectivity issues
- AWS service errors
- Invalid operation codes
- Invalid TTL values

## Testing

Run the Route53 handler tests:

```bash
bazel test //src/server/grpc_handler:route53_handler_test
```

## Security Considerations

1. **AWS Credentials**: Ensure AWS credentials are properly secured and have minimal required Route53 permissions
2. **Network Security**: Use TLS for production deployments
3. **Access Control**: Implement proper authentication and authorization for the gRPC service
4. **DNS Security**: Be careful with DNS changes as they can affect production services
5. **Change Tracking**: Always verify the change ID returned for audit purposes

## Dependencies

- AWS SDK for C++ (aws-cpp-sdk-core, aws-cpp-sdk-route53)
- gRPC
- Protocol Buffers
- glog (for logging)

## Configuration

The handler uses the default AWS configuration. You can customize the AWS client configuration by modifying the `Route53ManagementHandler` class to accept custom configuration options.

## Benefits of Unified Interface

1. **Simplified API**: Single endpoint for all Route53 DNS management operations
2. **Consistent Error Handling**: Unified error handling across all operations
3. **Easier Maintenance**: Single handler class to maintain
4. **Extensible**: Easy to add new Route53 operations (MX, TXT, etc.) in the future
5. **Reduced Code Duplication**: Shared AWS SDK initialization and cleanup logic
6. **Change Tracking**: Built-in support for Route53 change IDs

## Example Use Cases

### Load Balancer Failover
```bash
# Update A record to point to backup server
./route53_client_example localhost:50051 Z1234567890ABC api.example.com 10.0.1.100 A 60 us-west-2
```

### Blue-Green Deployment
```bash
# Update CNAME to point to new environment
./route53_client_example localhost:50051 Z1234567890ABC app.example.com new-env.example.com CNAME 300 us-east-1
```

### Disaster Recovery
```bash
# Point domain to backup region
./route53_client_example localhost:50051 Z1234567890ABC www.example.com backup.example.com CNAME 60 us-west-2
``` 