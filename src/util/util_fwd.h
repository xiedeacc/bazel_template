/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_UTIL_UTIL_FWD_H_
#define BAZEL_TEMPLATE_UTIL_UTIL_FWD_H_

// Forward declarations for the two types bazel_template.util only names in its
// public declarations. Including the real folly/IPAddress.h plus
// google/protobuf/message.h in util.cppm's global module fragment makes MSVC
// 14.44 die with an internal compiler error while writing the BMI, makes
// Apple clang reject protobuf's parse_context.h, and gives gcc a BMI it
// cannot merge with the importer's textual copy of those headers. MSVC also
// accepts nothing but preprocessor directives directly in that fragment
// (warning C5202), hence a header. gcc does need the importer to see the
// real declaration *before* the import; every translation unit imports
// last, so it always does.
//
// Being a header, this is part of the global module, so the classes stay
// attached to the global module and match the real definitions that util.cc
// and the callers include.

namespace folly {
class IPAddress;
}  // namespace folly

namespace google::protobuf {
class Descriptor;
class DescriptorPool;
class Message;
}  // namespace google::protobuf

#endif  // BAZEL_TEMPLATE_UTIL_UTIL_FWD_H_
