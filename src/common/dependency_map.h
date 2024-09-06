/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#ifndef BAZEL_TEMPLATE_COMMON_DEPENDENCY_MAP_H
#define BAZEL_TEMPLATE_COMMON_DEPENDENCY_MAP_H

#include <map>
#include <set>
#include <string>
#include <vector>

namespace bazel_template {
namespace common {
/// represent dependency rules, such as makefile rules:
/// A: B C
/// B: C
/// C:
typedef std::map<std::string, std::set<std::string>> DependencyMap;

/// Dependency map is a DAG(directed acyclic graph), do topological sort on the
/// DAG yield sorted result.
bool TopologicalSort(const DependencyMap& depends,
                     std::vector<std::string>* result);

}  // namespace common
}  // namespace bazel_template

#endif  // BAZEL_TEMPLATE_COMMON_DEPENDENCY_MAP_H
