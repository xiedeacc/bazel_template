/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "proxygen/httpserver/HTTPServer.h"
#include "proxygen/httpserver/HTTPServerOptions.h"
#include "src/server/http_handler/http_handler.h"

int main(int argc, char* argv[]) {
  proxygen::HTTPServerOptions options;
  options.threads = static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN));
  options.idleTimeout = std::chrono::milliseconds(60000);
  options.handlerFactories =
      proxygen::RequestHandlerChain()
          .addThen<bazel_template::server::http_handler::HTTPHandlerFactory>()
          .build();

  std::vector<proxygen::HTTPServer::IPConfig> IPs = {
      {folly::SocketAddress("0.0.0.0", 8080, true),
       proxygen::HTTPServer::Protocol::HTTP}};

  proxygen::HTTPServer server(std::move(options));
  server.bind(IPs);

  std::thread t([&]() { server.start(); });

  t.join();

  return 0;
}
