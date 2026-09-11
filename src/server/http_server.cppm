/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// bazel_template.server.http_server: the proxygen HTTP server. proxygen is
// only forward-declared; http_server.cc has the headers.
module;

#include <memory>

namespace proxygen {
class HTTPServer;
}  // namespace proxygen

export module bazel_template.server.http_server;

import bazel_template.server.context;

export namespace bazel_template::server {

class HttpServer final {
 public:
  explicit HttpServer(std::shared_ptr<ServerContext> server_context);
  ~HttpServer();

  HttpServer(const HttpServer&) = delete;
  HttpServer& operator=(const HttpServer&) = delete;
  HttpServer(HttpServer&&) = delete;
  HttpServer& operator=(HttpServer&&) = delete;

  void Start();
  void Shutdown();

 private:
  std::shared_ptr<proxygen::HTTPServer> server_;
  std::shared_ptr<ServerContext> server_context_;
};

}  // namespace bazel_template::server
