#ifndef CPP_GRPC_RPC_SERVICE_METHOD_TRAITS_H
#define CPP_GRPC_RPC_SERVICE_METHOD_TRAITS_H

#include "src/async_grpc/type_traits.h"

namespace async_grpc {

// The RPC service method concept describes types from which properties of an
// RPC service can be inferred. The type RpcServiceMethod satisfies the RPC
// service concept (async_grpc::RpcServiceMethodSpec) if:
//   1) it provides a static member function ' const char* MethodName()'
//   2) it provides an 'IncomingType' typedef; i.e. the proto message passed to
//      the service method
//   3) it provides an 'OutgoingType' typedef; i.e. the proto message passed to
//      the service method
// Note: the IncomingType and OutgoingType specified above may be wrapped (or
//       tagged) by async_grpc::Stream.
template <RpcServiceMethodSpec RpcServiceMethodConcept>
struct RpcServiceMethodTraits {
  // Returns the fully qualified name of the gRPC method this handler is
  // implementing. The fully qualified name has the structure
  // '/<<full service name>>/<<method name>>', where the service name is the
  // fully qualified proto package name of the service and method name the
  // name of the method as defined in the service definition of the proto.
  [[nodiscard]] static constexpr const char* MethodName() {
    return RpcServiceMethodConcept::MethodName();
  }

  // An object derived from ::google::protobuf::Message which is passed to a
  // specific service method.
  using RequestType =
      StripStream<typename RpcServiceMethodConcept::IncomingType>;

  // An object derived from ::google::protobuf::Message which is returned from a
  // specific service method.
  using ResponseType =
      StripStream<typename RpcServiceMethodConcept::OutgoingType>;

  static_assert(
      ProtoMessage<RequestType>,
      "The RPC request type must be derived from ::google::protobuf::Message.");

  static_assert(ProtoMessage<ResponseType>,
                "The RPC response type must be derived from "
                "::google::protobuf::Message.");

  // The streaming type of the service method. See also
  // ::grpc::internal::RpcMethod.
  static constexpr auto StreamType =
      RpcType<typename RpcServiceMethodConcept::IncomingType,
              typename RpcServiceMethodConcept::OutgoingType>::value;
};

}  // namespace async_grpc

#endif  // CPP_GRPC_RPC_SERVICE_METHOD_TRAITS_H
