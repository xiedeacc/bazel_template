/*******************************************************************************
 * Copyright (c) 2022 Copyright 2022- xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef GRPC_FRAMEWORK_CLIENT_GRPC_WRITER_H
#define GRPC_FRAMEWORK_CLIENT_GRPC_WRITER_H

#include <deque>
#include <list>
#include <thread>

#include "glog/logging.h"
#include "google/protobuf/arena.h"
#include "grpcpp/grpcpp.h"
#include "src/common/grpc_framework/tag_base.h"

namespace bazel_template {
namespace common {
namespace grpc_framework {

class WriterCallback {
 public:
  virtual void OnWrite(int write_id) = 0;
  virtual void OnWriteError() = 0;
  virtual void Finish() = 0;
};

template <typename RequestType, typename WriterType>
class Writer : public TagBase {
 public:
  Writer(WriterCallback *cb, WriterType &async_writer,
         size_t buf_size = 1024 * 1024 * 32)
      : callback_(*cb), writer_impl_(async_writer) {
    auto_write_ = true;
    max_buffer_size_ = buf_size;
    cur_buffer_size_ = 0;
    status_ = STOP;
    input_id = output_id = 0;
  }

  virtual ~Writer() { Stop(); }

  virtual std::string Name() override { return "Writer"; }

  void SetAuto(bool auto_write) { auto_write_ = auto_write; }

  void Start() {
    lock_t lock(mtx_);
    if (status_ == STOP) {
      status_ = IDLE;
    }
    write_buffer_.clear();
  }

  void Stop() {
    lock_t lock(mtx_);
    status_ = STOP;
    while (write_buffer_.size() > 0) {
      RequestType *w = write_buffer_.front();
      write_buffer_.pop_front();
      google::protobuf::Arena *arena = w->GetArena();
      if (arena) {
        delete arena;
      }
    }
    cur_buffer_size_ = 0;
  }

  RequestType *NewRequest() {
    if (cur_buffer_size_ >= max_buffer_size_) {
      return nullptr;
    }
    // google::protobuf::Arena *arena = new google::protobuf::Arena();
    // if (arena == nullptr) {
    // return nullptr;
    //}
    // RequestType *request =
    // google::protobuf::Arena::CreateMessage<RequestType>(arena);
    // cur_buffer_size_ += arena->SpaceUsed();
    RequestType *request = new RequestType;
    return request;
  }

  int NewResponse(const int num, std::vector<RequestType *> &request_queue) {
    int ret = 0;
    for (; ret < num; ++ret) {
      if (cur_buffer_size_ >= max_buffer_size_) {
        break;
      }
      // google::protobuf::Arena *arena = new google::protobuf::Arena();
      // if (arena == nullptr) {
      // break;
      //}
      RequestType *request = new RequestType;
      cur_buffer_size_ += 0;
      LOG(INFO) << "cur_buffer_size: " << cur_buffer_size_;
      request_queue.push_back(request);
    }
    return ret;
  }

  int Write(RequestType *request) {
    lock_t lock(mtx_);
    if (status_ == STOP) {
      return -1;
    }

    if (cur_buffer_size_ >= max_buffer_size_) {
      return -1;
    }

    write_buffer_.push_back(request);
    if (status_ == IDLE) {
      GPR_ASSERT(write_buffer_.size() == 1);
      if (write_buffer_.front() == nullptr) {
        LOG(INFO) << "write done!";
        status_ = STOP;
        writer_impl_.WritesDone(this);
        return -1;
      } else {
        status_ = WRITING;
        writer_impl_.Write(*write_buffer_.front(), this);
      }
    }

    return input_id++;
  }

  template <class _InputIter>
  std::pair<int, int> Write(_InputIter __first, _InputIter __last) {
    lock_t lock(mtx_);

    if (status_ == STOP) {
      return {-1, -1};
    }

    if (__first == __last) {
      return {-1, -1};
    }

    if (cur_buffer_size_ >= max_buffer_size_) {
      return {-1, -1};
    }

    for (_InputIter it = __first; it != __last; ++it) {
      write_buffer_.push_back(*it);
    }

    if (status_ == IDLE) {
      GPR_ASSERT(write_buffer_.size() == std::distance(__first, __last));
      if (write_buffer_.front() == nullptr) {
        LOG(INFO) << "write done!";
        status_ = STOP;
        writer_impl_.WritesDone(this);
        return {-1, -1};
      } else {
        status_ = WRITING;
        writer_impl_.Write(*write_buffer_.front(), this);
      }
    }

    int original = input_id;
    input_id += std::distance(__first, __last);
    return {original, input_id - 1};
  }

  /// 发送队列中的下一个数据。仅当set_auto(false)时使用。
  /// 注意：此处没有线程同步。
  void WriteNext() {
    if (status_ == STOP || status_ == IDLE) {
      return;
    }
    GPR_ASSERT(write_buffer_.size() > 0);
    writer_impl_.Write(*write_buffer_.front(), this);
  }

  virtual void Process() {
    lock_t lock(mtx_);
    if (status_ == STOP) {
      write_buffer_.clear();
      callback_.Finish();
      return;
    }
    GPR_ASSERT(write_buffer_.size() >= 1);
    RequestType *w = write_buffer_.front();
    write_buffer_.pop_front();

    if (w == nullptr) {
      LOG(INFO) << "should never happen!";
    }

    google::protobuf::Arena *arena = w->GetArena();
    if (arena) {
      LOG(INFO) << "arena: " << arena;
      cur_buffer_size_ -= arena->SpaceUsed();
      delete arena;
    }
    callback_.OnWrite(output_id++);
    if (write_buffer_.size() > 0) {
      if (auto_write_) {
        w = write_buffer_.front();
        if (w == nullptr) {
          LOG(INFO) << "write done!";
          status_ = STOP;
          writer_impl_.WritesDone(this);
          return;
        }
        writer_impl_.Write(*write_buffer_.front(), this);
      }
    } else {
      status_ = IDLE;
    }
  }

  virtual void OnError() { callback_.OnWriteError(); }

 private:
  enum CallStatus { IDLE, WRITING, STOP };

  CallStatus status_;
  typedef std::unique_lock<std::mutex> lock_t;
  std::mutex mtx_;
  std::list<RequestType *> write_buffer_;
  size_t max_buffer_size_;
  size_t cur_buffer_size_;
  WriterCallback &callback_;
  WriterType &writer_impl_;
  int input_id;
  int output_id;
  bool auto_write_;
};

}  // namespace grpc_framework
}  // namespace common
}  // namespace bazel_template

#endif  // GRPC_FRAMEWORK_CLIENT_GRPC_WRITER_H
