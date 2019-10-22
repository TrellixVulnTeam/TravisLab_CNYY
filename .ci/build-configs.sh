#!/bin/bash
test $DEBUG && set -x
set -e

# Build LittleEngine
build() {
	CONFIG=$1
	echo -e "\n== Building LittleEngine | $CONFIG...\n"
	cmake -G Ninja $2 -B$3/$1 -DCI_BUILD=1 -DCMAKE_BUILD_TYPE=$CONFIG -DSFML_STATIC_LIBS=0 $CMAKE_FLAGS
	ninja -v -C $3/$1
	ninja install -C $3/$1
}

[[ -z "$CONFIGS" ]] && CONFIGS=Release

for CONFIG in $CONFIGS; do
	build $CONFIG $*
done

exit
