set(SERVER_CMAKE_DIR ${SERVER_CMAKE_DIR}/cmake)
set(RPC_CMAKE_DIR ${SERVER_CMAKE_DIR}/rpc)
set(MASTER_CMAKE_DIR ${SERVER_CMAKE_DIR}/master)
set(SLAVE_CMAKE_DIR ${SERVER_CMAKE_DIR}/slave)
set(COMM_CMAKE_DIR ${SERVER_CMAKE_DIR}/common)

set(RPC_CODE_DIR ${SERVER_CODE_DIR}/rpc)
set(MASTER_CODE_DIR ${SERVER_CODE_DIR}/master)
set(SLAVE_CODE_DIR ${SERVER_CODE_DIR}/slave)
set(COMM_CODE_DIR ${SERVER_CODE_DIR}/common)

include(${RPC_CMAKE_DIR}/common.cmake)

include(${COMM_CMAKE_DIR}/common.cmake)
include(${RPC_CMAKE_DIR}/rpc.cmake)
include(${MASTER_CMAKE_DIR}/master.cmake)
include(${SLAVE_CMAKE_DIR}/slave.cmake)

target_link_libraries(master PRIVATE common rpc)
target_link_libraries(slave PRIVATE common rpc)
 
install(TARGETS master DESTINATION ${INSTALL_DIR})
install(TARGETS slave DESTINATION ${INSTALL_DIR})