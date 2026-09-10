/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

module;

#include <memory>
#include <string>

#include "glog/logging.h"
#include "proxygen/httpserver/RequestHandler.h"
#include "proxygen/httpserver/RequestHandlerFactory.h"
#include "proxygen/lib/http/HTTPMessage.h"

module bazel_template.server.http_handler;

namespace bazel_template::server::http_handler {

/// @brief Factory for creating HTTP request handlers based on URI path.
/// The template ships no handlers; add them here and return one per path.
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
      proxygen::HTTPMessage* /*msg*/) noexcept override {
    return nullptr;
  }
};

std::unique_ptr<proxygen::RequestHandlerFactory> CreateHandlerFactory() {
  return std::make_unique<HTTPHandlerFactory>();
}

}  // namespace bazel_template::server::http_handler
