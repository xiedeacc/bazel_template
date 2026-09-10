/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// Example gRPC client for the Route53 DNS record management RPC.

#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>

#include "grpc++/grpc++.h"
#include "src/proto/service.grpc.pb.h"

namespace {

using bazel_template::proto::MEGAService;
using bazel_template::proto::OpCode;
using bazel_template::proto::Route53Request;
using bazel_template::proto::Route53Response;

constexpr int32_t kDefaultTtl = 300;

class Route53Client {
 public:
  explicit Route53Client(const std::shared_ptr<grpc::Channel>& channel)
      : stub_(MEGAService::NewStub(channel)) {}

  // Manage a Route53 record (A or CNAME upsert based on the op code).
  bool ManageDNSRecord(const std::string& hosted_zone_id,
                       const std::string& domain_name,
                       const std::string& new_value,
                       const std::string& record_type, OpCode operation,
                       int32_t ttl, const std::string& region) {
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
    grpc::ClientContext context;
    const grpc::Status status =
        stub_->Route53Management(&context, request, &response);
    const std::string_view operation_name =
        operation == OpCode::OP_ROUTE53_UPDATE_A_RECORD ? "A Record Update"
                                                        : "CNAME Record Update";
    if (!status.ok()) {
      std::cout << operation_name << " failed: " << status.error_message()
                << '\n';
      return false;
    }
    std::cout << operation_name
              << " successful for domain: " << response.domain_name() << '\n'
              << "Record Type: " << response.record_type() << '\n'
              << "New Value: " << response.new_value() << '\n'
              << "Change ID: " << response.change_id() << '\n'
              << "Message: " << response.message() << '\n';
    return response.err_code() == bazel_template::proto::ErrCode::SUCCESS;
  }

  bool UpdateARecord(const std::string& hosted_zone_id,
                     const std::string& domain_name,
                     const std::string& new_ip_address, int32_t ttl,
                     const std::string& region) {
    return ManageDNSRecord(hosted_zone_id, domain_name, new_ip_address, "A",
                           OpCode::OP_ROUTE53_UPDATE_A_RECORD, ttl, region);
  }

  bool UpdateCNAMERecord(const std::string& hosted_zone_id,
                         const std::string& domain_name,
                         const std::string& new_domain_name, int32_t ttl,
                         const std::string& region) {
    return ManageDNSRecord(hosted_zone_id, domain_name, new_domain_name,
                           "CNAME", OpCode::OP_ROUTE53_UPDATE_CNAME_RECORD, ttl,
                           region);
  }

 private:
  std::unique_ptr<MEGAService::Stub> stub_;
};

int Run(int argc, char** argv) {
  if (argc < 5) {
    std::cout << "Usage: " << argv[0]
              << " <server_address:port> <hosted_zone_id> <domain_name> "
                 "<new_value> [record_type] [ttl] [region]\n"
              << "Example A Record: " << argv[0]
              << " localhost:50051 Z1234567890ABC api.example.com "
                 "192.168.1.100 A 300 us-west-2\n"
              << "Example CNAME Record: " << argv[0]
              << " localhost:50051 Z1234567890ABC www.example.com example.com "
                 "CNAME 600 us-east-1\n";
    return 1;
  }

  const std::string server_address = argv[1];
  const std::string hosted_zone_id = argv[2];
  const std::string domain_name = argv[3];
  const std::string new_value = argv[4];
  const std::string record_type = argc > 5 ? argv[5] : "A";
  const int32_t ttl = argc > 6 ? std::stoi(argv[6]) : kDefaultTtl;
  const std::string region = argc > 7 ? argv[7] : "";

  auto channel =
      grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
  Route53Client client(channel);

  bool success = false;
  if (record_type == "A") {
    std::cout << "Updating A record for domain: " << domain_name
              << " to IP: " << new_value << '\n';
    success = client.UpdateARecord(hosted_zone_id, domain_name, new_value, ttl,
                                   region);
  } else if (record_type == "CNAME") {
    std::cout << "Updating CNAME record for domain: " << domain_name
              << " to: " << new_value << '\n';
    success = client.UpdateCNAMERecord(hosted_zone_id, domain_name, new_value,
                                       ttl, region);
  } else {
    std::cout << "Invalid record type. Use 'A' or 'CNAME'.\n";
    return 1;
  }

  std::cout << (success ? "DNS record update completed successfully.\n"
                        : "DNS record update failed.\n");
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
