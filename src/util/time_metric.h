/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/
#pragma once

#include "glog/logging.h"
#include "src/util/util.h"

namespace bazel_template {
namespace common {

class TimeMetric {
 public:
  TimeMetric(const std::string qid)
      : qid_(qid),
        begin_time_(util::Util::CurrentTimeMillis()),
        start_time_(begin_time_),
        end_time_(begin_time_) {
    if (begin_time_ >= next_metric_time.load()) {
      next_metric_time.store(begin_time_ + 3000);
      print_log_ = true;
    }
  }

  ~TimeMetric() {}

  int64_t Reset(const std::string &addtional_log = "", bool print = false) {
    end_time_ = static_cast<uint64_t>(util::Util::CurrentTimeMillis());
    int64_t cost = end_time_ - start_time_;
    start_time_ = end_time_;
    if (print || print_log_) {
      LOG(INFO) << qid_ << " " << addtional_log << " cost: " << cost;
    }
    return cost;
  }

  int64_t TotalCost(const std::string &addtional_log = "", bool print = false) {
    end_time_ = static_cast<uint64_t>(util::Util::CurrentTimeMillis());
    int64_t cost = end_time_ - begin_time_;
    if (print || print_log_) {
      LOG(INFO) << qid_ << " " << addtional_log << " total cost: " << cost;
    }
    return cost;
  }

 public:
  static std::atomic<uint64_t> next_metric_time;

 private:
  const std::string qid_;
  uint64_t begin_time_;
  uint64_t start_time_;
  uint64_t end_time_;
  bool print_log_ = false;
};

}  // namespace common
}  // namespace bazel_template
