/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/server/service_impl.h"

#include "bvar/bvar.h"

using brpc::ClosureGuard;
using bvar::Adder;
using google::protobuf::Closure;
using google::protobuf::RpcController;
using google::protobuf::util::JsonOptions;
using std::map;
using std::string;
using std::thread;
using std::vector;

using namespace bazel_template::proto;
using namespace bazel_template::util;

namespace bazel_template {
namespace server {

std::atomic<bool> ServiceImpl::terminated(false);

bool ServiceImpl::Init() {
  shutdown_task_ = thread(std::bind(&ServiceImpl::ShutdownTask, this));
  LOG(WARNING) << "ServiceImpl Init success";
  return true;
}

void ServiceImpl::Handle(RpcController *cntl_base, const Request *req,
                         Response *res, Closure *done) {
  ClosureGuard done_guard(done);
  brpc::Controller *cntl = static_cast<brpc::Controller *>(cntl_base);
  if (!DoHandle(*req, res)) {
    BuildDefaultResponse(req, res, cntl);
  }
}

void ServiceImpl::HttpHandle(RpcController *cntl_base,
                             const HttpRequest *request, HttpResponse *response,
                             Closure *done) {
  ClosureGuard done_guard(done);
  brpc::Controller *cntl = static_cast<brpc::Controller *>(cntl_base);
  if (!cntl->has_http_request()) {
    LOG(ERROR) << "Not a http request";
    return;
  }

  cntl->http_response().set_content_type("application/octet-stream");

  Request req;
  Response res;
  auto &req_attachment = cntl->request_attachment();
  auto &res_attachment = cntl->response_attachment();
  if (!req.ParseFromString(req_attachment.to_string())) {
    LOG(ERROR) << "parse request error";
    BuildHttpDefaultResponse(&req, &res, cntl);
    return;
  }
  if (!DoHandle(req, &res)) {
    BuildHttpDefaultResponse(&req, &res, cntl);
    return;
  }

  butil::IOBufAsZeroCopyOutputStream wrapper(&res_attachment);
  if (!res.SerializeToZeroCopyStream(&wrapper)) {
    LOG(ERROR) << "SerializeToZeroCopyStream error";
  }
}

bool ServiceImpl::DoHandle(const Request &req, Response *res) {
  res->set_request_id(req.request_id());
  res->set_msg("Hello, this is ");
  res->mutable_msg()->append("");
  return true;
}

}  // namespace server
}  // namespace bazel_template
