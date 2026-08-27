/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_HTTP_HANDLER_FACTORY_H_
#define BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_HTTP_HANDLER_FACTORY_H_

#include "proxygen/httpserver/RequestHandler.h"
#include "proxygen/httpserver/RequestHandlerFactory.h"
#include "src/server/http_handler/websocket_upgrade_handler.h"

namespace bazel_template::server::http_handler {

class HTTPHandlerFactory : public proxygen::RequestHandlerFactory {
 public:
  void onServerStart(folly::EventBase* /*evb*/) noexcept override {}

  void onServerStop() noexcept override {
    // proxygen declares this noexcept; logging allocates and can throw.
    try {
      LOG(INFO) << "HTTP server stopped";
    } catch (...) {  // NOLINT(bugprone-empty-catch)
    }
  }

  proxygen::RequestHandler* onRequest(
      proxygen::RequestHandler* /*unused*/,
      proxygen::HTTPMessage* msg) noexcept override {
    // proxygen declares this noexcept, but reading the headers and allocating
    // the handler can both throw. Refusing the request is better than
    // terminating the server.
    try {
      const std::string& path = msg->getPath();
      const std::string& method = msg->getMethodString();

      if (method == "GET" && path == "/api/v1/folder/load") {
        return new WebSocketUpgradeHandler();
      }
    } catch (const std::exception& e) {
      LOG(ERROR) << "Failed to route request: " << e.what();
    } catch (...) {
      LOG(ERROR) << "Failed to route request";
    }

    return nullptr;
  }
};

}  // namespace bazel_template::server::http_handler

#endif  // BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_HTTP_HANDLER_FACTORY_H_
