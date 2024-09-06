/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_CONTEXT_GRPC_CONTEXT_H
#define BAZEL_TEMPLATE_CONTEXT_GRPC_CONTEXT_H

#include <atomic>
#include <string>
#include <vector>

#include "absl/synchronization/blocking_counter.h"
#include "src/proto/service.grpc.pb.h"
#include "src/proto/service.pb.h"
#include "src/util/config_manager.h"
#include "src/util/util.h"

namespace bazel_template {
namespace context {

class GrpcContext {
 public:
  GrpcContext(const proto::Request* request, proto::Response* response,
              size_t batch_size, bool build_context = true)
      : created_time(util::Util::CurrentTimeMillis()),
        request(request),
        response(response),
        blocking_counter(batch_size) {
    is_fast_fail = false;
    task = nullptr;
    if (build_context) {
      BuildContext(request);
    }
  }

 public:
  virtual ~GrpcContext() {}

  void BuildContext(const proto::Request* request) {
    do_metric = util::Util::Random(0, 10000) <
                util::ConfigManager::Instance()->MetricRatio();
    do_metric = do_metric || created_time > next_metric_time.load();
    if (do_metric) {
      next_metric_time.store(
          created_time +
          util::ConfigManager::Instance()->MetricIntervalSec() * 1000);
    }

    int64_t rand_10000 = 10000;
    if (!request->request_id().empty()) {
      int64_t hash_code = util::Util::Hash64(request->request_id());
      if (hash_code < 0) {
        hash_code = -hash_code;
      }
      rand_10000 = hash_code % 10000;
    }

    if (rand_10000 < 10000 &&
        rand_10000 <= util::ConfigManager::Instance()->DiscardRatio()) {
      is_skip = true;
    }
  }

 public:
  int64_t created_time;
  const proto::Request* request;
  proto::Response* response;
  absl::BlockingCounter blocking_counter;

  bool do_metric;
  bool is_fast_fail;
  bool is_skip;
  std::function<void()> task;
  static std::atomic<int64_t> next_metric_time;
};

class ScopeDecrementContextCounter {
 public:
  explicit ScopeDecrementContextCounter(GrpcContext* grpc_context)
      : grpc_context(grpc_context) {}
  ~ScopeDecrementContextCounter() {
    grpc_context->blocking_counter.DecrementCount();
  }

 private:
  GrpcContext* grpc_context;
};

}  // namespace context
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_CONTEXT_GRPC_CONTEXT_H
