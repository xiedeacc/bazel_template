#!/bin/bash

if [ $# != 1 ] ; then
    echo "must specify project name"
    exit -1
fi
echo "now clear $(pwd)/../$1"
rm -rf "$(pwd)/../$1"

echo "now copy bazel_template to $1"
cp -ra "$(pwd)" "$(pwd)/../$1"

cd "$(pwd)/../$1"
rm bazel-* -rf
mv "src/swig/java/bazel_template" "src/swig/java/$1"
sed -i "s/bazel_template/$1/g" `grep "bazel_template" -rl .`

