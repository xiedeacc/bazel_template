/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_COMMON_BLOCKING_LIST_H
#define BAZEL_TEMPLATE_COMMON_BLOCKING_LIST_H

#include <list>
#include <memory>
#include <mutex>
#include <string>

namespace bazel_template {
namespace common {

template <typename T>
class BlockingList {
 public:
  BlockingList();

  void push_back(const T& t);

  void push_front(const T& t);

  bool try_pop_front(T* t);

  bool try_pop_back(T* t);

  bool try_peek_front(T* t);

  bool try_peek_back(T* t);

  size_t size() const;

  void wait_until_not_empty();

  T pop_back();

  T pop_front();

  T peek_front_unsafe();

 protected:
  /**
   Move synchronization fields out instead of including boost/thread.hpp
   to avoid a boost/NVCC issues (#1009, #1010) on OSX. Also fails on
   Linux CUDA 7.0.18.
   */
  class sync;

  std::list<T> list_;
  std::shared_ptr<sync> sync_;

 private:
  BlockingList(const BlockingList&);
  BlockingList& operator=(const BlockingList&);
  int64_t size_;
};

template <typename T>
class BlockingList<T>::sync {
 public:
  mutable std::mutex mutex_;
  std::condition_variable condition_;
};

template <typename T>
BlockingList<T>::BlockingList() : sync_(new sync()), size_(0) {}

template <typename T>
void BlockingList<T>::push_front(const T& t) {
  std::mutex::scoped_lock lock(sync_->mutex_);
  list_.push_front(t);
  ++size_;
  sync_->condition_.notify_one();
}

template <typename T>
void BlockingList<T>::push_back(const T& t) {
  std::mutex::scoped_lock lock(sync_->mutex_);
  list_.push_back(t);
  ++size_;
  sync_->condition_.notify_one();
}

template <typename T>
bool BlockingList<T>::try_peek_back(T* t) {
  std::mutex::scoped_lock lock(sync_->mutex_);

  if (list_.empty()) {
    return false;
  }

  *t = list_.back();
  return true;
}

template <typename T>
bool BlockingList<T>::try_peek_front(T* t) {
  std::mutex::scoped_lock lock(sync_->mutex_);

  if (list_.empty()) {
    return false;
  }

  *t = list_.front();
  return true;
}

template <typename T>
bool BlockingList<T>::try_pop_back(T* t) {
  std::mutex::scoped_lock lock(sync_->mutex_);

  if (list_.empty()) {
    return false;
  }

  *t = list_.back();
  list_.pop_back();
  --size_;
  return true;
}

template <typename T>
bool BlockingList<T>::try_pop_front(T* t) {
  std::mutex::scoped_lock lock(sync_->mutex_);

  if (list_.empty()) {
    return false;
  }

  *t = list_.front();
  list_.pop_front();

  --size_;
  return true;
}

template <typename T>
T BlockingList<T>::pop_back() {
  std::mutex::scoped_lock lock(sync_->mutex_);

  while (list_.empty()) {
    sync_->condition_.wait(lock);
  }

  T t = list_.back();
  list_.pop_back();
  --size_;
  return t;
}

template <typename T>
void BlockingList<T>::wait_until_not_empty() {
  std::mutex::scoped_lock lock(sync_->mutex_);

  while (list_.empty()) {
    sync_->condition_.wait(lock);
  }
}

template <typename T>
T BlockingList<T>::pop_front() {
  std::mutex::scoped_lock lock(sync_->mutex_);

  while (list_.empty()) {
    sync_->condition_.wait(lock);
  }

  T t = list_.front();
  list_.pop_front();
  --size_;
  return t;
}

template <typename T>
T BlockingList<T>::peek_front_unsafe() {
  T t = list_.front();
  return t;
}

template <typename T>
size_t BlockingList<T>::size() const {
  return size_;
}

}  // namespace common
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_COMMON_BLOCKING_LIST_H
