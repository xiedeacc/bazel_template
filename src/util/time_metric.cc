/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/util/time_metric.h"

namespace bazel_template {
namespace common {

std::atomic<uint64_t> TimeMetric::next_metric_time = 0;

}  // namespace common
}  // namespace bazel_template
