#!/bin/bash

# This script runs cmake/ninja/make install on all `Project`
# directory suffixes passed to it
# Requirements:
# - os.h
# - cmake
# - ninja / make
# - (Win64) vcvars64.bat loaded into shell's PATH

THREADS=8

if [[ $# < 1 ]]; then
	echo "Syntax for Project<X>: install_builds.sh <X0> [X1] [X2...]"
	exit 1
fi

cd "$(dirname "${BASH_SOURCE[0]}")"
. ./os.sh

declare -a DIRS
ALL_DIRS=""
IDX=0
for SUFFIX in "${@}"; do
	DIRS[IDX]=../../Project$SUFFIX
	ALL_DIRS="$ALL_DIRS ${DIRS[IDX]}"
	let IDX=$IDX+1
done

## Confirm paths exist
for PROJ_PATH in "${DIRS[@]}"; do
	if [[ ! -d $PROJ_PATH ]]; then
		echo -e "\nError! [$PROJ_PATH] does not exist!"
		exit 1
	elif [[ ! -f $PROJ_PATH/CMakeCache.txt ]]; then
		echo -e "Warning! [$PROJ_PATH/CMakeCache.txt] does not exist!"
	fi
done

## MSVC Build
if [ "$IS_WIN64" == "TRUE" ]; then
	[ -z "$IS_VCVARS_SET" ] &&  echo -e "Warning: Win64 machine but vcvarsall.bat has not been run; linking (non-MinGW) builds will fail!"
fi

## Build and/or Install
for PROJ_PATH in "${DIRS[@]}"; do
	INSTALLING="\n== Installing [$PROJ_PATH]\n"
	echo -e "\n== Configuring [$PROJ_PATH]\n"
	$CMAKE $PROJ_PATH
	echo -e "\n== Building [$PROJ_PATH] [$THREADS T]\n"
	$CMAKE --build $PROJ_PATH -j$THREADS
	if [[ -f "$PROJ_PATH/build.ninja" ]]; then
		echo -e "$INSTALLING"
		$NINJA install -C $PROJ_PATH
	elif [[ -f "$PROJ_PATH/Makefile" ]]; then
		echo -e "$INSTALLING"
		$MAKE install -C $PROJ_PATH
	else
		echo "\n ERROR: Install [$PROJ_PATH] failed! (Unsupported build system)"
	fi
done
