/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "boost/thread/executors/basic_thread_pool.hpp"
#include "brpc/channel.h"
#include "brpc/server.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "google/protobuf/util/json_util.h"
#include "src/proto/service.pb.h"
#include "src/util/config_util.h"

using namespace bazel_template::proto;
using namespace bazel_template::util;
using namespace google::protobuf::util;
using namespace std;

atomic<int> cnt = 0;
const int thread_num = 10;

vector<Request> Buildrequest(int thread_id) {
  vector<string> dhids = Util::LoadLines("test_data/request_json");
  vector<Request> ret;
  for (size_t i = thread_id; i < dhids.size(); i += thread_num) {
    Request request;
    ret.push_back(request);
  }
  return ret;
}

absl::base_internal::SpinLock lock_;
int DoRpcRequest(int thread_id) {
  brpc::Channel channel;
  brpc::ChannelOptions options;
  options.protocol = "baidu_std";
  options.timeout_ms = 10000;
  options.max_retry = 3;
  if (channel.Init("127.0.0.1:10005", "", &options) != 0) {
    LOG(ERROR) << "Fail to initialize channel";
    return -1;
  }

  Service_Stub stub(&channel);
  vector<Request> requests = Buildrequest(thread_id);
  vector<Response> responses;
  LOG(INFO) << "requests size: " << requests.size();
  for (size_t i = thread_id; i < requests.size(); i += thread_num) {
    auto &request = requests[i];
    Response response;
    brpc::Controller cntl;

    stub.Handle(&cntl, &request, &response, nullptr);
    if (cntl.Failed()) {
      LOG(ERROR) << cntl.ErrorText();
      return -1;
    }

    LOG(INFO) << "response: " << response.msg();
    responses.emplace_back(std::move(response));
    Util::Sleep(5);
  }

  absl::base_internal::SpinLockHolder locker(&lock_);
  for (size_t i = 0; i < responses.size(); ++i) {
  }

  ++cnt;
  return 0;
}

int main(int argc, char *argv[]) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  FLAGS_log_dir = "./log";
  FLAGS_stop_logging_if_full_disk = true;
  google::InitGoogleLogging(argv[0]);

  std::shared_ptr<boost::executors::basic_thread_pool> thread_pool;
  thread_pool.reset(new boost::basic_thread_pool(10));
  for (int i = 0; i < thread_num; ++i) {
    thread_pool->submit(std::bind(DoRpcRequest, i));
  }
  while (cnt.load() != 10) {
    Util::Sleep(100);
  }

  thread_pool->close();
  LOG(INFO) << "DebugClient is going to quit";
  return 0;
}
