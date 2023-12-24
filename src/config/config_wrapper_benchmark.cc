/*******************************************************************************
 * Copyright (c) 2023  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "benchmark/benchmark.h"
#include "src/config/base_config.h"
#include "src/config/config_wrapper.h"
#include "src/util/util.h"

namespace bazel_template {
namespace config {

using namespace std;
using namespace bazel_template::util;

ConfigWrapper<BaseConfig> base_config;
static void DoSetup(const benchmark::State &state) {
  base_config.Make(
      Util::LoadContent(string("test_data/config_test/base_config.json")));
}

static void ConfigWrapperBenchmark(benchmark::State &state) {
  if (state.thread_index() == 0) {
    base_config.Make(
        Util::LoadContent(string("test_data/config_test/base_config.json")));
  }

  for (auto _ : state) {
    base_config.Hold();
  }
}

BENCHMARK(ConfigWrapperBenchmark)
    ->UseRealTime()
    ->Threads(16)
    ->Threads(32)
    ->ThreadPerCpu();
BENCHMARK_MAIN();

}  // namespace config
}  // namespace bazel_template
