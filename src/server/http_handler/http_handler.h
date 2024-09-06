/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_HTTP_HANDLER_H
#define BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_HTTP_HANDLER_H

#include <memory>

#include "proxygen/httpserver/RequestHandler.h"
#include "proxygen/httpserver/RequestHandlerFactory.h"

namespace bazel_template {
namespace server {
namespace http_handler {

class HTTPHandler : public proxygen::RequestHandler {
 public:
  void onRequest(
      std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;
  void onEOM() noexcept override;
  void onUpgrade(proxygen::UpgradeProtocol protocol) noexcept override;
  void requestComplete() noexcept override;
  void onError(proxygen::ProxygenError err) noexcept override;
};

class HTTPHandlerFactory : public proxygen::RequestHandlerFactory {
 public:
  void onServerStart(folly::EventBase* evb) noexcept override;
  void onServerStop() noexcept override;
  proxygen::RequestHandler* onRequest(
      proxygen::RequestHandler* handler,
      proxygen::HTTPMessage* msg) noexcept override;
};

}  // namespace http_handler
}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_HTTP_HANDLER_H
