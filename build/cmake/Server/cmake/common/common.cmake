set(SRC_DIR ${COMM_CODE_DIR}/source)
aux_source_directory(${SRC_DIR} SRC)

add_library(common ${SRC})