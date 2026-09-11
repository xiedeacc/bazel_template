/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

module;

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include "proxygen/httpserver/HTTPServer.h"
#include "proxygen/httpserver/HTTPServerOptions.h"
#include "proxygen/httpserver/RequestHandlerFactory.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

module bazel_template.server.http_server;

import bazel_template.server.http_handler;
import bazel_template.util.config_manager;

namespace bazel_template::server {

HttpServer::HttpServer(std::shared_ptr<ServerContext> server_context)
    : server_context_(std::move(server_context)) {
  proxygen::HTTPServerOptions options;
  options.threads = static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN));
  options.idleTimeout = std::chrono::seconds(60);
  options.handlerFactories = proxygen::RequestHandlerChain()
                                 .addThen(http_handler::CreateHandlerFactory())
                                 .build();

  std::string addr = util::ConfigManager::Instance()->ServerAddr();
  auto port =
      static_cast<int32_t>(util::ConfigManager::Instance()->HttpServerPort());
  proxygen::HTTPServer::IPConfig ip_config{
      folly::SocketAddress(addr, port, true),
      proxygen::HTTPServer::Protocol::HTTP};
  std::vector<proxygen::HTTPServer::IPConfig> IPs{ip_config};
  server_ = std::make_shared<proxygen::HTTPServer>(std::move(options));
  server_->bind(IPs);
}

HttpServer::~HttpServer() = default;

void HttpServer::Start() {
  server_->start();
  ServerContext::MarkedHttpServerInitedDone();
}

void HttpServer::Shutdown() {
  server_->stop();
}

}  // namespace bazel_template::server
