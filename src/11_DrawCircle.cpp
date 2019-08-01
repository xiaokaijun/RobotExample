//
// Created by kaijun on 7/31/19.
//
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>
#include <utils/RPYUtils.h>
#include "Robot.h"
/**
 * 画圆: 保持末端xyz坐标值不变,通过调整姿态进行画圆
 */
using namespace std;

Robot robot;

void drawCircle(){
    // 获取当前tcp的坐标
    const std::vector<double> &tcp = {0.07,-0.32,0.175};
    // 工具的长度
    double T = 0.180;
    // 根据工具的长途和Z的值计算圆的半径
    double R = sqrt(pow(T,2) - pow(tcp[2],2));
    // 轨迹的偏移量
    double i = M_PI/12;
    for (double alpha = 0; alpha< 2*M_PI ; alpha+=i) {

        std::vector<double> norm{R*cos(alpha),R*sin(alpha),-tcp[2]};

        std::vector<double> result = calcRPY(norm);
        cout<<"result0:"<<result[0]<<endl;
        cout<<"result1:"<<result[1]<<endl;
        cout<<"result2:"<<result[2]<<endl;
        cout<<"==============="<<endl;
        usleep(1000*1000);
        std::vector<double> poseTarget{tcp[0], tcp[1], tcp[2], result[2], result[1], result[0]};

        // 这里加上的0.033只是为了让机械臂不撞上桌面
        double xyz[]={tcp[0], tcp[1], tcp[2]+0.033};
        double rpy[]={result[2], result[1], result[0]};
        robot.movel(xyz,rpy);
    }
}


int main(int argc,char** argv){
    QApplication app(argc,argv);

    QWidget widget;
    widget.setWindowTitle("画圆案例");

    QVBoxLayout boxlayout(&widget);

    QPushButton btn("开始画圆",&widget);
    QObject::connect(&btn,&QPushButton::clicked,drawCircle);

    boxlayout.addWidget(&btn);
    widget.show();


    return app.exec();
}