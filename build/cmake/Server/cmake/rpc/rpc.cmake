set(SRC_DIR ${RPC_CODE_DIR}/source)
set(HEADER_DIR ${RPC_CODE_DIR}/include)
set(RPC_PROTO ${RPC_CODE_DIR}/rpc.proto)
get_filename_component(PROTO_PATH "${RPC_PROTO}" PATH)

set(PROTO_SRC "${SRC_DIR}/rpc.pb.cc")
set(GRPC_SRC "${SRC_DIR}/rpc.grpc.pb.cc")
set(PROTO_HEADER "rpc.pb.h")
set(GRPC_HEADER "rpc.grpc.pb.h")

add_custom_command(
      OUTPUT "${PROTO_SRC}" "${GRPC_SRC}" "${PROTO_HEADER}" "${GRPC_HEADER}"
      COMMAND ${_PROTOBUF_PROTOC} 
      ARGS --grpc_out "${SRC_DIR}" --cpp_out "${SRC_DIR}" -I "${PROTO_PATH}" --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}" "${RPC_PROTO}"
      COMMAND ${CMAKE_COMMAND} -E rename ${SRC_DIR}/${PROTO_HEADER} ${HEADER_DIR}/${PROTO_HEADER}
      COMMAND ${CMAKE_COMMAND} -E rename ${SRC_DIR}/${GRPC_HEADER} ${HEADER_DIR}/${GRPC_HEADER}
      DEPENDS ${RPC_PROTO}
)

add_custom_target(
    generate_grpc
    DEPENDS ${PROTO_SRC} ${GRPC_SRC} ${HEADER_DIR}/${PROTO_HEADER} ${HEADER_DIR}/${GRPC_HEADER}
)

include_directories(${HEADER_DIR})
aux_source_directory(${SRC_DIR} SRC)

add_library(rpc ${SRC})
add_dependencies(rpc generate_grpc)