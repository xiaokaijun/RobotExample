#ifndef AUBOIUTILR_ROBOT_H
#define AUBOIUTILR_ROBOT_H

#include <iostream>
#include <cmath>

// Eigen 部分
#include <Eigen/Core>
// 稠密矩阵的代数运算（逆，特征值等）
#include <Eigen/Dense>
//Eigen 几何模块
#include <Eigen/Geometry>

#include <opencv2/opencv.hpp>

/**
 * 本文件包含:
 *  1.像素坐标系转相机坐标系
 *  2.相机坐标系转Base坐标系
 */

using namespace std;
using namespace Eigen;
using namespace cv;

// 内参文件路径
static cv::String inCailFilePath="./assets/3DCameraInCailResult.xml";
// 外参文件路径
static cv::String exCailFilePath="./assets/3DCameraExCailResult.xml";


/**
 * 将行列坐标，转成相机坐标系中的x,y,z
 * @param piexld 输入 {列, 行, 深度}
 * @param cameraPos 输出 {x,y,z}
 */
static void piexl2camera(double *piexld,double *cameraPos){
    Mat cameraMatrix=cv::Mat_<double>(3, 3);;
    // 读取内参
    FileStorage paramFs(inCailFilePath,FileStorage::READ);
    paramFs["cameraMatrix"]>>cameraMatrix;

    cout<<"图像平面：行："<<piexld[1]<<"  列："<<piexld[0]<<endl;
    // 行
    double v = piexld[1];
    // 列
    double u = piexld[0];
    double d = piexld[2];
    // 内参数据
    double camera_fx = cameraMatrix.at<double>(0,0);
    double camera_fy = cameraMatrix.at<double>(1,1);
    double camera_cx = cameraMatrix.at<double>(0,2);
    double camera_cy = cameraMatrix.at<double>(1,2);
    // 计算相机坐标系中的值
    double z = double(d)/1000; //单位是米
    double x = (u - camera_cx) * z / camera_fx;
    double y = (v - camera_cy) * z / camera_fy;

    cameraPos[0]=-x;
    cameraPos[1]=y;
    cameraPos[2]=z;

    cout<<"ub空间：x:"<<x<<" y:"<<y<<" d:"<<z<<endl;
    cout<<"相机坐标系：x:"<<cameraPos[0]<<" y:"<<cameraPos[1]<<" z:"<<cameraPos[2]<<endl;
}


/**
 * @param cameraPos 传入相机坐标系中的值
 * @param basePos 输出机器人Base坐标系中的值
 */
static void camera2base(double *cameraPos, double *basePos) {
    double angle = 0;
    double axisX = 0;
    double axisY = 0;
    double axisZ = 0;
    double translationX = 0;
    double translationY = 0;
    double translationZ = 0;

    cv::FileStorage fs(exCailFilePath, cv::FileStorage::READ);
    fs["Angle"] >> angle;
    fs["AxisX"] >> axisX;
    fs["AxisY"] >> axisY;
    fs["AxisZ"] >> axisZ;
    fs["TranslationX"] >> translationX;
    fs["TranslationY"] >> translationY;
    fs["TranslationZ"] >> translationZ;
    // 轴角对
    Vector3d axisMatrix(axisX, axisY, axisZ);
    AngleAxisd angleAxisd(angle, axisMatrix);
    // 获取旋转矩阵
    Matrix3d ratationMatrix = angleAxisd.toRotationMatrix();
    cout << "旋转矩阵：" << angleAxisd.toRotationMatrix() << endl;
    // 获取平移矩阵
    Vector3d translationMatrix(translationX, translationY, translationZ);

    // 获取相机拍摄到的坐标
    Vector3d cameraPosition(cameraPos[0], cameraPos[1], cameraPos[2]);

    // 获取输出结果
    cout << "旋转之后的结果:" << ratationMatrix * cameraPosition << endl;
    cout << "平移向量:" << translationMatrix / 1000 << endl;

    //进行转换                    3*3             3*1      +  平移矩阵
    Vector3d resultBase = ratationMatrix * cameraPosition + translationMatrix / 1000;

    cout<<"最终结果："<<resultBase<<endl;

    // 输出结果
//    memcpy(basePos,resultBase.data(), sizeof(double)*3);
    basePos[0]=resultBase[0];
    basePos[1]=resultBase[1];
    basePos[2]=resultBase[2];
}



//int main(int argc, char **argv)
//{
//
//    double cameraPos[3]={0.01,0.01,0.9};
//    double basePos[3]={0};
//    camera2base(cameraPos, basePos);
//
//
//    return 0;
//}

#endif