#指定libfreenect2库所在的目录
set(FREENECT_ROOT /home/kaijun/devtool/robotdependents/libs/libfreenect2)


# 引入kenect2相关的依赖
set(FREENECT_INCLUDE_DIRS ${FREENECT_ROOT}/include)
set(FREENECT_LIBRARIES ${FREENECT_ROOT}/lib)

include_directories(${FREENECT_INCLUDE_DIRS})

FIND_LIBRARY(libfreenect NAMES freenect2 PATHS ${FREENECT_ROOT}/lib NO_DEFAULT_PATH)
set(FREENECT_LIBRARIES ${libfreenect})
message(AAA ${FREENECT_LIBRARIES})