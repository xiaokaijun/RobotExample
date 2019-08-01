//
// Created by kaijun on 7/20/19.
//
#ifndef AUBOIUTILR_05_DetectSquare_H
#define AUBOIUTILR_05_DetectSquare_H
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
/**
 * 小方块识别案例
*/
/**
 * 小方块颜色枚举类型 : 只会检测 红绿紫 三种颜色的小方块
 */
typedef enum{
    RED = 0,
    GREEN = 1,
    PURPLE = 2
}EnumColor;

struct Square{
    Rect rect;
    Point2d center;
    EnumColor color;
};


/**
 * 计算三个点之间的cos值
 * @param p0
 * @param p1
 * @param p2
 * @return
 */
double angle_cos(Point p0,Point p1,Point p2){
    Mat m0(p0);
    Mat m1(p1);
    Mat m2(p2);

    Mat d1 = m0-m1;
    Mat d2 = m2-m1;

    double result = d1.dot(d2)/sqrt(d1.dot(d1)*(d2.dot(d2)));

//    cout<<"result:"<<result<< " d1 "<< d1.size << endl;
    return abs(result);
}
/**
 * 计算最大的cos值
 * @param points
 * @return
 */
double calculateMaxCos(vector<Point> points){
    double maxCos=0;
    for (int i = 0; i < points.size(); ++i) {
        Point p0 = points[i];
        Point p1 = points[(i+1)%4];
        Point p2 = points[(i+2)%4];

        double cos = angle_cos(p0,p1,p2);
        if(cos>maxCos){
            maxCos=cos;
        }
    }
    return maxCos;
}

/**
 * 判定颜色是否在某个范围内
 * @param roi0    图片
 * @param lowerb  颜色最小值
 * @param upperb  颜色最大值
 * @return   是否在范围内
 */
bool detectColor(const Mat &roi0, const Scalar &lowerb, const Scalar &upperb) {
    Mat binaryResult;
    inRange(roi0, lowerb, upperb, binaryResult);
    uchar centerColor = binaryResult.at<uchar>(int(binaryResult.rows / 2), int(binaryResult.cols / 2));
    if (centerColor == 255) {

        return true;
    }
    return false;
}
/**
 * 判断方块的颜色
 */
EnumColor detectColor(const vector<Point> &c0, Mat &roi) {

    Rect rect0 = boundingRect(c0);
    rectangle(roi, rect0, Scalar(0, 0, 255), 2);

    Mat roi_hsv;
    cvtColor(roi, roi_hsv, COLOR_BGR2HSV);
    // 截取出小方块
    Mat roi0 = roi_hsv(rect0);

    // 紫色判定
    if(detectColor(roi0, Scalar(125, 43, 46), Scalar(155, 255, 255))){
        cout << "颜色判断成功: 紫色"<< endl;
        return EnumColor::PURPLE;
    }
    // 绿色判定
    if(detectColor(roi0, Scalar(35, 43, 46), Scalar(77, 255, 255))){
        cout << "颜色判断成功: 绿色"<< endl;
        return EnumColor::GREEN;
    }
    // 红色判定
    if(detectColor(roi0, Scalar(0, 43, 40), Scalar(10, 255, 255)) || detectColor(roi0, Scalar(156, 43, 40), Scalar(180, 255, 255))){
        cout << "颜色判断成功: 红色"<< endl;
        return EnumColor::RED;
    }

}

void findSquareContours(const Mat &roi,vector<vector<Point>> &squareContours) {// 对图像进行高斯滤波
    Mat gaussianImg;
    GaussianBlur(roi, gaussianImg, Size(5, 5), 0);

    // 获取颜色的三通道
    vector<Mat> channels;
    split(gaussianImg, channels);

    // 在一个通道上面进行查找可能会失败，所以需要三通道
    for (int i = 0; i < 3; ++i) {
        // 取出其中某个通道的值
        Mat gray = channels[i];
        // 对图像进行处理
        Mat binary;
        threshold(gray, binary, 100, 255, THRESH_BINARY);
        // 查找轮廓
        vector<vector<Point>> contours;
        vector<Vec4i> hierachy;
        // 查找轮廓
        findContours(binary, contours, hierachy, RETR_LIST, CHAIN_APPROX_SIMPLE);

        for (int j = 0; j < contours.size(); ++j) {
            vector<Point> contour = contours[j];
            // 弧长
            double arclength = arcLength(contour, true);
            // 多边形你和
            vector<Point> newcontour;
            approxPolyDP(contour, newcontour, 0.02 * arclength, true);

            if (newcontour.size() == 4 && contourArea(newcontour) > 800 && isContourConvex(newcontour)) {
                // 计算cos值  cos90=0
                double maxCos = calculateMaxCos(newcontour);
                if (maxCos < 0.1) {


                    // 由于是在3通道中找，所以会出现重复的方块
                    Rect rect = boundingRect(newcontour);
                    cout << "找到了一个矩形：" << rect << endl;
                    bool isExist = false;
                    for (int k = 0; k < squareContours.size(); ++k) {
                        Rect oldRect = boundingRect(squareContours[k]);
                        if (abs(oldRect.x - rect.x) < 10 && abs(oldRect.y - rect.y) < 10) {
                            cout << "已经保存了这个轮廓" << endl;
                            isExist = true;
                            break;
                        }
                    }
                    // 将查找到矩形轮廓保存起来
                    if(!isExist){
                        squareContours.push_back(newcontour);
                    }

                }
            }
        }
    }


}




void findSquare(Mat &src, vector<Square> &squares) {// 定义截取的位置
    int x = 700;
    int y = 250;
    int w = 700;
    int h = 500;
    Rect rect(x, y, w, h);
    // ROI截取
    Mat roi = src(rect);
//    imshow("roi", roi);

    // 用于保存查找出来满足条件的轮廓
    vector<vector<Point>> squareContours;

    findSquareContours(roi, squareContours);// 判断轮廓的颜色类型
    for (int i = 0; i < squareContours.size(); ++i) {
        vector<Point> c0 = squareContours[i];
        EnumColor colorIndex = detectColor(c0, roi);

        Rect rect = boundingRect(c0);
        // 保存找到的图形，注意坐标要对应原图中
        Square square;
        square.rect = Rect(x + rect.x, y + rect.y, rect.width, rect.height);
        square.color = colorIndex;
        square.center = Point2i(int(square.rect.x + rect.width/2),int(square.rect.y+rect.height/2));

        squares.push_back(square);
    }

//    imshow("roi", roi);


}



#endif

