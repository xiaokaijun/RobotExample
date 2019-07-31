#指定傲博依赖库所在的目录
set(AUBO_ROOT /home/kaijun/devtool/robotdependents/libs)


# include 路径
set(PROJECT_INCLUDE_DIRS
        ${AUBO_ROOT}/libconfig/linux_x64/inc;
        ${AUBO_ROOT}/log4cplus/linux_x64/inc;
        ${AUBO_ROOT}/protobuf/linux_x64/google/protobuf/inc;
        ${AUBO_ROOT}/robotController/Header;
        ${AUBO_ROOT}/robotSDK/inc;
        )

include_directories(${PROJECT_INCLUDE_DIRS})


# 设置查找的库组件
SET(PROJECT_LIB_COMPONENTS config;log4cplus;protobuf;protobuf-lite;protoc;auborobotcontroller)

FOREACH(aubocomponent ${PROJECT_LIB_COMPONENTS})
    find_library(lib_${aubocomponent} NAMES ${aubocomponent} PATHS
            ${AUBO_ROOT}/protobuf/linux-x64/lib
            ${AUBO_ROOT}/libconfig/linux_x64/lib
            ${AUBO_ROOT}/log4cplus/linux_x64/lib
            ${AUBO_ROOT}/robotController/linux_x64
            ${AUBO_ROOT}/robotSDK/lib/linux_x64
            NO_DEFAULT_PATH)
    set(AUBO_LIBRARIES ${AUBO_LIBRARIES};${lib_${aubocomponent}})
ENDFOREACH()

MESSAGE("AUBO_LIBRARIES====" ${AUBO_LIBRARIES})