BUILD_DIR=`pwd`
ROOT_DIR=`dirname ${BUILD_DIR}`
SRC_DIR=${ROOT_DIR}/src
CMAKE_DIR=${BUILD_DIR}
TMP_DIR=${BUILD_DIR}/tmp

mkdir -p ${TMP_DIR}
cd ${TMP_DIR}

cmake ${CMAKE_DIR} \
-G "Unix Makefiles" \
-DSRC_DIR=${SRC_DIR} \
-DBUILD_DIR=${BUILD_DIR}

make -j4
make install