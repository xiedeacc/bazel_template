/*******************************************************************************
 * Copyright (c) 2022 Copyright 2022- xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <boost/thread/future.hpp>
#include <functional>
#include <shared_mutex>
#include <string>
#include <thread>

#include "bazel_template/bazel_template_service.pb.h"
#include "call_tags/ip_info_call.h"
#include "grpcpp/grpcpp.h"
#include "src/context/grpc_context.h"
#include "src/context/server_run_info.h"
#include "src/server/call_tags/call_tags.h"
#include "src/server/server_base.h"
#include "src/util/blocking_list.h"
#include "src/util/common_util.h"
#include "src/util/config_util.h"
#include "src/util/garbage_collector.h"

namespace bazel_template {
namespace server {

class GrpcServer final : public GrpcServerBase {
 public:
  GrpcServer(int port)
      : GrpcServerBase(port), terminated(false), working_thread_in_pool(0) {
    server_run_info.reset(new context::ServerRunInfo());

    if (util::ConfigUtil::Instance()->ThreadPoolSize() > 0) {
      working_thread_limit = bazel_template_config->work_thread_pool_size;
    }
    // LOG(INFO) << "working_thread_limit: " << working_thread_limit;
    thread_pool_.reset(new boost::basic_thread_pool(working_thread_limit));

    auto task = std::bind(&GrpcServer::ScanList, this);
    thread_pool_->submit(task);
    server_run_info->MarkedServerInitedDone();
  };

  virtual void OnRun(grpc::ServerCompletionQueue *request_queue,
                     grpc::ServerCompletionQueue *response_queue) override {
    new server::call_tags::IpInfoCall(this, request_queue, response_queue);
    new server::call_tags::ServerStatusCall(this, request_queue,
                                            response_queue);
  };

  virtual void OnExit() override {};

  virtual void OnServerStatusRead(
      const server::ServerStatusRequest *request,
      server::ServerStatusResponse *response) override {
    LOG(INFO) << "OnServerStatusRead";
    LOG(INFO) << "request: \n"
              << util::CommonUtil::MessageToJsonString(*request);
    grpc::Status status = ServerStatus(request, response);
  }

  grpc::Status ServerStatus(const server::ServerStatusRequest *request,
                            server::ServerStatusResponse *response);

  void DoServerStatus(const server::ServerStatusRequest *request,
                      server::ServerStatusResponse *response,
                      context::GrpcContext *grpc_context);

  virtual void OnIpInfoRead(const server::IpInfoRequest *request,
                            server::IpInfoResponse *response) override {
    LOG(INFO) << "OnIpInfoRead";
    LOG(INFO) << "request: \n"
              << util::CommonUtil::MessageToJsonString(*request);
    server::IpInfo *ip_info = response->add_ip_infos();
    ip_info->set_update_timestamp(absl::ToUnixNanos(absl::Now()));
    ip_info->set_location("aws");
    ip_info->set_ip_addr("192.168.1.1");
    ip_info->set_mac_addr("xxxxxx");
    ip_info->set_ip_addr_v6("xxxxxx");
  }

  void ScanList();
  void RequestFastFail(context::GrpcContext *grpc_context);
  void BuildDefaultResponse(const server::ServerStatusRequest *request,
                            server::ServerStatusResponse *response);

 private:
  std::atomic<int64_t> next_log_info_time_;
  common::BlockingList<context::GrpcContext *> request_pending_queue_;
  int64_t fast_fail_one_scope_limit_ = 10;
  std::shared_ptr<boost::executors::basic_thread_pool> thread_pool_;

 public:
  std::atomic_bool terminated;
  std::atomic<int64_t> working_thread_in_pool;
  absl::Mutex working_thread_mu_;
  int64_t working_thread_limit = 6;
  std::shared_ptr<context::ServerRunInfo> server_run_info;
};

}  // namespace server
}  // namespace bazel_template
