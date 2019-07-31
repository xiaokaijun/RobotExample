cmake_minimum_required(VERSION 3.14)
project(AuboInterfaceExample)

set(CMAKE_CXX_STANDARD 11)

set(CAKE_CXX_FLAGS  " g3 -Wall" )


# 设置example的路径
set(EXAMPLE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/example)

# 设置findpackage查找工程的目录
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
find_package(Aubo REQUIRED)

## 设置以来的路径
#set(DEPENDENTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependents)
#
## include 路径
#set(PROJECT_INCLUDE_DIRS
#        ${DEPENDENTS_DIR}/libconfig/linux_x64/inc;
#        ${DEPENDENTS_DIR}/log4cplus/linux_x64/inc;
#        ${DEPENDENTS_DIR}/protobuf/linux_x64/google/protobuf/inc;
#        ${DEPENDENTS_DIR}/robotController/Header;
#        ${DEPENDENTS_DIR}/robotSDK/inc;
#        ${EXAMPLE_DIR}
#        )
#
#include_directories(${PROJECT_INCLUDE_DIRS})
#
#
## 设置查找的库组件
#SET(PROJECT_LIB_COMPONENTS config;log4cplus;protobuf;protobuf-lite;protoc;auborobotcontroller)
#
#FOREACH(aubocomponent ${PROJECT_LIB_COMPONENTS})
#    find_library(lib_${aubocomponent} NAMES ${aubocomponent} PATHS
#            ${DEPENDENTS_DIR}/protobuf/linux-x64/lib
#            ${DEPENDENTS_DIR}/libconfig/linux_x64/lib
#            ${DEPENDENTS_DIR}/log4cplus/linux_x64/lib
#            ${DEPENDENTS_DIR}/robotController/linux_x64
#            ${DEPENDENTS_DIR}/robotSDK/lib/linux_x64
#            ${EXAMPLE_DIR}
#            NO_DEFAULT_PATH)
#    set(PROJECT_LIBRARIES ${PROJECT_LIBRARIES};${lib_${aubocomponent}})
#ENDFOREACH()
#


# 指定example文件夹的源码位置
aux_source_directory(example DIR_SRCS)
include_directories(example)
# 让example文件夹下的源文件和main.cpp一起编译
add_executable(AuboInterfaceExample main.cpp ${DIR_SRCS})


#link_directories(${DEPENDENTS_DIR}/robotSDK/lib/linux_x64)

# 指定链接库..
target_link_libraries(AuboInterfaceExample ${AUBO_LIBRARIES})


#add_executable(DrawCircle  DrawCircle.cpp)
#target_link_libraries(DrawCircle ${PROJECT_LIBRARIES})

#
#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin" CACHE PATH "Runtime directory" FORCE)
#SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/libs" CACHE PATH "Library directory" FORCE)
#SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/libs" CACHE PATH "Archive directory" FORCE)
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/dependents/qt)
#
#file(
#        WRITE
#        ${CMAKE_CURRENT_SOURCE_DIR}/build/bin/qt.conf
#        "[Paths]
#Plugins=${CMAKE_CURRENT_SOURCE_DIR}/dependents/qt/plugins"
#)
#
#
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)
#
#
#find_package(Qt5Core REQUIRED)
#find_package(Qt5Gui REQUIRED)
#find_package(Qt5Widgets REQUIRED)
#find_package(Qt5Network REQUIRED)
#
#SET(QT_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Widgets_LIBRARIES})
#
#
## 头文件和链接库的路径
#include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS}
#        ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} )

# 导入QT相关的依赖
find_package(Qt REQUIRED)

# 引入Eigen3相关的库
include_directories("/usr/include/eigen3")




# 引入opencv相关以来
#set(PREBUILT_DIR ${CMAKE_SOURCE_DIR}/dependents)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
#message(STATUS "test:${CMAKE_MODULE_PATH}")
find_package(OpenCV REQUIRED)
#include_directories(${OpenCV_INCLUDE_DIRS})

#find_package(PCL REQUIRED)

# 引入kenect2相关的依赖
#set(FREENECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/denpendents/include)
#set(FREENECT_LIBRARIES ${PROJECT_SOURCE_DIR}/denpendents/lib)
#
#include_directories(${FREENECT_INCLUDE_DIRS})
#link_directories(${FREENECT_LIBRARIES})
FIND_PACKAGE(Freenect REQUIRED)


# 引入robwork相关的依赖
#set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};/home/kaijun/workspace/RobWork/RobWork/cmake")
#set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};/home/kaijun/workspace/RobWork/RobWork/build")
#message("CMAKE_MODULE_PATH:${CMAKE_MODULE_PATH}")
#
#set(CMAKE_PREFIX_PATH "/home/kaijun/workspace/RobWork/RobWork/build" )


FIND_PACKAGE(RobWork REQUIRED)
message("ROBWORK_LIBRARIES:${ROBWORK_LIBRARIES}")







add_subdirectory(src)
