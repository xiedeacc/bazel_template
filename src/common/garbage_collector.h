/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_COMMON_GARBAGE_COLLECTOR_H
#define BAZEL_TEMPLATE_COMMON_GARBAGE_COLLECTOR_H

#include <atomic>
#include <list>
#include <memory>
#include <thread>
#include <vector>

#include "absl/base/internal/spinlock.h"
#include "folly/Singleton.h"

namespace bazel_template {
namespace common {

class GCBase {
 public:
  virtual ~GCBase() {}
};

class GarbageCollector {
  friend class folly::Singleton<GarbageCollector>;

 private:
  GarbageCollector();
  ~GarbageCollector();

 public:
  static std::shared_ptr<GarbageCollector> Instance() {
    return folly::Singleton<GarbageCollector>::try_get();
  }

  inline void AddToGc(GCBase* entry);

  void GC();
  void Run();

  // Use for test only!
 public:
  int64_t GetGCCount() { return gc_count_.load(); }

  void ResetGCCount() { gc_count_.store(0); }

 private:
  void WaitGCThreadExit();
  void CreateThread();

 private:
  std::list<GCBase*>* volatile gc_list_;

  std::atomic<bool> stop_requested_;
  std::vector<std::thread> gc_threads_;
  std::atomic<int> running_gc_thread_count_;
  mutable absl::base_internal::SpinLock lock_;

  // Use for test only
  std::atomic<int64_t> gc_count_;
};

typedef std::shared_ptr<GarbageCollector> GarbageCollectorPtr;

inline void GarbageCollector::AddToGc(GCBase* features) {
  absl::base_internal::SpinLockHolder locker(&lock_);
  gc_list_->push_back(features);
}

template <class T>
class GCEntry : public virtual GCBase {
 public:
  GCEntry() {
    this->value = nullptr;
    this->async_gc_ = false;
  }

  explicit GCEntry(T* value, bool async_gc = true) {
    this->value = value;
    this->async_gc_ = async_gc;
  }

  void SetValue(T* value, bool async_gc = true) {
    this->value = value;
    this->async_gc_ = async_gc;
  }

  virtual ~GCEntry() {
    if (value != nullptr) {
      if (async_gc_) {
        GCEntry* gc_entry = new GCEntry<T>(value, false);
        GarbageCollector::Instance()->AddToGc(gc_entry);
      } else {
        delete value;
      }
    }
  }

 public:
  T* value;
  bool async_gc_;
};

}  // namespace common
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_COMMON_GARBAGE_COLLECTOR_H
