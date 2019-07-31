
# once done, this will define
#OpenCV_FOUND        -  whether there is OpenCV in the prebuilt libraries
#OpenCV_INCLUDE_DIRS -  include directories for OpenCV
#OpenCV_LIBRARY_DIRS -  library directories for OpenCV
#OpenCV_LIBRARIES    -  link this to use OpenCV

unset(OpenCV_FOUND)

set(PREBUILT_DIR /home/kaijun/devtool/robotdependents/libs)


set(OpenCV_DIR ${PREBUILT_DIR}/opencv-3.4.1)

MESSAGE(STATUS "TEST  ${PREBUILT_DIR}")

set(OpenCV_INCLUDE_DIRS ${OpenCV_DIR}/include)
SET(OpenCV_LIB_COMPONENTS opencv_videostab;opencv_video;opencv_superres;opencv_stitching;opencv_photo;
        opencv_objdetect;opencv_ml;opencv_imgproc;opencv_highgui;opencv_flann;opencv_features2d;
        opencv_core;opencv_calib3d;opencv_xfeatures2d;opencv_imgcodecs)
find_path(OpenCV_INCLUDE_DIRS NAMES opencv.h HINTS ${OpenCV_DIR}/include NO_SYSTEM_ENVIRONMENT_PATH)

set(OpenCV_LIBRARY_DIRS ${OpenCV_DIR}/lib)

FOREACH(cvcomponent ${OpenCV_LIB_COMPONENTS})
  find_library(lib_${cvcomponent} NAMES ${cvcomponent} HINTS ${OpenCV_DIR}/lib NO_DEFAULT_PATH)

  set(OpenCV_LIBRARIES ${OpenCV_LIBRARIES};${lib_${cvcomponent}})
ENDFOREACH()

set(OpenCV_LIBS ${OpenCV_LIBRARIES})

set(OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS};${OpenCV_INCLUDE_DIRS}/opencv)

if (OpenCV_INCLUDE_DIRS AND OpenCV_LIBRARIES)
  set(OpenCV_FOUND TRUE)
endif (OpenCV_INCLUDE_DIRS AND OpenCV_LIBRARIES)

if (OpenCV_FOUND)
  if (NOT OpenCV_FIND_QUIETLY)
    message(STATUS "Found OpenCV: ${OpenCV_LIBRARIES}")
  endif (NOT OpenCV_FIND_QUIETLY)
else(OpenCV_FOUND)
  if (OpenCV_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find the OpenCV library")
  endif ()
endif (OpenCV_FOUND)

# 导入opencv相关的头文件
include_directories(${OpenCV_INCLUDE_DIRS})