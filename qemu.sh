#!/usr/bin/env bash

apt update && apt install -y cmake g++

mkdir build 
cd build 
cmake .. -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_BUILD_TYPE=$BUILD_TYPE 

cd .. 
cmake --build ./build --config $BUILD_TYPE

ls -lh ./build

chmod +x ./test.sh

./test.sh