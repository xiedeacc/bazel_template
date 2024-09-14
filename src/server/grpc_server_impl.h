/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <memory>
#include <string>

#include "absl/strings/str_cat.h"
#include "src/async_grpc/server.h"
#include "src/server/grpc_handler/get_runing_sum_handler.h"
#include "src/server/grpc_handler/get_sum_handler.h"
#include "src/server/server_context.h"
#include "src/util/config_manager.h"

namespace bazel_template {
namespace server {

class GrpcServer final {
 public:
  GrpcServer() : terminated(false) {
    async_grpc::Server::Builder server_builder;
    std::string addr_port =
        util::ConfigManager::Instance()->ServerAddr() + ":" +
        absl::StrCat(util::ConfigManager::Instance()->GrpcServerPort());
    server_builder.SetServerAddress(addr_port);
    server_builder.SetNumGrpcThreads(
        util::ConfigManager::Instance()->GrpcThreads());
    server_builder.SetNumEventThreads(
        util::ConfigManager::Instance()->EventThreads());

    server_builder
        .RegisterHandler<bazel_template::server::grpc_handler::GetSumHandler>();
    server_builder.RegisterHandler<
        bazel_template::server::grpc_handler::GetRunningSumHandler>();
    server_ = server_builder.Build();
    server_->SetExecutionContext(std::make_unique<ServerContext>());
    server_->Start();

    server_->GetContext<ServerContext>()->MarkedServerInitedDone();
  }

 public:
  void WaitForShutdown() { server_->WaitForShutdown(); }

 private:
  std::unique_ptr<async_grpc::Server> server_;
  std::atomic<int64_t> next_log_info_time_;

 public:
  std::atomic_bool terminated;
};

}  // namespace server
}  // namespace bazel_template
