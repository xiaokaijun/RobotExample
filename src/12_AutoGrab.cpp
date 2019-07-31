
#include <iostream>

#include <opencv2/opencv.hpp>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#include <thread>
#include "Robot.h"
#include "utils/03_Camera2Base.h"
#include "05_DetectSquare.cpp"
/*
  自动抓取小方块
*/
using namespace std;
using namespace cv;

// 垃圾桶的位置
double rubbish[3][6] = {
        {-0.267778, -0.397378, 0.40, 175 * DE2RA, 0 * DE2RA, -90 * DE2RA},
        {-0.267778, -0.269837, 0.40, 175 * DE2RA, 0 * DE2RA, -90 * DE2RA},
        {-0.267778, -0.169321, 0.40, 175 * DE2RA, 0 * DE2RA, -90 * DE2RA},
};

// 吃屎姿态
double defaultPos[6] = {-0.307233, -0.119000, 0.421263, 175 * DE2RA, 0 * DE2RA, -90 * DE2RA};

//Robot* robot = nullptr;
Robot robot;

void grabOneBox(Mat &depth, double defaultDepth, const Square &square);

void grabOne(double *position, EnumColor colorIndex) {
    double zoffset = 0.15;
    // 1. 移动到物品上方的位置
    double upPos[6] = {0};
    memcpy(upPos, position, sizeof(double) * 6);
    upPos[2] = upPos[2] + zoffset;
    robot.movel(upPos);
    // 2. 打开机械抓
    robot.openHand();
    sleep(1);
    // 3. 向下移动到方块的位置
    robot.movel(position);
    // 4. 关闭机械抓
    robot.closeHand();
    sleep(1);
    // 5. 移动到物品上方0.08 的位置
    robot.movel(upPos);

    // 6. 移动到对应颜色的垃圾桶上方
    double *rPos = rubbish[colorIndex];
    robot.movel(rPos);
    // 7. 向下移动
//    double upRPos[6]={0};
//    memcpy(upRPos,rPos, sizeof(double)*6);
//    upRPos[2]=upRPos[2]-0.02;
//    robot->movel(upRPos);
    // 8. 打开机械抓
    robot.openHand();
    sleep(1);
    // 9. 移动到对应颜色的垃圾桶上方
//    robot->movel(upRPos);
    // 10. 关闭机械抓
//    robot->closeHand();

    // 11. 休息0.5秒
    sleep(1);
}


void grabSquareToBox(Mat depth, vector<Square> squares) {

    double defaultDepth = 0.931642;

    for (int i = 0; i < squares.size(); ++i) {
        Square &square = squares[i];

        grabOneBox(depth, defaultDepth, square);

        //        break;
    }

    // 干完之后回到初始姿态
    robot.movel(defaultPos);
}

void grabOneBox(Mat &depth, double defaultDepth, const Square &square) {
    Point2d center = square.center;
    // 获取深度信息
    float d = depth.at<float32>(int(center.y), int(center.x));

    if (d == 0) {
        cout << "深度信息丢失--->行:" << int(center.y) << "列：" << int(center.x) << endl;
        d = defaultDepth;
//            continue;
    }

    double p[3] = {center.x, center.y, d};
    double cameraPos[3] = {0};
    piexl2camera(p, cameraPos);

    cout << "相机坐标系：x:" << cameraPos[0] << " y:" << cameraPos[1] << " z:" << cameraPos[2] << endl;

    double basePos[3] = {0};
    camera2base(cameraPos, basePos);

    cout << "Base坐标系：x:" << basePos[0] << " y:" << basePos[1] << " z:" << basePos[2] << endl;

    double toolLength = 0.177;
    double x = basePos[0] + 0.01;  // 标定结果的偏移量
    double y = basePos[1];
    double z = basePos[2] + toolLength;

    if (z > 0.005) {
        cout << "z值的求解：" << z << endl;
    }

    double pos[6] = {x, y, z, 180 * DE2RA, 0, -90 * DE2RA};

    grabOne(pos, square.color);
}


int main(int argc, char **argv) {
//    robot = new Robot();
    double defaultDepth = 0.931642;
    //! [context]
    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device *dev = nullptr;
    libfreenect2::PacketPipeline *pipeline = nullptr;
    //! [context]
    //! [discovery]
    if (freenect2.enumerateDevices() == 0) {
        std::cout << "no device connected!" << std::endl;
        return -1;
    }
    std::string serial = freenect2.getDefaultDeviceSerialNumber();
    if (serial == "") return -1;
    cout << "The serial number is :" << serial << endl;
    //! [discovery]

    pipeline = new libfreenect2::OpenGLPacketPipeline();

    dev = freenect2.openDevice(serial, pipeline);


    //! [listeners]
    libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color | libfreenect2::Frame::Depth);
    libfreenect2::FrameMap frames;
    dev->setColorFrameListener(&listener);
    dev->setIrAndDepthFrameListener(&listener);
    //! [listeners]


    //! [start]
    dev->start();
    std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
    std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;
    //! [start]

    // 内参文件路径
    cv::String inCailFilePath = "./assets/3DCameraInCailResult.xml";
    FileStorage paramFs(inCailFilePath, FileStorage::READ);
    Mat cameraMatrix = cv::Mat_<double>(3, 3);
    paramFs["cameraMatrix"] >> cameraMatrix;
    double camera_fx = cameraMatrix.at<double>(0, 0);
    double camera_fy = cameraMatrix.at<double>(1, 1);
    double camera_cx = cameraMatrix.at<double>(0, 2);
    double camera_cy = cameraMatrix.at<double>(1, 2);

    libfreenect2::Registration *registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), bigdepth(1920, 1082, 4);

    int keyCode = 0;
//    while (true) {

        cout<<"执行代码"<<endl;
//        listener.waitForNewFrame(frames);
        if (!listener.waitForNewFrame(frames, 10*1000)) // 10 sconds
        {
            std::cout << "kinect相机timeout!" << std::endl;
            return -1;
        }
        libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];
        // 相机配准
        registration->apply(rgb, depth, &undistorted, &registered, true, &bigdepth);

        Mat rgbmat = Mat(rgb->height, rgb->width, CV_8UC4, rgb->data);
        Mat deptmat = Mat(depth->height, depth->width, CV_32FC1, depth->data);
        Mat bigdepthmat = Mat(bigdepth.height, bigdepth.width, CV_32FC1, bigdepth.data);


        for (int row = 0; row < bigdepthmat.rows; ++row) {
            for (int col = 0; col < bigdepthmat.cols; ++col) {
                float32 d = bigdepthmat.at<float32>(row, col);
                if (fpclassify(d) == FP_INFINITE || fpclassify(d) == NAN) {
                    bigdepthmat.at<float32>(row, col) = 0;
                }
            }
        }

        // 查找图中的小方块
        vector<Square> squares;
        findSquare(rgbmat, squares);

//        if (squares.size() == 0) {
//            break;
//        }

        // 在原图中绘制方块
        for (int j = 0; j < squares.size(); ++j) {
            Square square = squares[j];
            rectangle(rgbmat, square.rect, Scalar(0, 255, 255), 2);
        }

        imshow("srcrgb", rgbmat);
        grabOneBox(bigdepthmat, defaultDepth, squares[0]);
        listener.release(frames);

        sleep(1);
//    }

    cout<<"程序执行完成的样子"<<endl;


    waitKey(0);
    dev->stop();
    dev->close();

//    delete robot;
    return 0;
}