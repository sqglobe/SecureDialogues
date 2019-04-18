#!/bin/bash
git clone https://github.com/weidai11/cryptopp.git && git checkout CRYPTOPP_8_1_0
cd cryptopp 
git submodule add https://github.com/noloader/cryptopp-cmake.git cmake
git submodule update --remote

cp "$PWD/cmake/cryptopp-config.cmake" "$PWD"
cp "$PWD/cmake/CMakeLists.txt" "$PWD"
