/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_GRPC_SERVER_IMPL_H_
#define BAZEL_TEMPLATE_SERVER_GRPC_SERVER_IMPL_H_

#include <memory>
#include <string>

#include "absl/strings/str_cat.h"
#include "src/async_grpc/server.h"
#include "src/server/grpc_handler/account_state_handler.h"
#include "src/server/grpc_handler/file_handler.h"
#include "src/server/grpc_handler/load_folder_handler.h"
#include "src/server/server_context.h"
#include "src/util/config_manager.h"

namespace bazel_template {
namespace server {

class GrpcServer final {
 public:
  GrpcServer(std::shared_ptr<ServerContext> server_context)
      : terminated(false) {
    async_grpc::Server::Builder server_builder;
    std::string addr_port =
        util::ConfigManager::Instance()->ServerAddr() + ":" +
        absl::StrCat(util::ConfigManager::Instance()->GrpcServerPort());
    server_builder.SetServerAddress(addr_port);
    server_builder.SetNumGrpcThreads(
        util::ConfigManager::Instance()->GrpcThreads());
    server_builder.SetNumEventThreads(
        util::ConfigManager::Instance()->EventThreads());

    // Register handlers
    server_builder.RegisterHandler<
        bazel_template::server::grpc_handler::LoadFolderHandler>();

    server_ = server_builder.Build();
    server_->SetExecutionContext(server_context);
  }

 public:
  void Start() {
    server_->Start();
    server_->GetContext<ServerContext>()->MarkedGrpcServerInitedDone();
  }

  void Shutdown() { server_->Shutdown(); }
  void WaitForShutdown() { server_->WaitForShutdown(); }

 private:
  std::unique_ptr<async_grpc::Server> server_;

 public:
  std::atomic_bool terminated;
};

}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_GRPC_SERVER_IMPL_H_
