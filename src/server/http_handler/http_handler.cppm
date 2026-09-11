/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// bazel_template.server.http_handler: the HTTP request handler factory.
// proxygen (and through it folly) is only forward-declared here: a BMI that
// carries folly's headers cannot be merged by gcc. The factory class is an
// implementation unit reached through CreateHandlerFactory().
module;

#include <memory>

namespace proxygen {
class RequestHandlerFactory;
}  // namespace proxygen

export module bazel_template.server.http_handler;

export namespace bazel_template::server::http_handler {

/// @brief The factory the HTTP server chains.
[[nodiscard]] std::unique_ptr<proxygen::RequestHandlerFactory>
CreateHandlerFactory();

}  // namespace bazel_template::server::http_handler
