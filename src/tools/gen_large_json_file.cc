/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

// Writes a large JSON document for load tests.

#include <cstddef>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "glog/logging.h"

namespace {

constexpr size_t kMegabyte = size_t{1024} * 1024;
constexpr size_t kTargetSize = 5 * kMegabyte;
constexpr size_t kChunkSize = kMegabyte;  // progress reporting granularity
constexpr size_t kElementsPerChunk = 1000;
constexpr size_t kApproximateElementSize = 25;
constexpr int kNodeWidth = 8;

void GenerateLargeJsonFile(const std::string& output_path) {
  std::ofstream out(output_path, std::ios::binary);
  if (!out) {
    LOG(ERROR) << "Failed to open output file: " << output_path;
    return;
  }

  out << R"({"nodes":[)";

  size_t total_written = 0;
  size_t count = 0;
  bool first = true;
  while (total_written < kTargetSize) {
    for (size_t i = 0; i < kElementsPerChunk && total_written < kTargetSize;
         ++i) {
      if (!first) {
        out << ",";
      }
      first = false;
      out << R"({"f":")" << std::setw(kNodeWidth) << std::setfill('0') << count
          << R"("})";
      count++;
      total_written += kApproximateElementSize;
    }
    if (total_written % kChunkSize < kElementsPerChunk) {
      std::cout << "\rProgress: " << (total_written * 100 / kTargetSize)
                << "% (" << (total_written / kMegabyte) << "MB)" << std::flush;
    }
  }

  out << "]}";
  out.close();

  std::cout << "\nGenerated " << count << " nodes in " << output_path << '\n'
            << "Total size: " << (total_written / kMegabyte) << "MB\n";
}

// noexcept: reporting from a catch handler must not throw again, which is
// what keeps main() exception-safe.
void ReportFailure(const char* what) noexcept {
  try {
    std::cerr << "Failed" << (what == nullptr ? "" : ": ")
              << (what == nullptr ? "" : what) << '\n';
  } catch (...) {  // NOLINT(bugprone-empty-catch)
  }
}

}  // namespace

int main(int argc, char* argv[]) {
  try {
    google::InitGoogleLogging(argv[0]);
    if (argc != 2) {
      std::cerr << "Usage: " << argv[0] << " <output_file>\n";
      return 1;
    }
    GenerateLargeJsonFile(argv[1]);
    return 0;
  } catch (const std::exception& e) {
    ReportFailure(e.what());
    return 1;
  } catch (...) {
    ReportFailure(nullptr);
    return 1;
  }
}
