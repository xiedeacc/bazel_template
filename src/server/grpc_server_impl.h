/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <memory>
#include <string>

#include "src/async_grpc/server.h"
// #include "src/context/grpc_context.h"
// #include "boost/thread/thread_pool.hpp"
#include "boost/thread/executors/basic_thread_pool.hpp"
#include "src/common/blocking_list.h"
#include "src/common/garbage_collector.h"
#include "src/context/server_run_info.h"
#include "src/proto/service.pb.h"
#include "src/server/grpc_service_handlers.h"
#include "src/util/config_manager.h"

namespace bazel_template {
namespace server {

class GrpcServer final {
 public:
  GrpcServer(int port) : terminated(false), working_thread_in_pool(0) {
    server_run_info.reset(new context::ServerRunInfo());

    if (util::ConfigManager::Instance()->ThreadPoolSize() > 0) {
      thread_pool_size = util::ConfigManager::Instance()->ThreadPoolSize();
    }
    LOG(INFO) << "thread_pool_size: " << thread_pool_size;
    thread_pool_.reset(
        new boost::executors::basic_thread_pool(thread_pool_size));

    //auto task = std::bind(&GrpcServer::ScanList, this);
    //thread_pool_->submit(task);

    async_grpc::Server::Builder server_builder;
    server_builder.SetServerAddress("127.0.0.1");
    server_builder.SetNumGrpcThreads(10);
    server_builder.SetNumEventThreads(10);
    server_builder.RegisterHandler<GetSumHandler>();
    server_builder.RegisterHandler<GetRunningSumHandler>();
    server_ = server_builder.Build();
    server_->SetExecutionContext(
        std::make_unique<MathServerContext>());
    server_->Start();

    server_run_info->MarkedServerInitedDone();
  }

 public:
  void WaitForShutdown() { server_->WaitForShutdown(); }

  void ScanList();

 private:
  std::atomic<int64_t> next_log_info_time_;
  // common::BlockingList<context::GrpcContext *> request_pending_queue_;
  int64_t fast_fail_one_scope_limit_ = 10;
  std::shared_ptr<boost::executors::basic_thread_pool> thread_pool_;

 public:
  std::atomic_bool terminated;
  std::atomic<int64_t> working_thread_in_pool;
  absl::Mutex working_thread_mu_;
  int64_t thread_pool_size = 2;
  std::shared_ptr<context::ServerRunInfo> server_run_info;
  std::unique_ptr<async_grpc::Server> server_;
};

}  // namespace server
}  // namespace bazel_template
