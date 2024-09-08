set(SRC_DIR ${SLAVE_CODE_DIR}/source)
aux_source_directory(${SRC_DIR} SRC)

add_library(slave STATIC ${SRC})