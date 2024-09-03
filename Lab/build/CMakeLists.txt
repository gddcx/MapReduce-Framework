# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake build file for C++ helloworld example.
# Assumes protobuf and gRPC have been installed using cmake.
# See cmake_externalproject/CMakeLists.txt for all-in-one cmake build
# that automatically builds all the dependencies before building helloworld.

cmake_minimum_required(VERSION 3.8)

project(HelloWorld C CXX)

set(CMAKE_CXX_FLAGS "-Ithread")

include(${CMAKE_DIR}/common.cmake)

# grpc_proto
set(rpc ${ROOT_DIR}/rpc)
set(rpc_proto ${rpc}/mr.proto)
get_filename_component(proto_path "${rpc_proto}" PATH)
set(proto_srcs "${rpc}/mr.pb.cc")
set(proto_hdrs "${rpc}/mr.pb.h")
set(grpc_srcs "${rpc}/mr.grpc.pb.cc")
set(grpc_hdrs "${rpc}/mr.grpc.pb.h")

add_custom_command(
      OUTPUT "${proto_srcs}" "${proto_hdrs}" "${grpc_srcs}" "${grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${rpc}"
        --cpp_out "${rpc}"
        -I "${proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${rpc_proto}"
      DEPENDS "${rpc_proto}")

set(grpc_client_srcs "${rpc}/RpcClient.cpp")
set(grpc_client_hdrs "${rpc}/RpcClient.h")
set(grpc_server_srcs "${rpc}/RpcServer.cpp")
set(grpc_server_hdrs "${rpc}/RpcServer.h")
add_library(grpc_proto
  ${proto_srcs}
  ${proto_hdrs}
  ${grpc_srcs}
  ${grpc_hdrs}
  ${grpc_client_srcs}
  ${grpc_client_hdrs}
  ${grpc_server_srcs}
  ${grpc_server_hdrs})
target_link_libraries(grpc_proto
  absl::check
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# source file
set(src ${ROOT_DIR}/source)
set(hdrs ${ROOT_DIR}/include)

include_directories(${rpc})
include_directories(${hdrs})

add_executable(test_master "${src}/test_master.cpp" "${src}/MasterNode.cpp")
target_link_libraries(test_master
  grpc_proto
  absl::check
  absl::flags
  absl::flags_parse
  absl::log
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
  
add_executable(test_worker "${src}/test_worker.cpp" "${src}/WorkerNode.cpp" "${src}/Timer.cpp" "${src}/function.cpp")
target_link_libraries(test_worker
  grpc_proto
  absl::check
  absl::flags
  absl::flags_parse
  absl::log
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})