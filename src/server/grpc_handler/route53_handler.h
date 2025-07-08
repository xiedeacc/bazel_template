/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_GRPC_HANDLER_ROUTE53_HANDLER_H_
#define BAZEL_TEMPLATE_SERVER_GRPC_HANDLER_ROUTE53_HANDLER_H_

#include <memory>
#include <string>

#include "aws/core/Aws.h"
#include "aws/route53/Route53Client.h"
#include "aws/route53/model/ChangeResourceRecordSetsRequest.h"
#include "aws/route53/model/ResourceRecordSet.h"
#include "aws/route53/model/Change.h"
#include "glog/logging.h"
#include "src/async_grpc/rpc_handler.h"
#include "src/server/grpc_handler/meta.h"

namespace bazel_template {
namespace server {
namespace grpc_handler {

class Route53ManagementHandler : public async_grpc::RpcHandler<Route53ManagementMethod> {
 public:
  Route53ManagementHandler() {
    // Initialize AWS SDK
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    aws_initialized_ = true;
  }

  ~Route53ManagementHandler() {
    // Cleanup AWS SDK if we initialized it
    if (aws_initialized_) {
      Aws::SDKOptions options;
      Aws::ShutdownAPI(options);
    }
  }

  void OnRequest(const proto::Route53Request& req) override {
    auto res = std::make_unique<proto::Route53Response>();
    res->set_domain_name(req.domain_name());
    res->set_record_type(req.record_type());
    res->set_new_value(req.new_value());

    LOG(INFO) << "Route53 management request: " << req.op()
              << " for domain: " << req.domain_name()
              << " type: " << req.record_type()
              << " value: " << req.new_value();

    try {
      // Determine the operation based on the request op code
      switch (req.op()) {
        case proto::OpCode::OP_ROUTE53_UPDATE_A_RECORD:
          HandleUpdateARecord(req, res.get());
          break;
        case proto::OpCode::OP_ROUTE53_UPDATE_CNAME_RECORD:
          HandleUpdateCNAMERecord(req, res.get());
          break;
        default:
          res->set_err_code(proto::ErrCode::FAIL);
          res->set_message("Invalid operation code for Route53 management");
          LOG(ERROR) << "Invalid operation code: " << req.op();
          break;
      }
    } catch (const std::exception& e) {
      LOG(ERROR) << "Route53 management operation failed: " << e.what();
      res->set_err_code(proto::ErrCode::FAIL);
      res->set_message(std::string("Operation failed: ") + e.what());
    }

    Send(std::move(res));
  }

  void OnReadsDone() override { Finish(grpc::Status::OK); }

 private:
  void HandleUpdateARecord(const proto::Route53Request& req,
                          proto::Route53Response* res) {
    Aws::Route53::Route53Client route53_client;

    // Set region if specified
    if (!req.region().empty()) {
      // route53_client = Aws::Route53::Route53Client(req.region());
    }

    // Create the resource record set
    Aws::Route53::Model::ResourceRecordSet record_set;
    record_set.SetName(req.domain_name());
    record_set.SetType(Aws::Route53::Model::RRType::A);
    record_set.SetTTL(req.ttl() > 0 ? req.ttl() : 300);

    // Add the new IP address
    Aws::Route53::Model::ResourceRecord record;
    record.SetValue(req.new_value());
    record_set.AddResourceRecords(record);

    // Create the change
    Aws::Route53::Model::Change change;
    change.SetAction(Aws::Route53::Model::ChangeAction::UPSERT);
    change.SetResourceRecordSet(record_set);

    // Create the change request
    Aws::Route53::Model::ChangeResourceRecordSetsRequest change_request;
    change_request.SetHostedZoneId(req.hosted_zone_id());
    // change_request.AddChanges(change);

    auto outcome = route53_client.ChangeResourceRecordSets(change_request);

    if (outcome.IsSuccess()) {
      res->set_err_code(proto::ErrCode::SUCCESS);
      res->set_change_id(outcome.GetResult().GetChangeInfo().GetId());
      res->set_message("A record updated successfully");
      LOG(INFO) << "Successfully updated A record for domain: " << req.domain_name()
                << " to IP: " << req.new_value();
    } else {
      res->set_err_code(proto::ErrCode::FAIL);
      res->set_message("Failed to update A record: " +
                      outcome.GetError().GetMessage());
      LOG(ERROR) << "Failed to update A record for domain: " << req.domain_name()
                 << " - " << outcome.GetError().GetMessage();
    }
  }

  void HandleUpdateCNAMERecord(const proto::Route53Request& req,
                              proto::Route53Response* res) {
    Aws::Route53::Route53Client route53_client;

    // Set region if specified
    if (!req.region().empty()) {
      // route53_client = Aws::Route53::Route53Client(req.region());
    }

    // Create the resource record set
    Aws::Route53::Model::ResourceRecordSet record_set;
    record_set.SetName(req.domain_name());
    record_set.SetType(Aws::Route53::Model::RRType::CNAME);
    record_set.SetTTL(req.ttl() > 0 ? req.ttl() : 300);

    // Add the new domain name
    Aws::Route53::Model::ResourceRecord record;
    record.SetValue(req.new_value());
    record_set.AddResourceRecords(record);

    // Create the change
    Aws::Route53::Model::Change change;
    change.SetAction(Aws::Route53::Model::ChangeAction::UPSERT);
    change.SetResourceRecordSet(record_set);

    // Create the change request
    Aws::Route53::Model::ChangeResourceRecordSetsRequest change_request;
    change_request.SetHostedZoneId(req.hosted_zone_id());
    // change_request.AddChanges(change);

    auto outcome = route53_client.ChangeResourceRecordSets(change_request);

    if (outcome.IsSuccess()) {
      res->set_err_code(proto::ErrCode::SUCCESS);
      res->set_change_id(outcome.GetResult().GetChangeInfo().GetId());
      res->set_message("CNAME record updated successfully");
      LOG(INFO) << "Successfully updated CNAME record for domain: " << req.domain_name()
                << " to: " << req.new_value();
    } else {
      res->set_err_code(proto::ErrCode::FAIL);
      res->set_message("Failed to update CNAME record: " +
                      outcome.GetError().GetMessage());
      LOG(ERROR) << "Failed to update CNAME record for domain: " << req.domain_name()
                 << " - " << outcome.GetError().GetMessage();
    }
  }

  bool aws_initialized_ = false;
};

}  // namespace grpc_handler
}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_GRPC_HANDLER_ROUTE53_HANDLER_H_
