/*
 * Copyright 2025 The bazel_template Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// End-to-end lifetime tests for async_grpc::Server. These exist above all to
// be run under AddressSanitizer/LeakSanitizer: every path here has to leave no
// live Rpc behind in ActiveRpcs once Server::Shutdown() returns.

#include <memory>
#include <string>
#include <vector>

#include "grpc++/grpc++.h"
#include "grpc++/impl/codegen/proto_utils.h"
#include "grpcpp/generic/async_generic_service.h"
#include "gtest/gtest.h"
#include "src/async_grpc/test_service.pb.h"
#include "src/async_grpc/type_traits.h"
#include "src/common/logging.h"

import bazel_template.async_grpc;
import bazel_template.async_grpc.client;

namespace async_grpc {
namespace {

using async_grpc_test::TestRequest;
using async_grpc_test::TestResponse;

struct EchoSignature {
  static const char* MethodName() {
    return "/async_grpc_test.TestService/Echo";
  }
  using IncomingType = TestRequest;
  using OutgoingType = TestResponse;
};

struct EchoStreamSignature {
  static const char* MethodName() {
    return "/async_grpc_test.TestService/EchoStream";
  }
  using IncomingType = TestRequest;
  using OutgoingType = Stream<TestResponse>;
};

struct SumStreamSignature {
  static const char* MethodName() {
    return "/async_grpc_test.TestService/SumStream";
  }
  using IncomingType = Stream<TestRequest>;
  using OutgoingType = TestResponse;
};

class EchoHandler : public RpcHandler<EchoSignature> {
 public:
  void OnRequest(const TestRequest& request) override {
    auto response = std::make_unique<TestResponse>();
    response->set_value(request.value());
    Send(std::move(response));
  }
};

class EchoStreamHandler : public RpcHandler<EchoStreamSignature> {
 public:
  void OnRequest(const TestRequest& request) override {
    for (int i = 0; i < request.value(); ++i) {
      auto response = std::make_unique<TestResponse>();
      response->set_value(i);
      Send(std::move(response));
    }
    Finish(::grpc::Status::OK);
  }
};

class SumStreamHandler : public RpcHandler<SumStreamSignature> {
 public:
  void OnRequest(const TestRequest& request) override {
    sum_ += request.value();
  }

  void OnReadsDone() override {
    auto response = std::make_unique<TestResponse>();
    response->set_value(sum_);
    Send(std::move(response));
  }

 private:
  int sum_ = 0;
};

// Binds an ephemeral loopback port with a throwaway gRPC server just to learn
// a port number that is free right now. Doing it through gRPC keeps the test
// free of platform socket code.
int PickUnusedPort() {
  ::grpc::ServerBuilder builder;
  // gRPC refuses to start a server that serves nothing, so register a generic
  // service and a completion queue that we immediately tear down again.
  ::grpc::AsyncGenericService generic_service;
  builder.RegisterAsyncGenericService(&generic_service);
  const std::unique_ptr<::grpc::ServerCompletionQueue> completion_queue =
      builder.AddCompletionQueue();
  int port = 0;
  builder.AddListeningPort("127.0.0.1:0", ::grpc::InsecureServerCredentials(),
                           &port);
  const std::unique_ptr<::grpc::Server> server = builder.BuildAndStart();
  CHECK(server) << "Could not bind an ephemeral loopback port.";
  CHECK_GT(port, 0);
  server->Shutdown();
  completion_queue->Shutdown();
  void* tag = nullptr;
  bool ok = false;
  while (completion_queue->Next(&tag, &ok)) {
  }
  server->Wait();
  return port;
}

class ServerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    server_address_ = "127.0.0.1:" + std::to_string(PickUnusedPort());
    Server::Builder builder;
    builder.SetNumGrpcThreads(2);
    builder.SetNumEventThreads(2);
    builder.SetServerAddress(server_address_);
    builder.RegisterHandler<EchoHandler>();
    builder.RegisterHandler<EchoStreamHandler>();
    builder.RegisterHandler<SumStreamHandler>();
    server_ = builder.Build();
    server_->Start();
  }

  void TearDown() override { ShutdownServer(); }

  void ShutdownServer() {
    if (server_) {
      server_->Shutdown();
      server_.reset();
    }
  }

  [[nodiscard]] std::shared_ptr<::grpc::Channel> CreateChannel() const {
    return ::grpc::CreateChannel(server_address_,
                                 ::grpc::InsecureChannelCredentials());
  }

  std::string server_address_;
  std::unique_ptr<Server> server_;
};

// The plain start/shutdown cycle: every listening Rpc that Service registered
// has to be gone by the time Shutdown() returns.
TEST_F(ServerTest, StartsAndShutsDownWithoutAnyCall) {
  ShutdownServer();
}

TEST_F(ServerTest, HandlesUnaryCalls) {
  Client<EchoSignature> client(CreateChannel());
  for (int i = 1; i <= 3; ++i) {
    TestRequest request;
    request.set_value(i);
    ::grpc::Status status;
    ASSERT_TRUE(client.Write(request, &status)) << status.error_message();
    EXPECT_EQ(client.response().value(), i);
  }
  ShutdownServer();
}

TEST_F(ServerTest, HandlesServerStreamingCalls) {
  Client<EchoStreamSignature> client(CreateChannel());
  TestRequest request;
  request.set_value(4);
  ::grpc::Status status;
  ASSERT_TRUE(client.Write(request, &status)) << status.error_message();

  std::vector<int> values;
  TestResponse response;
  while (client.StreamRead(&response)) {
    values.push_back(response.value());
  }
  EXPECT_TRUE(client.StreamFinish().ok());
  EXPECT_EQ(values, (std::vector<int>{0, 1, 2, 3}));
  ShutdownServer();
}

TEST_F(ServerTest, HandlesClientStreamingCalls) {
  Client<SumStreamSignature> client(CreateChannel());
  for (int i = 1; i <= 4; ++i) {
    TestRequest request;
    request.set_value(i);
    ::grpc::Status status;
    ASSERT_TRUE(client.Write(request, &status)) << status.error_message();
  }
  EXPECT_TRUE(client.StreamWritesDone());
  EXPECT_TRUE(client.StreamFinish().ok());
  EXPECT_EQ(client.response().value(), 1 + 2 + 3 + 4);
  ShutdownServer();
}

// Shutting down with a client stream still open: the server-side Rpc is in the
// middle of a read when the gRPC server goes away, and both its READ and its
// DONE tag come back from the completion queue during shutdown.
TEST_F(ServerTest, ShutsDownWithAnOpenClientStream) {
  Client<SumStreamSignature> client(CreateChannel());
  TestRequest request;
  request.set_value(7);
  ::grpc::Status status;
  ASSERT_TRUE(client.Write(request, &status)) << status.error_message();

  ShutdownServer();

  // The call is torn down by the shutdown; we only care that the client is
  // told about it rather than hanging, and that the server exited cleanly.
  const ::grpc::Status finish_status = client.StreamFinish();
  LOG(INFO) << "Stream finished with: " << finish_status.error_code() << " "
            << finish_status.error_message();
}

// A second start/shutdown cycle in the same process, with traffic in between,
// catches state that leaks from one Server into the next.
TEST_F(ServerTest, SurvivesRepeatedCallsAndShutdown) {
  {
    Client<EchoSignature> client(CreateChannel());
    TestRequest request;
    request.set_value(11);
    ::grpc::Status status;
    ASSERT_TRUE(client.Write(request, &status)) << status.error_message();
    EXPECT_EQ(client.response().value(), 11);
  }
  ShutdownServer();

  SetUp();
  {
    Client<EchoSignature> client(CreateChannel());
    TestRequest request;
    request.set_value(12);
    ::grpc::Status status;
    ASSERT_TRUE(client.Write(request, &status)) << status.error_message();
    EXPECT_EQ(client.response().value(), 12);
  }
  ShutdownServer();
}

}  // namespace
}  // namespace async_grpc
