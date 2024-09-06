/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_CONTEXT_SERVER_RUN_INFO_H
#define BAZEL_TEMPLATE_CONTEXT_SERVER_RUN_INFO_H

#include <atomic>

namespace bazel_template {
namespace context {

class ServerRunInfo {
 public:
  ServerRunInfo() { is_inited.store(false); }

  void MarkedServerInitedDone() { is_inited.store(true); }

  bool IsInitYet() { return is_inited.load(); }

 private:
  std::atomic_bool is_inited;
};

}  // namespace context
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_CONTEXT_SERVER_RUN_INFO_H
