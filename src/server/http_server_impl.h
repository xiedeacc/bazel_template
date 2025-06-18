/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_HTTP_SERVER_IMPL_H_
#define BAZEL_TEMPLATE_SERVER_HTTP_SERVER_IMPL_H_

#include <memory>
#include <string>

#include "proxygen/httpserver/HTTPServer.h"
#include "proxygen/httpserver/HTTPServerOptions.h"
#include "src/server/http_handler/http_handler_factory.h"
#include "src/server/server_context.h"
#include "src/util/config_manager.h"

namespace bazel_template {
namespace server {

class HttpServer final {
 public:
  HttpServer(std::shared_ptr<ServerContext> server_context)
      : server_context_(server_context) {
    proxygen::HTTPServerOptions options;
    options.threads = static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN));
    options.idleTimeout = std::chrono::milliseconds(60000);
    options.handlerFactories =
        proxygen::RequestHandlerChain()
            .addThen<bazel_template::server::http_handler::HTTPHandlerFactory>()
            .build();

    std::string addr = util::ConfigManager::Instance()->ServerAddr();
    int32_t port = util::ConfigManager::Instance()->HttpServerPort();

    proxygen::HTTPServer::IPConfig ip_config{
        folly::SocketAddress(addr, port, true),
        proxygen::HTTPServer::Protocol::HTTP};

    std::vector<proxygen::HTTPServer::IPConfig> IPs{ip_config};

    server_ = std::make_shared<proxygen::HTTPServer>(std::move(options));
    server_->bind(IPs);
  }

 public:
  void Start() {
    server_->start();
    server_context_->MarkedHttpServerInitedDone();
  }
  void Shutdown() { server_->stop(); }

 private:
  std::shared_ptr<proxygen::HTTPServer> server_;
  std::shared_ptr<ServerContext> server_context_;
};

}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_HTTP_SERVER_IMPL_H_
