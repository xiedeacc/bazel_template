/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

module;

#include <exception>
#include <memory>
#include <string>

#include "proxygen/httpserver/RequestHandler.h"
#include "proxygen/httpserver/RequestHandlerFactory.h"
#include "proxygen/lib/http/HTTPMessage.h"
#include "src/common/logging.h"
#include "src/server/http_handler/websocket_upgrade_handler.h"

module bazel_template.server.http_handler;

namespace bazel_template::server::http_handler {

/// @brief Factory for creating HTTP request handlers based on URI path.
class HTTPHandlerFactory : public proxygen::RequestHandlerFactory {
 public:
  void onServerStart(folly::EventBase* /*evb*/) noexcept override {}

  void onServerStop() noexcept override {
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
      ::bazel_template::logging::ReportException("Failed to route request",
                                                 e.what());
    } catch (...) {
      ::bazel_template::logging::ReportException("Failed to route request",
                                                 nullptr);
    }
    return nullptr;
  }
};

std::unique_ptr<proxygen::RequestHandlerFactory> CreateHandlerFactory() {
  return std::make_unique<HTTPHandlerFactory>();
}

}  // namespace bazel_template::server::http_handler
