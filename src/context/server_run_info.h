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
  ServerRunInfo() { is_inited_.store(false); }

  void MarkedServerInitedDone() { is_inited_.store(true); }

  bool IsInitYet() { return is_inited_.load(); }

 private:
  std::atomic_bool is_inited_;
  std::string git_commit_;
};

}  // namespace context
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_CONTEXT_SERVER_RUN_INFO_H
