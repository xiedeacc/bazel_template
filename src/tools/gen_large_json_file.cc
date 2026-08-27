/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "src/common/logging.h"

namespace {

constexpr size_t TARGET_SIZE = size_t{5} * 1024 * 1024;  // 200MB
constexpr size_t CHUNK_SIZE =
    size_t{1024} * 1024;  // 1MB chunks for progress reporting

void GenerateLargeJsonFile(const std::string& output_path) {
  std::ofstream out(output_path, std::ios::binary);
  if (!out) {
    LOG(ERROR) << "Failed to open output file: " << output_path;
    return;
  }

  // Write the opening of the JSON object and array
  out << "{\"nodes\":[";

  size_t total_written = 0;
  size_t count = 0;
  bool first = true;

  // Keep writing until we reach the target size
  while (total_written < TARGET_SIZE) {
    // Write a chunk of array elements
    for (size_t i = 0; i < 1000 && total_written < TARGET_SIZE; ++i) {
      if (!first) {
        out << ",";
      }
      first = false;

      // Write a simple object with just an 'f' field
      out << R"({"f":")" << std::setw(8) << std::setfill('0') << count << "\"}";

      count++;
      total_written += 25;  // Approximate size of each element
    }

    // Report progress
    if (total_written % CHUNK_SIZE < 1000) {  // Report roughly every MB
      std::cout << "\rProgress: " << (total_written * 100 / TARGET_SIZE)
                << "% (" << (total_written / (size_t{1024} * 1024)) << "MB)"
                << std::flush;
    }
  }

  // Write the closing of the array and object
  out << "]}";
  out.close();

  std::cout << "\nGenerated " << count << " nodes in " << output_path << '\n';
  std::cout << "Total size: " << (total_written / (size_t{1024} * 1024)) << "MB"
            << '\n';
}

}  // namespace

int main(int argc, char* argv[]) {
  // An exception escaping main() calls std::terminate before anything is
  // logged, which makes a failure look like a silent crash.
  try {
    bazel_template::logging::Initialize(argv[0]);

    if (argc != 2) {
      std::cerr << "Usage: " << argv[0] << " <output_file>" << '\n';
      return 1;
    }

    GenerateLargeJsonFile(argv[1]);
    return 0;
  } catch (const std::exception& e) {
    ::bazel_template::logging::ReportException("gen_large_json_file failed",
                                               e.what());
    return 1;
  } catch (...) {
    ::bazel_template::logging::ReportException("gen_large_json_file failed",
                                               nullptr);
    return 1;
  }
}
