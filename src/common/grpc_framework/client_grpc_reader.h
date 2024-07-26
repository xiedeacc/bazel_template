/*******************************************************************************
 * Copyright (c) 2022 Copyright 2022- xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef GRPC_FRAMEWORK_CLIENT_GRPC_READER_H
#define GRPC_FRAMEWORK_CLIENT_GRPC_READER_H

#include "glog/logging.h"
#include "google/protobuf/arena.h"
#include "grpcpp/grpcpp.h"
#include "src/common/grpc_framework/tag_base.h"

namespace bazel_template {
namespace common {
namespace grpc_framework {

template <typename ResponseType, typename ReaderType>
class Reader;

class ReaderCallback {
  template <typename ResponseType, typename ReaderType>
  friend class Reader;

 public:
  virtual void OnReadError() = 0;
  virtual void OnRead(void *req) = 0;
};

template <typename ResponseType, typename ReaderType>
class Reader : public TagBase {
 public:
  Reader(ReaderCallback *cb, ReaderType &async_reader)
      : callback_(*cb), reader_impl_(async_reader), auto_read_(true) {
    arena_ =
        std::unique_ptr<google::protobuf::Arena>(new google::protobuf::Arena());
  }

  virtual std::string Name() override { return "Reader"; }

  void SetAuto(bool auto_read) { auto_read_ = auto_read; }

  void Read() {
    LOG(INFO) << "start reading, " << this;
    arena_->Reset();
    rsp_ = google::protobuf::Arena::Create<ResponseType>(arena_.get());
    reader_impl_.Read(rsp_, this);
  }

  virtual void Process() {
    LOG(INFO) << "end reading, " << this;
    callback_.OnRead((void *)rsp_);
    if (auto_read_) {
      Read();
    }
  }

  virtual void OnError() {
    LOG(ERROR) << "OnError";
    callback_.OnReadError();
  }

 private:
  ReaderCallback &callback_;
  ReaderType &reader_impl_;
  bool auto_read_;
  ResponseType *rsp_;

  std::unique_ptr<google::protobuf::Arena> arena_;
};

}  // namespace grpc_framework
}  // namespace common
}  // namespace bazel_template

#endif  // GRPC_FRAMEWORK_CLIENT_GRPC_READER_H
