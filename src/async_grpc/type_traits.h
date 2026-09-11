/*
 * Copyright 2017 The Cartographer Authors
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

#ifndef CPP_GRPC_TYPE_TRAITS_H_
#define CPP_GRPC_TYPE_TRAITS_H_

#include <grpc++/grpc++.h>

#include <concepts>
#include <type_traits>

// Only forward-declared: this header ends up in the global module fragment of
// bazel_template.async_grpc.client, and MSVC dies writing a BMI that reaches
// protobuf's full Message. The concepts below only need it complete where they
// are instantiated, which is the importing translation unit.
#include "src/util/util_fwd.h"

namespace async_grpc {

// Tags an incoming or outgoing message type as streaming.
template <typename Request>
class Stream {
  using type = Request;
};

template <template <typename> class, typename T>
struct Strip {
  using type = T;
};

template <template <typename> class T, typename Param>
struct Strip<T, T<Param>> {
  using type = Param;
};

template <typename T>
using StripStream = Strip<Stream, T>::type;

// Concepts describing the RPC service method definitions that this library is
// parameterized on. These replace the DEFINE_HAS_SIGNATURE /
// DEFINE_HAS_MEMBER_TYPE detection macros that used to live here.

// Provides 'static const char* MethodName()'.
template <typename T>
concept HasRpcMethodName = requires {
  { T::MethodName() } -> std::convertible_to<const char*>;
};

// Provides an 'IncomingType' typedef; i.e. the proto message passed to the
// service method. It may be wrapped (tagged) by async_grpc::Stream.
template <typename T>
concept HasIncomingType = requires { typename T::IncomingType; };

// Provides an 'OutgoingType' typedef; i.e. the proto message returned from the
// service method. It may be wrapped (tagged) by async_grpc::Stream.
template <typename T>
concept HasOutgoingType = requires { typename T::OutgoingType; };

// A type from which the properties of an RPC service method can be inferred.
// See RpcServiceMethodTraits.
template <typename T>
concept RpcServiceMethodSpec =
    HasRpcMethodName<T> && HasIncomingType<T> && HasOutgoingType<T>;

// A generated protocol buffer message type.
template <typename T>
concept ProtoMessage = std::derived_from<T, ::google::protobuf::Message>;

template <typename Incoming, typename Outgoing>
struct RpcType
    : public std::integral_constant<::grpc::internal::RpcMethod::RpcType,
                                    ::grpc::internal::RpcMethod::NORMAL_RPC> {};

template <typename Incoming, typename Outgoing>
struct RpcType<Stream<Incoming>, Outgoing>
    : public std::integral_constant<
          ::grpc::internal::RpcMethod::RpcType,
          ::grpc::internal::RpcMethod::CLIENT_STREAMING> {};

template <typename Incoming, typename Outgoing>
struct RpcType<Incoming, Stream<Outgoing>>
    : public std::integral_constant<
          ::grpc::internal::RpcMethod::RpcType,
          ::grpc::internal::RpcMethod::SERVER_STREAMING> {};

template <typename Incoming, typename Outgoing>
struct RpcType<Stream<Incoming>, Stream<Outgoing>>
    : public std::integral_constant<
          ::grpc::internal::RpcMethod::RpcType,
          ::grpc::internal::RpcMethod::BIDI_STREAMING> {};

}  // namespace async_grpc

#endif  // CPP_GRPC_TYPE_TRAITS_H_
