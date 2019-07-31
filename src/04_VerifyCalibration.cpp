
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
#include "Robot.h"

#include "utils/03_Camera2Base.h"
/*
  验证手眼标定
*/
using namespace std;
using namespace cv;

const Size patternSize(7,5);
const int squareSize=30;

bool findCorners(Mat &img,vector<Point2f> &corners) {
    Mat gray;
    // 将图片转成灰度图像
    cvtColor(img, gray, COLOR_BGR2GRAY);
    // 查找所有的角点
    bool patternfound = findChessboardCorners(gray, patternSize, corners);

    if (patternfound) {
        // 提高角点的精确度
        cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.1));
        // 将所有的焦点在原图中绘制出来
        drawChessboardCorners(img, patternSize, corners, patternfound);
        // 绘制完角点之后，显示原图

        circle(img,corners[0],5,Scalar(0,0,255),2);

        imshow("src", img);
        return true;
    }else{
        cout<<"角点检测失败！"<<endl;
    }


    return false;
}

Robot robot;
void verifyCalibration(vector<Point2f> corners,Mat depth){
    for (int i = 0; i < corners.size(); ++i) {
        Point2f point1 = corners[i];

        float d = depth.at<float32>(int(point1.y),int(point1.x));
        if(d==0){
            cout<<"行:"<<int(point1.y)<<"列："<<int(point1.x)<<endl;
            continue;        }

           double p[3] = {point1.x,point1.y,d};
        double cameraPos[3]={0};
        piexl2camera(p,cameraPos);

        cout<<"相机坐标系：x:"<<cameraPos[0]<<" y:"<<cameraPos[1]<<" z:"<<cameraPos[2]<<endl;

        double basePos[3]={0};
        camera2base(cameraPos,basePos);

        cout<<"Base坐标系：x:"<<basePos[0]<<" y:"<<basePos[1]<<" z:"<<basePos[2]<<endl;

        double result[6]={basePos[0]+0.01,basePos[1],basePos[2]+0.15,180*DE2RA,0,-90*DE2RA};
        robot.movel(result);
    }


}


int main(int argc, char** argv) {
    //! [context]
    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device *dev = nullptr;
    libfreenect2::PacketPipeline *pipeline = nullptr;
    //! [context]
    //! [discovery]
    if(freenect2.enumerateDevices() == 0)
    {
        std::cout << "no device connected!" << std::endl;
        return -1;
    }
    std::string serial = freenect2.getDefaultDeviceSerialNumber();
    if(serial == "")  return -1;
    cout<<"The serial number is :"<<serial<<endl;
    //! [discovery]

    pipeline = new libfreenect2::OpenGLPacketPipeline();

    dev = freenect2.openDevice(serial, pipeline);


    //! [listeners]
    libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color |libfreenect2::Frame::Depth);
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
    cv::String inCailFilePath="./assets/3DCameraInCailResult.xml";
    FileStorage paramFs(inCailFilePath,FileStorage::READ);
    Mat cameraMatrix=cv::Mat_<double>(3, 3);
    paramFs["cameraMatrix"]>>cameraMatrix;
    double camera_fx = cameraMatrix.at<double>(0,0);
    double camera_fy = cameraMatrix.at<double>(1,1);
    double camera_cx = cameraMatrix.at<double>(0,2);
    double camera_cy = cameraMatrix.at<double>(1,2);



    libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());

    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), bigdepth(1920, 1082, 4);



    int keyCode=0;

//    while((keyCode=waitKey(100))!=27) {
        listener.waitForNewFrame(frames);
        libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];

        registration->apply(rgb, depth, &undistorted, &registered,true,&bigdepth);

        Mat rgbmat = Mat(rgb->height, rgb->width, CV_8UC4, rgb->data);
        Mat deptmat = Mat(depth->height, depth->width, CV_32FC1, depth->data);
//        Mat undistortedmat = Mat(undistorted.height, undistorted.width, CV_8UC4, undistorted.data);
//        Mat registeredmat = Mat(registered.height, registered.width, CV_8UC4, registered.data);
        Mat bigdepthmat = Mat(bigdepth.height, bigdepth.width, CV_32FC1 , bigdepth.data);




        for (int row = 0; row < bigdepthmat.rows; ++row) {
            for (int col = 0; col < bigdepthmat.cols; ++col) {
                float32 d = bigdepthmat.at<float32>(row, col);

                if(fpclassify(d) == FP_INFINITE || fpclassify(d) == NAN){
                    bigdepthmat.at<float32>(row, col)=0;
                }

            }
        }


        imwrite("./assets/color0.jpg",rgbmat);
//
//        FileStorage fs("./assets/depth1.xml",FileStorage::WRITE);
//        fs<<"depth"<<deptmat;
//        fs.release();
//
        FileStorage fs2("./assets/depth2.xml",FileStorage::WRITE);
        fs2<<"depth"<<bigdepthmat;
        fs2.release();
//
        listener.release(frames);
//
        vector<Point2f> corners ;
        findCorners(rgbmat,corners);

        verifyCalibration(corners,bigdepthmat);

//        imshow("src",rgbmat);
//        imshow("bigdepthmat",bigdepthmat);
//


//    }

    waitKey(0);
    dev->stop();
    dev->close();

    return 0;
}