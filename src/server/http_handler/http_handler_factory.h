/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_HTTP_HANDLER_FACTORY_H_
#define BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_HTTP_HANDLER_FACTORY_H_

#include "proxygen/httpserver/RequestHandler.h"
#include "proxygen/httpserver/RequestHandlerFactory.h"
#include "src/server/http_handler/load_folder_handler.h"

namespace bazel_template {
namespace server {
namespace http_handler {

class HTTPHandlerFactory : public proxygen::RequestHandlerFactory {
 public:
  void onServerStart(folly::EventBase*) noexcept override {}

  void onServerStop() noexcept override { LOG(INFO) << "HTTP server stopped"; }

  proxygen::RequestHandler* onRequest(
      proxygen::RequestHandler*, proxygen::HTTPMessage* msg) noexcept override {
    const std::string& path = msg->getPath();
    const std::string& method = msg->getMethodString();

    if (method == "GET" && path == "/api/v1/folder/load") {
      return new LoadFolderHandler();
    }

    return nullptr;
  }
};

}  // namespace http_handler
}  // namespace server
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_SERVER_HTTP_HANDLER_HTTP_HANDLER_FACTORY_H_
