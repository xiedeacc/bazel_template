/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/
#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "absl/synchronization/mutex.h"
#include "src/util/barrier.h"

namespace bazel_template {
namespace util {

template <typename T>
class SingletonBase {
  SingletonBase(const SingletonBase &) = delete;
  SingletonBase &operator=(const SingletonBase &) = delete;

 protected:
  SingletonBase() {}
  ~SingletonBase() {}

 public:
  static T &Instance() {
    if (!is_alive_) {
      fprintf(stderr, "Singleton has been destroyed\n");
      abort();
    }

    // double check
    if (!instance_) {
      absl::MutexLock lock(&mutex_);
      if (!instance_) {
        T *p = new T();
        MemoryWriteBarrier();
        instance_ = p;
        atexit(Destroy);
      }
    }

    return *instance_;
  }

  template <typename A1>
  static T &Instance(const A1 &a1) {
    if (!is_alive_) {
      fprintf(stderr, "Singleton has been destroyed\n");
      abort();
    }

    // double check
    if (!instance_) {
      absl::MutexLock lock(&mutex_);
      if (!instance_) {
        T *p = new T(a1);
        MemoryWriteBarrier();
        instance_ = p;
        atexit(Destroy);
      }
    }

    return *instance_;
  }

  static bool IsAlive() { return instance_ != nullptr; }

 private:
  static void Destroy() {
    is_alive_ = false;
    // execute before main exit, no lock required
    if (instance_) {
      delete instance_;
      instance_ = nullptr;
    }
  }

 private:
  static absl::Mutex mutex_;
  static T *volatile instance_;
  static bool volatile is_alive_;
};

template <typename T>
absl::Mutex SingletonBase<T>::mutex_;

template <typename T>
T *volatile SingletonBase<T>::instance_ = nullptr;

template <typename T>
bool volatile SingletonBase<T>::is_alive_ = true;

template <typename T>
class Singleton : public SingletonBase<T> {};

}  // namespace util
}  // namespace bazel_template
