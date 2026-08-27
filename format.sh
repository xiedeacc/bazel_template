#!/usr/bin/env bash
# Run clang-format over the repository.
#
#   ./format.sh          rewrite files in place
#   ./format.sh check     report violations, exit non-zero if any
#
# This exists because `bazel run //tools/format` does not work on Windows, for
# two separate reasons:
#
#   1. Bazel cannot CreateProcess a .bash file, which is what rules_lint emits.
#   2. The multirun aggregator locates its runfiles by looking for a .runfiles
#      directory next to argv[0]. Windows uses a manifest-based layout, so that
#      lookup fails even when the script is started from bash by hand.
#
# Calling the per-language script through bash sidesteps both: it is a plain
# script with no multirun involved. On Linux and macOS `bazel run //tools/format`
# works too, and does the same thing.
set -euo pipefail

mode="${1:-fix}"
case "$mode" in
fix) suffix="" ;;
check) suffix=".check" ;;
*)
  echo "usage: ${0##*/} [fix|check]" >&2
  exit 2
  ;;
esac

root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
target="//tools/format:format_C++_with_clang-format${suffix}"

bazel build "$target"
script="$(bazel info bazel-bin)/tools/format/format_C++_with_clang-format${suffix}.bash"

if [[ ! -f $script ]]; then
  echo "format script not found: $script" >&2
  exit 1
fi

BUILD_WORKSPACE_DIRECTORY="$root" RUNFILES_DIR="$script.runfiles" exec bash "$script"
