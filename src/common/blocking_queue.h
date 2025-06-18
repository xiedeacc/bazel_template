/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_COMMON_BLOCKING_QUEUE_H_
#define BAZEL_TEMPLATE_COMMON_BLOCKING_QUEUE_H_

#include <condition_variable>
#include <mutex>
#include <queue>

namespace bazel_template {
namespace common {

template <typename T>
class BlockingQueue {
 public:
  // Constructor
  BlockingQueue() = default;

  // Destructor
  ~BlockingQueue() = default;

  // Prevent copying
  BlockingQueue(const BlockingQueue&) = delete;
  BlockingQueue& operator=(const BlockingQueue&) = delete;

  // Push an item to the queue
  void Push(T item) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(std::move(item));
    cv_.notify_one();
  }

  bool TryPop(T& item) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
      return false;
    }
    item = std::move(queue_.front());
    queue_.pop();
    return true;
  }

  // Pop an item from the queue, blocking if the queue is empty
  // Returns false if the queue was closed
  bool Pop(T& item) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty() || closed_; });

    if (closed_ && queue_.empty()) {
      return false;
    }

    item = std::move(queue_.front());
    queue_.pop();
    return true;
  }

  // Close the queue and wake up all waiting threads
  void Close() {
    std::lock_guard<std::mutex> lock(mutex_);
    closed_ = true;
    cv_.notify_all();
  }

  // Check if the queue is empty
  bool Empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  // Get the current size of the queue
  size_t Size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
  }

 private:
  mutable std::mutex mutex_;
  std::condition_variable cv_;
  std::queue<T> queue_;
  bool closed_ = false;
};

}  // namespace common
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_COMMON_BLOCKING_QUEUE_H_
