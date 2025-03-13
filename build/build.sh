BUILD_DIR=`pwd`
ROOT_DIR=`dirname ${BUILD_DIR}`
TMP_DIR=${BUILD_DIR}/tmp
CMAKE_DIR=${BUILD_DIR}/cmake

# ============= Server编译 ============= #
SERVER_CMAKE_DIR=${CMAKE_DIR}/Server
SERVER_TMP_DIR=${TMP_DIR}/server
SERVER_INSTALL_DIR=${TMP_DIR}

mkdir -p ${SERVER_TMP_DIR}
cd ${SERVER_TMP_DIR}

cmake ${SERVER_CMAKE_DIR} \
-G "Unix Makefiles" \
-DROOT_DIR=${ROOT_DIR} \
-DCMAKE_DIR=${CMAKE_DIR} \
-DINSTALL_DIR=${SERVER_INSTALL_DIR}

make -j4
make install
# ============= Test编译 ============= #
TEST_CMAKE_DIR=${CMAKE_DIR}/Test
TEST_TMP_DIR=${TMP_DIR}/test

mkdir -p ${TEST_TMP_DIR}
cd ${TEST_TMP_DIR}

cmake ${TEST_CMAKE_DIR} \
-G "Unix Makefiles" \
-DROOT_DIR=${ROOT_DIR} \
-DCMAKE_DIR=${CMAKE_DIR} \
-DMR_LIB=${TMP_DIR}

make -j4

# ============= Client编译 ============= #
g++ -shared -fPIC -o "${TEST_TMP_DIR}/MapReduce.so" -I"${ROOT_DIR}/Client/include" "${ROOT_DIR}/Client/source/MapReduce.cpp" 