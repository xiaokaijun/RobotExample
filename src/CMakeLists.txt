aux_source_directory(. SOURCE)
aux_source_directory(paw PAW_SOURCE)
aux_source_directory(utils UTILS_SOURCE)

add_executable(TestRobot 01_TestRobotSDK.cpp Robot.h Robot.cpp ${PAW_SOURCE})
target_link_libraries(TestRobot ${QT_LIBRARIES} ${AUBO_LIBRARIES})

add_executable(TestDoubleHand 02_TestDoubleRobot.cpp Robot.h Robot.cpp ${PAW_SOURCE} )
target_link_libraries(TestDoubleHand ${QT_LIBRARIES} ${AUBO_LIBRARIES})

# 测试相机坐标系转机械臂基坐标系
add_executable(03_TestCamera2Base 03_TestCamera2Base.cpp ${UTILS_SOURCE} )
target_link_libraries(03_TestCamera2Base  ${QT_LIBRARIES} ${OpenCV_LIBRARIES} ${AUBO_LIBRARIES})



add_executable(04_VerifyCalibration 04_VerifyCalibration.cpp 03_TestCamera2Base.cpp Robot.cpp Robot.h  ${PAW_SOURCE})
target_link_libraries(04_VerifyCalibration  libfreenect2.so ${OpenCV_LIBRARIES}  ${QT_LIBRARIES} ${AUBO_LIBRARIES})

# 小方块识别案例
add_executable(05_DetectSquare 05_DetectSquare.cpp ${UTILS_SOURCE}  )
target_link_libraries(05_DetectSquare  ${FREENECT_LIBRARIES} ${OpenCV_LIBRARIES}  ${QT_LIBRARIES} ${AUBO_LIBRARIES})


## 自动抓取案例
add_executable(06_AutoGrab 06_AutoGrab.cpp Robot.cpp Robot.h  ${UTILS_SOURCE} ${PAW_SOURCE})
target_link_libraries(06_AutoGrab   ${FREENECT_LIBRARIES}  ${OpenCV_LIBRARIES}  ${QT_LIBRARIES} ${AUBO_LIBRARIES})


## 自动抓取案例
add_executable(07_test_paw 07_test_paw.cpp ${PAW_SOURCE})
target_link_libraries(07_test_paw  libfreenect2.so ${OpenCV_LIBRARIES}  ${QT_LIBRARIES} ${AUBO_LIBRARIES})

# 自动写字
add_executable(08_Painter 08_Painter.cpp  Robot.cpp Robot.h ${PAW_SOURCE} )
target_link_libraries(08_Painter  libfreenect2.so ${OpenCV_LIBRARIES}  ${QT_LIBRARIES} ${AUBO_LIBRARIES})



## 测试RobWork依赖的引入
add_executable(10_RobWork utils/RPYUtils.h 10_RobWork.cpp ${UTILS_SOURCE} Robot.cpp Robot.h  ${PAW_SOURCE})
target_link_libraries(10_RobWork  libfreenect2.so ${OpenCV_LIBRARIES}  ${QT_LIBRARIES} ${AUBO_LIBRARIES} ${ROBWORK_LIBRARIES})


## 画圆的案例
add_executable(11_DrawCircle utils/RPYUtils.h 11_DrawCircle.cpp Robot.cpp Robot.h ${PAW_SOURCE})
target_link_libraries(11_DrawCircle  libfreenect2.so ${QT_LIBRARIES} ${AUBO_LIBRARIES} ${ROBWORK_LIBRARIES})


## 自动实时抓取案例
add_executable(12_AutoGrab 12_AutoGrab.cpp ${UTILS_SOURCE} Robot.cpp Robot.h  ${PAW_SOURCE})
target_link_libraries(12_AutoGrab  libfreenect2.so ${OpenCV_LIBRARIES}  ${QT_LIBRARIES} ${AUBO_LIBRARIES})