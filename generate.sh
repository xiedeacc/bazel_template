#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 repo_name"
    exit 1
fi

SOURCE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TARGET_DIR="../$1"

cp -r "$SOURCE_DIR" "$TARGET_DIR"
echo "Copied $SOURCE_DIR to $TARGET_DIR"
cd $TARGET_DIR
rm -rf .git
sed -i 's/bazel_template/$1/' `grep 'bazel_template' -rl .`
