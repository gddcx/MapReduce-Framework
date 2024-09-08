cmake_minimum_required(VERSION 3.8)

project(MapReduceServer C CXX)

set(CMAKE_CXX_FLAGS "-std=c++14 -Ithread -Wall -O3")

set(SERVER_CODE_DIR ${ROOT_DIR}/Server)
set(CLIENT_CMAKE_DIR ${ROOT_DIR}/build/cmake/Client)
set(SERVER_CMAKE_DIR ${ROOT_DIR}/build/cmake/Server)

include_directories(${SERVER_CODE_DIR}/common/include)
include_directories(${SERVER_CODE_DIR}/rpc/include)
include_directories(${SERVER_CODE_DIR}/master/include)
include_directories(${SERVER_CODE_DIR}/slave/include)

include(${SERVER_CMAKE_DIR}/cmake/server.cmake)