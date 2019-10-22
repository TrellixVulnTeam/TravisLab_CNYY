#!/bin/bash
test $DEBUG && set -x

echo -e "\n== Building ThirdParty ==\n"
cmake -GNinja $1 -B $2 -DBUILD_SHARED_LIBS=1 -DCI_BUILD=1 $CMAKE_FLAGS
ninja -v -C $2
cd ..

exit
