/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "src/common/dependency_map.h"

#include <assert.h>

#include <queue>

namespace bazel_template {
namespace common {

bool TopologicalSort(const DependencyMap& depends,
                     std::vector<std::string>* result) {
  std::map<std::string, int> indegrees;
  for (DependencyMap::const_iterator i = depends.begin(); i != depends.end();
       ++i) {
    const std::set<std::string>& edges = i->second;
    for (std::set<std::string>::const_iterator j = edges.begin();
         j != edges.end(); ++j) {
      ++indegrees[*j];
    }
  }

  std::queue<std::string> zero_queue;
  for (DependencyMap::const_iterator i = depends.begin(); i != depends.end();
       ++i) {
    if (indegrees.find(i->first) == indegrees.end()) zero_queue.push(i->first);
  }

  result->clear();

  while (!zero_queue.empty()) {
    result->push_back(zero_queue.front());
    zero_queue.pop();

    DependencyMap::const_iterator mit = depends.find(result->back());
    assert(mit != depends.end());

    const std::set<std::string>& edges = mit->second;
    for (std::set<std::string>::const_iterator j = edges.begin();
         j != edges.end(); ++j) {
      if (--indegrees[*j] == 0) zero_queue.push(*j);
    }
  }

  return result->size() == depends.size();
}

}  // namespace common
}  // namespace bazel_template
