set(SRC_DIR ${MASTER_CODE_DIR}/source)
aux_source_directory(${SRC_DIR} SRC)

add_library(master ${SRC})