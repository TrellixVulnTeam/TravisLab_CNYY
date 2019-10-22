FROM ubuntu:18.04

# Core
RUN apt-get update && apt-get install -y gnupg2 tar wget git g++-8 clang-8 ninja-build
# SFML
RUN apt-get install -y libx11-dev libxrandr-dev libgl1-mesa-dev libudev-dev libfreetype6-dev libflac-dev libopenal-dev libvorbis-dev

# Environment
ENV CMAKE_ROOT /usr/share/cmake-3.15

# CMake
RUN CMAKE_FILE=cmake-3.15.4-Linux-x86_64 && \
	wget https://github.com/Kitware/CMake/releases/download/v3.15.4/$CMAKE_FILE.tar.gz && \
	tar -xf $CMAKE_FILE.tar.gz && \
	cp -a $CMAKE_FILE/. /usr

# C++
RUN ln -s /usr/bin/g++-8 /usr/bin/g++ && \
	ln -s /usr/bin/clang-8 /usr/bin/clang && \
	ln -s /usr/bin/clang++-8 /usr/bin/clang++

WORKDIR /workspace
