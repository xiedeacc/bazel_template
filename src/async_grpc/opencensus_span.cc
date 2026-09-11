/*
 * Copyright 2018 The Cartographer Authors
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

module;

#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"
#include "grpc++/grpc++.h"
#include "grpc++/impl/codegen/async_stream.h"
#include "grpc++/impl/codegen/async_unary_call.h"
#include "grpc++/impl/codegen/proto_utils.h"
#include "grpc++/impl/codegen/service_type.h"
#include "src/async_grpc/common/blocking_queue.h"
#include "src/async_grpc/common/mutex.h"
#include "src/async_grpc/common/time.h"
#include "src/async_grpc/rpc_service_method_traits.h"
#include "src/async_grpc/type_traits.h"
#include "src/common/logging.h"
#include "src/util/util_fwd.h"

module bazel_template.async_grpc;

#if BUILD_TRACING

#include <memory>
#include <string_view>
#include <utility>

namespace async_grpc {

std::unique_ptr<Span> OpencensusSpan::StartSpan(std::string_view name,
                                                const OpencensusSpan* parent) {
  return std::unique_ptr<OpencensusSpan>(new OpencensusSpan(name, parent));
}

std::unique_ptr<Span> OpencensusSpan::CreateChildSpan(std::string_view name) {
  return std::unique_ptr<OpencensusSpan>(new OpencensusSpan(name, this));
}

void OpencensusSpan::SetStatus(const ::grpc::Status& status) {
  span_.SetStatus(
      static_cast<opencensus::trace::StatusCode>(status.error_code()));
}

void OpencensusSpan::End() {
  span_.End();
}

OpencensusSpan::OpencensusSpan(std::string_view name,
                               const OpencensusSpan* parent)
    : span_(opencensus::trace::Span::StartSpan(
          name, parent != nullptr ? &parent->span_ : nullptr)) {}

}  // namespace async_grpc

#endif  // BUILD_TRACING
