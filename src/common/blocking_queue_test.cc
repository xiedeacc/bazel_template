/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/common/blocking_queue.h"

#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <vector>

namespace bazel_template {
namespace common {
namespace {

TEST(BlockingQueueTest, BasicOperations) {
  BlockingQueue<int> queue;
  int value;

  // Test empty queue
  EXPECT_TRUE(queue.Empty());
  EXPECT_EQ(queue.Size(), 0);
  EXPECT_FALSE(queue.TryPop(value));

  // Test push and pop
  queue.Push(1);
  EXPECT_FALSE(queue.Empty());
  EXPECT_EQ(queue.Size(), 1);

  EXPECT_TRUE(queue.TryPop(value));
  EXPECT_EQ(value, 1);
  EXPECT_TRUE(queue.Empty());
  EXPECT_EQ(queue.Size(), 0);

  // Test TryPop on empty queue
  EXPECT_FALSE(queue.TryPop(value));

  // Test multiple values
  queue.Push(2);
  queue.Push(3);
  EXPECT_EQ(queue.Size(), 2);

  EXPECT_TRUE(queue.TryPop(value));
  EXPECT_EQ(value, 2);

  EXPECT_TRUE(queue.TryPop(value));
  EXPECT_EQ(value, 3);

  EXPECT_TRUE(queue.Empty());
  EXPECT_EQ(queue.Size(), 0);
}

TEST(BlockingQueueTest, CloseBehavior) {
  BlockingQueue<int> queue;
  int result;

  // Test close with empty queue
  queue.Close();
  EXPECT_FALSE(queue.Pop(result));

  // Test close with items in queue
  BlockingQueue<int> queue2;
  queue2.Push(1);
  queue2.Push(2);
  queue2.Close();

  EXPECT_TRUE(queue2.Pop(result));
  EXPECT_EQ(result, 1);
  EXPECT_TRUE(queue2.Pop(result));
  EXPECT_EQ(result, 2);
  EXPECT_FALSE(queue2.Pop(result));
}

TEST(BlockingQueueTest, MultipleThreads) {
  BlockingQueue<int> queue;
  const int num_items = 1000;
  std::vector<int> results;
  std::mutex results_mutex;

  // Producer thread
  std::thread producer([&queue]() {
    for (int i = 0; i < num_items; ++i) {
      queue.Push(i);
    }
    queue.Close();
  });

  // Consumer thread
  std::thread consumer([&queue, &results, &results_mutex]() {
    int value;
    while (queue.Pop(value)) {
      std::lock_guard<std::mutex> lock(results_mutex);
      results.push_back(value);
    }
  });

  producer.join();
  consumer.join();

  EXPECT_EQ(results.size(), num_items);
  for (int i = 0; i < num_items; ++i) {
    EXPECT_EQ(results[i], i);
  }
}

TEST(BlockingQueueTest, TryPopWithMultipleThreads) {
  BlockingQueue<int> queue;
  const int num_items = 1000;
  std::vector<int> results;
  std::mutex results_mutex;
  std::atomic<bool> running{true};

  // Producer thread
  std::thread producer([&queue]() {
    for (int i = 0; i < num_items; ++i) {
      queue.Push(i);
      std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
  });

  // Consumer thread using TryPop
  std::thread consumer([&queue, &results, &results_mutex, &running]() {
    int value;
    while (running) {
      if (queue.TryPop(value)) {
        std::lock_guard<std::mutex> lock(results_mutex);
        results.push_back(value);
      }
      std::this_thread::sleep_for(std::chrono::microseconds(50));
    }
  });

  // Wait for producer to finish
  producer.join();

  // Give consumer time to process remaining items
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  running = false;
  consumer.join();

  // Sort results since TryPop order is not guaranteed
  std::sort(results.begin(), results.end());

  EXPECT_EQ(results.size(), num_items);
  for (int i = 0; i < num_items; ++i) {
    EXPECT_EQ(results[i], i);
  }
}

}  // namespace
}  // namespace common
}  // namespace bazel_template
