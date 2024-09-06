/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/context/grpc_context.h"

namespace bazel_template {
namespace context {

std::atomic<int64_t> GrpcContext::next_metric_time(0);

}  // namespace context
}  // namespace bazel_template
