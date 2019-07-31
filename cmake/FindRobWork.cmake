set(ROOT_RW_DIR "/home/kaijun/devtool/robotdependents/libs/robwork")



include_directories(${ROOT_RW_DIR}/src)


# 设置查找的库组件
SET(ROBWORK_LIB_COMPONENTS rw_algorithms;
                            rw_pathplanners;
                            rw_pathoptimization;
                            rw_simulation;
                            rw_opengl;
                            rw_task;
                            rw_calibration;
                            rw_csg;
                            rw_control;
                            rw_proximitystrategies;
                            yaobi;
                            pqp;
                            fcl;
                            rw;
                            boost_system;
                            rw_assembly)

FOREACH(component_name ${ROBWORK_LIB_COMPONENTS})
    find_library(lib${component_name} NAMES ${component_name} PATHS
            ${ROOT_RW_DIR}/libs
            /usr/lib/x86_64-linux-gnu/
            NO_DEFAULT_PATH)
    set(ROBWORK_LIBRARIES ${ROBWORK_LIBRARIES};${lib${component_name}})
ENDFOREACH()

MESSAGE(${ROBWORK_LIBRARIES})