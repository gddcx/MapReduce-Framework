cmake_minimum_required(VERSION 3.16.0)
project(mapreduce)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

include_directories(${SRC_DIR}/mrapps/include)

aux_source_directory(${SRC_DIR}/mrapps/source SRC)
aux_source_directory(${SRC_DIR}/main/source SRC)

add_executable(run ${SRC})
install(TARGETS run DESTINATION ${BUILD_DIR})