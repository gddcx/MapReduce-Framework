cmake_minimum_required(VERSION 3.8)

project(Test)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -O3")

set(TEST_DIR ${ROOT_DIR}/Test)
set(SOURCE_DIR ${TEST_DIR}/source)

# 临时使用，之后要切到Client的目录。只给用户暴露Client的库
set(MAPREDUCE_LIB ${ROOT_DIR}/Server)
include_directories(${MAPREDUCE_LIB}/master/include)
include_directories(${MAPREDUCE_LIB}/slave/include)
include_directories(${MAPREDUCE_LIB}/rpc/include)
include_directories(${MAPREDUCE_LIB}/common/include)
include_directories(${ROOT_DIR}/Client/include)

include(${ROOT_DIR}/build/cmake/Server/cmake/rpc/common.cmake)

add_executable(test_master "${SOURCE_DIR}/test_master.cpp")
target_link_libraries(test_master PRIVATE
${MR_LIB}/libmaster.a
absl::check 
absl::flags
absl::flags_parse
absl::log
${_REFLECTION} 
${_GRPC_GRPCPP} 
${_PROTOBUF_LIBPROTOBUF}
)
  
add_executable(test_worker "${SOURCE_DIR}/test_worker.cpp")
target_link_libraries(test_worker PRIVATE
${MR_LIB}/libslave.a
absl::check 
absl::flags
absl::flags_parse
absl::log
${_REFLECTION} 
${_GRPC_GRPCPP} 
${_PROTOBUF_LIBPROTOBUF}
)