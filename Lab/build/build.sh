BUILD_DIR=`pwd`
ROOT_DIR=`dirname ${BUILD_DIR}`
CMAKE_DIR=${BUILD_DIR}
TMP_DIR=${BUILD_DIR}/tmp

cmake ${CMAKE_DIR} \
-G "Unix Makefiles" \
-DROOT_DIR=${ROOT_DIR} \
-DCMAKE_DIR=${CMAKE_DIR} \
-B ${TMP_DIR}

cd ${TMP_DIR}
make -j4