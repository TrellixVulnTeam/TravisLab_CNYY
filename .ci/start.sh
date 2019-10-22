#!/bin/bash
test $DEBUG && set -x

cmake --version && echo "\nninja: $(ninja --version)\n" && clang --version

git submodule update --init --recursive

export CMAKE_FLAGS="-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++"
export CONFIGS="Release Develop"

.ci/pre-build.sh Source/ThirdParty out/TP
.ci/build-configs.sh Source out
echo -e "\n== Success ==\n"
exit
