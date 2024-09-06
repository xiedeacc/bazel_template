/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/common/garbage_collector.h"

#include "glog/logging.h"
#include "src/common/module.h"
#include "src/util/util.h"

using std::list;
using std::vector;

namespace bazel_template {
namespace common {

GarbageCollector::GarbageCollector() {
  stop_requested_.store(false);
  gc_list_ = new list<GCBase*>();
  running_gc_thread_count_.store(0);
  ResetGCCount();

  for (int i = 0; i < 4; i++) {
    CreateThread();
  }
}

GarbageCollector::~GarbageCollector() {
  stop_requested_.store(true);
  WaitGCThreadExit();

  GC();
  if (gc_list_ != nullptr) {
    delete gc_list_;
  }
}

void GarbageCollector::CreateThread() {
  if (stop_requested_.load() == true) {
    return;
  }

  int thread_count = running_gc_thread_count_.load();
  if (thread_count > 8) {
    LOG(WARNING) << "Too many thread count " << thread_count;
    return;
  }

  // create new gc thread if no one else have created
  if (running_gc_thread_count_.compare_exchange_strong(thread_count,
                                                       thread_count + 1) &&
      !stop_requested_.load()) {
    gc_threads_.emplace_back(
        std::thread(std::bind(&GarbageCollector::Run, this)));
  }
}

void GarbageCollector::WaitGCThreadExit() {
  stop_requested_.store(true);
  while (running_gc_thread_count_.load() > 0) {
    util::Util::Sleep(10);
  }
}

void GarbageCollector::Run() {
  do {
    int64_t start_time = util::Util::CurrentTimeMillis();
    GC();
    int64_t delta = 5 + start_time - util::Util::CurrentTimeMillis();
    if (delta > 0) {
      util::Util::Sleep(delta);
    } else if (delta < -100) {
      LOG(WARNING) << "GC Thread is too busy";
    }
  } while (!stop_requested_);
  LOG(WARNING) << "GC thread exit...";
  running_gc_thread_count_.fetch_sub(1);
}

void GarbageCollector::GC() {
  list<GCBase*>* cur_list = nullptr;
  list<GCBase*>* new_list = new list<GCBase*>();
  {
    absl::base_internal::SpinLockHolder locker(&lock_);
    cur_list = gc_list_;
    gc_list_ = new_list;
  }

  if (cur_list == nullptr) {
    return;
  }
  unsigned int size = cur_list->size();
  int64_t start_time = util::Util::CurrentTimeMillis();
  while (cur_list->size() > 0) {
    GCBase* vm = cur_list->front();
    if (vm != nullptr) {
      gc_count_.fetch_add(1);
      delete vm;
    }
    cur_list->pop_front();
  }

  delete cur_list;
  int64_t duration_time = util::Util::CurrentTimeMillis() - start_time;
  if (size > 100 || duration_time > 1000) {
    LOG(INFO) << "MeliGC. count=" << size << ", time=" << duration_time
              << ", threadCount=" << running_gc_thread_count_.load();
  }

  if (duration_time > 1000) {
    LOG(INFO) << "Try to create new gc thread";
    CreateThread();
  }
}

}  // namespace common
}  // namespace bazel_template

DEFINE_MODULE(GarbageCollector) {
  bazel_template::common::GarbageCollector::Instance();
  return true;
}
