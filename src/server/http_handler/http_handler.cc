/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/server/http_handler/http_handler.h"

#include "proxygen/httpserver/ResponseBuilder.h"

namespace bazel_template {
namespace server {
namespace http_handler {

void HTTPHandler::onRequest(
    std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
  // Handle request headers
}

void HTTPHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
  // Handle request body
}

void HTTPHandler::onEOM() noexcept {
  // End of message
  proxygen::ResponseBuilder(downstream_)
      .status(200, "OK")
      .body("<html><body><h1>Hello, Proxygen!</h1></body></html>")
      .sendWithEOM();
}

void HTTPHandler::onUpgrade(proxygen::UpgradeProtocol protocol) noexcept {
  // Handle protocol upgrade
}

void HTTPHandler::requestComplete() noexcept { delete this; }

void HTTPHandler::onError(proxygen::ProxygenError err) noexcept { delete this; }

void HTTPHandlerFactory::onServerStart(folly::EventBase* evb) noexcept {
  // Server start actions
}

void HTTPHandlerFactory::onServerStop() noexcept {
  // Server stop actions
}

proxygen::RequestHandler* HTTPHandlerFactory::onRequest(
    proxygen::RequestHandler* handler, proxygen::HTTPMessage* msg) noexcept {
  return new HTTPHandler();
}

}  // namespace http_handler
}  // namespace server
}  // namespace bazel_template
