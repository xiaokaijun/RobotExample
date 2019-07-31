
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

/*
  画图案例
*/
using namespace std;
using namespace cv;


// 初始姿态
double defaultPos[6]={-0.307233,-0.119000,0.421263,175*DE2RA,0*DE2RA,-90*DE2RA};

//Robot* robot = nullptr;
Robot robot;


void draw(Mat& binary) {

    Mat dst(binary.rows, binary.cols, CV_8UC1);

    double defaultDepth = 800;

    double xoffset = 950;
    double yoffset = 500;

    // 固定的z值
    double constZ = 0.2125;

    // 查找轮廓
    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;
    // 查找轮廓
    findContours(binary, contours, hierachy, RETR_LIST, CHAIN_APPROX_SIMPLE);


    double lastX = 0;
    double lastY = 0;
    double lastZ = 0;

    for (int i = 0; i < contours.size(); ++i) {
        // 获得轮廓
        vector<Point> c = contours[i];
        // 遍历每一个轮廓中的每一个像素点
        for (int j = 0; j < c.size(); ++j) {
            Point2d p = c[j];
            // 将相机坐标点转成base坐标点
            double picturePos[3] = {xoffset + p.x, yoffset + p.y, defaultDepth};
            double cameraPos[3] = {0};

            // 将像素坐标系中点转成相机坐标系中的点
            piexl2camera(picturePos, cameraPos);

            // 将相机坐标系中的点转成base坐标系中的点
            double basePos[3] = {0};
            camera2base(cameraPos, basePos);

//            double toolLength = 0.177;
            double x = basePos[0] + 0.01;  // 标定结果的偏移量
            double y = basePos[1];
//            double z = basePos[2] + toolLength;

            double pos[6] = {x, y, constZ, 180 * DE2RA, 0, -90 * DE2RA};
//            cout << "机械臂将要移动到x=" << x << " y=" << y << " z=" << z << endl;
            lastX = x;
            lastY = y;
//            lastZ = z;
            robot.movel(pos);

        }

        // 每画完一个轮廓，机械臂要抬起来一点
        double uppos[6] = {lastX, lastY, constZ + 0.05, 180 * DE2RA, 0, -90 * DE2RA};
        robot.movel(uppos);
    }

}



int main(int argc, char** argv) {

    Mat src = imread("./assets/logo.png",IMREAD_GRAYSCALE);
    imshow("src",src);
    // 将原图缩小到250
    Mat src2;
    resize(src,src2, Size(),0.4,0.4);
    imshow("src2",src2);

    Mat binary;
    threshold(src2,binary,100,255,THRESH_BINARY|THRESH_OTSU);

//    imshow("binary",binary);

    draw(binary);



    return waitKey(0);
}