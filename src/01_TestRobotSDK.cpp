#include "Robot.h"
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>
#include <QDebug>

#include <zconf.h>

#define DE2RA M_PI/180

// 姿态1:
double joint1[] = {18.819843*DE2RA, -17.610569*DE2RA, -109.448857*DE2RA,-3.861306*DE2RA, -88.872687*DE2RA, -160.007877*DE2RA};
double point1[] = {-0.227602, -0.205245, 0.312001,-178.483566*DE2RA, 1.75*DE2RA, 88.870956*DE2RA};
//
//// 姿态2:  从点1的位置到点2的位置，逆解的时候会出现大翻转
double joint2[] = {87.401334*DE2RA, -9.414021*DE2RA, -106.291800*DE2RA,-11.284257*DE2RA, -88.872687*DE2RA, -160.009120*DE2RA};
double point2[] = {0.106190, -0.325713, 0.302872,-176.938768*DE2RA, 2.561946*DE2RA, 157.537857*DE2RA};


// 创建一个机器人对象
Robot robot;

//void testOpen(){
//    robot.openHand();
//}

void doPutUp(){
    //1. 移动到抓盒子的点的上方
    robot.movej(joint1);
    //2. 打开机械抓
    robot.openHand();
    sleep(1);
    //3. 向下移动到物品
    double downPos[aubo_robot_namespace::ARM_DOF];
    memcpy(downPos,point1,aubo_robot_namespace::ARM_DOF);
    downPos[2] -= 0.05;
    robot.movel(downPos);

    //4. 关闭机械抓
    robot.closeHand();
    sleep(1);
    //5. 返回到盒子上方的位置
    robot.movel(point1);
}

void doPutDown(){
    //1. 移动到放物品的点的上方
    robot.movej(joint2);
    //2. 向下移动到放物品的地方
    double downPos[aubo_robot_namespace::ARM_DOF];
    memcpy(downPos,point2,aubo_robot_namespace::ARM_DOF);
    downPos[2] -= 0.05;
    robot.movel(point2);
    //3. 打开机械抓。等待1s
    robot.openHand();
    sleep(1);
    //4. 向上移动到放物品的点
    robot.movel(point2);
    //5. 关闭机械抓
    robot.closeHand();
    sleep(1);
}



int main(int argc,char **argv){
    // 创建Qt应用
    QApplication app(argc,argv);
    // 创建QT窗口
    QWidget widget;
    widget.setFixedWidth(300);
    // 创建布局
    QVBoxLayout layout;
    // 设置窗口布局为layout
    widget.setLayout(&layout);

    //打开机械爪
    QPushButton openHand0("连续操机械抓");
    layout.addWidget(&openHand0);
    QObject::connect(&openHand0,&QPushButton::clicked,[&](){
        for (int i = 0; i < 5; ++i) {
            robot.openHand();
            sleep(1);
            robot.closeHand();
            sleep(1);
        }

    });
    //打开机械爪
    QPushButton openHand("打开机械爪");
    layout.addWidget(&openHand);
    QObject::connect(&openHand,&QPushButton::clicked,[&](){robot.openHand();});

    //关闭机械爪
    QPushButton closeHand("关闭机械爪");
    layout.addWidget(&closeHand);
    QObject::connect(&closeHand,&QPushButton::clicked,[&](){robot.closeHand();});

    //抓取的按钮
    QPushButton putUpBtn("抓取盒子");
    layout.addWidget(&putUpBtn);
    QObject::connect(&putUpBtn,&QPushButton::clicked,doPutUp);

    //放下的按钮
    QPushButton putDownBtn("放下盒子");
    layout.addWidget(&putDownBtn);
    QObject::connect(&putDownBtn,&QPushButton::clicked,doPutDown);

    // 将窗口显示出来
    widget.show();

    int exec_code = app.exec();

    return exec_code;
}