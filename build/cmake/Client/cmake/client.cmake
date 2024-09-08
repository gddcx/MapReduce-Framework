include_directories(${CLIENT_CODE_DIR}/include)
aux_source_directory(${CLIENT_CODE_DIR}/source SRC)

add_library(client ${SRC})