/*******************************************************************************
 * Copyright (c) 2022 Copyright 2022- xiedeacc.com.
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <functional>
#include <memory>
#include <shared_mutex>
#include <string>
#include <thread>

// #include "src/async_grpc/server.h"
// #include "src/context/grpc_context.h"
#include "boost/thread/thread_pool.hpp"
#include "src/common/blocking_list.h"
#include "src/common/garbage_collector.h"
#include "src/context/server_run_info.h"
#include "src/proto/service.pb.h"
// #include "src/server/grpc_service_handlers.h"
#include "src/util/config_manager.h"
#include "src/util/util.h"

namespace bazel_template {
namespace server {

class GrpcServer final {
 public:
  GrpcServer(int port) : terminated(false), working_thread_in_pool(0) {
    server_run_info.reset(new context::ServerRunInfo());

    if (util::ConfigManager::Instance()->ThreadPoolSize() > 0) {
      working_thread_limit = util::ConfigManager::Instance()->ThreadPoolSize();
    }
    // LOG(INFO) << "working_thread_limit: " << working_thread_limit;
    thread_pool_.reset(
        new boost::executors::basic_thread_pool(working_thread_limit));

    auto task = std::bind(&GrpcServer::ScanList, this);
    thread_pool_->submit(task);
    server_run_info->MarkedServerInitedDone();

    //async_grpc::Server::Builder server_builder;
    //server_builder.SetServerAddress("127.0.0.1");
    //server_builder.SetNumGrpcThreads(10);
    //server_builder.SetNumEventThreads(10);
    //server_builder.RegisterHandler<GetSumHandler>();
    //server_builder.RegisterHandler<GetRunningSumHandler>();
    //server_ = server_builder.Build();
    //server_->SetExecutionContext(
        //async_grpc::common::make_unique<MathServerContext>());
    //server_->Start();
  }

 public:
  // void WaitForShutdown() { server_->WaitForShutdown(); }

  // grpc::Status ServerStatus(
  // const tbox::grpc_server::ServerStatusRequest *request,
  // tbox::grpc_server::ServerStatusResponse *response);

  // void DoServerStatus(const tbox::grpc_server::ServerStatusRequest *request,
  // tbox::grpc_server::ServerStatusResponse *response,
  // tbox::context::GrpcContext *grpc_context);

  void ScanList();
  // void RequestFastFail(tbox::context::GrpcContext *grpc_context);
  // void BuildDefaultResponse(
  // const tbox::grpc_server::ServerStatusRequest *request,
  // tbox::grpc_server::ServerStatusResponse *response);

 private:
  std::atomic<int64_t> next_log_info_time_;
  // common::BlockingList<context::GrpcContext *> request_pending_queue_;
  int64_t fast_fail_one_scope_limit_ = 10;
  std::shared_ptr<boost::executors::basic_thread_pool> thread_pool_;

 public:
  std::atomic_bool terminated;
  std::atomic<int64_t> working_thread_in_pool;
  absl::Mutex working_thread_mu_;
  int64_t working_thread_limit = 6;
  std::shared_ptr<context::ServerRunInfo> server_run_info;
  // std::unique_ptr<async_grpc::Server> server_;
};

}  // namespace server
}  // namespace bazel_template
