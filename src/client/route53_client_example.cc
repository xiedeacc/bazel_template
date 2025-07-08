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
using bazel_template::proto::Route53Request;
using bazel_template::proto::Route53Response;
using bazel_template::proto::OpCode;

class Route53Client {
 public:
  Route53Client(std::shared_ptr<Channel> channel)
      : stub_(MEGAService::NewStub(channel)) {}

  // Manage Route53 DNS records (update A/CNAME based on op code)
  bool ManageDNSRecord(const std::string& hosted_zone_id,
                      const std::string& domain_name,
                      const std::string& new_value,
                      const std::string& record_type,
                      OpCode operation,
                      int32_t ttl = 300,
                      const std::string& region = "") {
    Route53Request request;
    request.set_op(operation);
    request.set_hosted_zone_id(hosted_zone_id);
    request.set_domain_name(domain_name);
    request.set_new_value(new_value);
    request.set_record_type(record_type);
    request.set_ttl(ttl);
    if (!region.empty()) {
      request.set_region(region);
    }

    Route53Response response;
    ClientContext context;

    Status status = stub_->Route53Management(&context, request, &response);

    if (status.ok()) {
      std::string operation_name = (operation == OpCode::OP_ROUTE53_UPDATE_A_RECORD) ? "A Record Update" : "CNAME Record Update";
      std::cout << operation_name << " successful for domain: " 
                << response.domain_name() << std::endl;
      std::cout << "Record Type: " << response.record_type() << std::endl;
      std::cout << "New Value: " << response.new_value() << std::endl;
      std::cout << "Change ID: " << response.change_id() << std::endl;
      std::cout << "Message: " << response.message() << std::endl;
      return response.err_code() == bazel_template::proto::ErrCode::SUCCESS;
    } else {
      std::string operation_name = (operation == OpCode::OP_ROUTE53_UPDATE_A_RECORD) ? "A Record Update" : "CNAME Record Update";
      std::cout << operation_name << " failed: " << status.error_message() 
                << std::endl;
      return false;
    }
  }

  // Convenience methods for A and CNAME record updates
  bool UpdateARecord(const std::string& hosted_zone_id,
                    const std::string& domain_name,
                    const std::string& new_ip_address,
                    int32_t ttl = 300,
                    const std::string& region = "") {
    return ManageDNSRecord(hosted_zone_id, domain_name, new_ip_address, "A",
                          OpCode::OP_ROUTE53_UPDATE_A_RECORD, ttl, region);
  }

  bool UpdateCNAMERecord(const std::string& hosted_zone_id,
                        const std::string& domain_name,
                        const std::string& new_domain_name,
                        int32_t ttl = 300,
                        const std::string& region = "") {
    return ManageDNSRecord(hosted_zone_id, domain_name, new_domain_name, "CNAME",
                          OpCode::OP_ROUTE53_UPDATE_CNAME_RECORD, ttl, region);
  }

 private:
  std::unique_ptr<MEGAService::Stub> stub_;
};

int main(int argc, char** argv) {
  if (argc < 5) {
    std::cout << "Usage: " << argv[0] 
              << " <server_address:port> <hosted_zone_id> <domain_name> <new_value> [record_type] [ttl] [region]"
              << std::endl;
    std::cout << "Example A Record: " << argv[0] 
              << " localhost:50051 Z1234567890ABC api.example.com 192.168.1.100 A 300 us-west-2"
              << std::endl;
    std::cout << "Example CNAME Record: " << argv[0] 
              << " localhost:50051 Z1234567890ABC www.example.com example.com CNAME 600 us-east-1"
              << std::endl;
    return 1;
  }

  std::string server_address = argv[1];
  std::string hosted_zone_id = argv[2];
  std::string domain_name = argv[3];
  std::string new_value = argv[4];
  std::string record_type = (argc > 5) ? argv[5] : "A";
  int32_t ttl = (argc > 6) ? std::stoi(argv[6]) : 300;
  std::string region = (argc > 7) ? argv[7] : "";

  // Create a gRPC channel
  auto channel = grpc::CreateChannel(server_address, 
                                   grpc::InsecureChannelCredentials());
  Route53Client client(channel);

  bool success = false;
  if (record_type == "A") {
    std::cout << "Updating A record for domain: " << domain_name 
              << " to IP: " << new_value << std::endl;
    success = client.UpdateARecord(hosted_zone_id, domain_name, new_value, ttl, region);
  } else if (record_type == "CNAME") {
    std::cout << "Updating CNAME record for domain: " << domain_name 
              << " to: " << new_value << std::endl;
    success = client.UpdateCNAMERecord(hosted_zone_id, domain_name, new_value, ttl, region);
  } else {
    std::cout << "Invalid record type. Use 'A' or 'CNAME'." << std::endl;
    return 1;
  }

  if (success) {
    std::cout << "DNS record update completed successfully." << std::endl;
    return 0;
  } else {
    std::cout << "DNS record update failed." << std::endl;
    return 1;
  }
} 