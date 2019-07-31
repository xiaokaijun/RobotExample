#include "Robot.h"
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>
#include <QDebug>
//#include <random>
#include <zconf.h>
#include <thread>

#define DE2RA M_PI/180

using namespace std;
// 姿态1:
double joint1[] = {18.819843*DE2RA, -17.610569*DE2RA, -109.448857*DE2RA,-3.861306*DE2RA, -88.872687*DE2RA, -160.007877*DE2RA};
double point1[] = {-0.227602, -0.205245, 0.312001,-178.483566*DE2RA, 1.75*DE2RA, 88.870956*DE2RA};
//
//// 姿态2:  从点1的位置到点2的位置，逆解的时候会出现大翻转
double joint2[] = {87.401334*DE2RA, -9.414021*DE2RA, -106.291800*DE2RA,-11.284257*DE2RA, -88.872687*DE2RA, -160.009120*DE2RA};
double point2[] = {0.106190, -0.325713, 0.302872,-176.938768*DE2RA, 2.561946*DE2RA, 157.537857*DE2RA};


// 右臂待机点
double rightStandByJoints[]={-95.951385*DE2RA,-70.584759*DE2RA,52.387425*DE2RA,-29.676421*DE2RA,133.899589*DE2RA,174.029901*DE2RA};
double rightStandByPoints[]={-0.100499,-0.444972,-0.016334,111.274101*DE2RA,24.156603*DE2RA,135.808929*DE2RA};




// 右臂下移动到瓶子位置
double rightJoint1[]={-130.533207*DE2RA,-58.556953*DE2RA,50.121663*DE2RA,22.039323*DE2RA,86.159913*DE2RA,136.317503*DE2RA};
double rightPoint1[]={-0.470703,-0.357681,0.125994,179.782471*DE2RA,-5.096696*DE2RA,-176.953720*DE2RA};



// 左臂待机点
double leftStandByJoints[]={67.199472*DE2RA,68.128173*DE2RA,-53.337565*DE2RA,1.482907*DE2RA,-71.974150*DE2RA,174.005954*DE2RA};
double leftStandByPoints[]={-0.058421,-0.521114,-0.008554,147.223282*DE2RA,18.363787*DE2RA,152.379044*DE2RA};


// 创建一个机器人对象
//Robot leftRobot;
Robot* rightRobot = new Robot("192.168.17.250",8899,"admin","1","/dev/ttyUSB0");
Robot* leftRobot = new Robot("192.168.17.251",8899,"admin","1","/dev/ttyUSB1");

// 获取随机数 0.01~0.09之间的随机数
double getRandom(){
    double v = random()%10*1.0/100;
    return v;
}

void doTest(){
    leftRobot->movej(leftStandByJoints);
    rightRobot->movej(rightStandByJoints);
}

bool standFlag = true;




void rightHand(){
    //1. 移动到要抓取水杯的位置点1
    rightRobot->movej(rightJoint1);
    //2. 调整姿态

    //3. 打开机械爪
    rightRobot->openHand();

    //4. 移动到水杯的位置
//    double joint2[]={-132.473636*DE2RA,-77.131960*DE2RA,49.483187*DE2RA,43.471070*DE2RA,89.686859*DE2RA,136.333485*DE2RA};
    double joint2[]={-125.779955*DE2RA,-72.321130*DE2RA,60.970462*DE2RA,46.974896*DE2RA,87.546564*DE2RA,140.727988*DE2RA};
    rightRobot->movej(joint2);
//    double rightPoint2[aubo_robot_namespace::ARM_DOF] = {0};
//    memcpy(rightPoint2,rightJoint1,6);
//    rightPoint2[2]=-0.023191;
//    rightRobot->movel(rightPoint2);


    //5. 关闭机械爪
}

void rightHandClose(){
    rightRobot->closeHand();
    sleep(1);
    //6. 保持当前姿态抬起机械爪
//    double joint3[]={-113.827179*DE2RA,-56.857370*DE2RA,118.371741*DE2RA,87.633779*DE2RA,89.724896*DE2RA,158.083040*DE2RA};
//    rightRobot->movej(joint3);
//
//    sleep(1);

    // 向外移动一点
//    double joint4[]={-120.287991*DE2RA,-71.269684*DE2RA,86.387522*DE2RA,79.171585*DE2RA,89.792153*DE2RA,148.444517*DE2RA};
    double joint4[]={-121.919901*DE2RA,-71.295113*DE2RA,87.380324*DE2RA,78.732137*DE2RA,91.891043*DE2RA,148.444517*DE2RA};
    rightRobot->movej(joint4);
}



void leftHand(){
    //7. 移动到瓶盖上方
    double joint4[]={61.348752*DE2RA,91.817693*DE2RA,-61.696989*DE2RA,29.160892*DE2RA,-31.649215*DE2RA,175*DE2RA};

    leftRobot->movej(joint4);
    //8. 调整姿态
    cout<<"左臂操作："<<endl;
    //9. 打开机械爪
    leftRobot->openHand();
    //10. 向下移动机械爪
    sleep(1);

    //12. 调整姿态，开始扭瓶盖
//    double newJoints[]={67.196105*DE2RA,85.900201*DE2RA,-82.755904*DE2RA,4.626307*DE2RA,-18.818792*DE2RA,174*DE2RA};
    double joint5[6]={76.707642*DE2RA,85.768379*DE2RA,-81.199024*DE2RA,3.780824*DE2RA,-12.012680*DE2RA,174*DE2RA};

    leftRobot->movej(joint5);
}

void leftHandClose(){

    //11. 关闭机械爪
    leftRobot->closeHand();

}



//double newJoints[]={67.196105*DE2RA,85.900201*DE2RA,-82.755904*DE2RA,4.626307*DE2RA,-18.818792*DE2RA,174*DE2RA};
double joint5[]={76.707642*DE2RA,85.768379*DE2RA,-81.199024*DE2RA,3.780824*DE2RA,-12.012680*DE2RA,174*DE2RA};
void leftHandOpenCap(){
    //13. 转动瓶盖  -175 - 175
    joint5[5]-=10*DE2RA;
    cout<<joint5[5]<<endl;
    leftRobot->movej(joint5);
}


void shougong(){
    double newJoints[]={-111.990145*DE2RA,-76.038481*DE2RA,18.184523*DE2RA,86.308074*DE2RA,91.949268*DE2RA,174*DE2RA};
    rightRobot->movej(newJoints);
    rightRobot->openHand();

    // 左臂 移动到垃圾1 正上方
    double laji1[]={90.167923*DE2RA,39.253316*DE2RA,-125.998316*DE2RA,2.935550*DE2RA,1.631492*DE2RA,174.009519*DE2RA};
    leftRobot->movej(laji1);

    // 向下稍微移动一点
    double laji2[]={103.767956*DE2RA,42.563587*DE2RA,-120.275806*DE2RA,15.148304*DE2RA,21.464110*DE2RA,174.009519*DE2RA};
    leftRobot->movej(laji2);

    // 扔掉瓶盖
    leftRobot->openHand();
    sleep(1);
    leftRobot->closeHand();
    // 左臂回到待机状态
    leftRobot->movej(leftStandByJoints);


}

/* 自动打开盖子*/
void autoOpenGap(){
    rightHand();
    rightHandClose();
    leftHand();
    leftHandClose();
    sleep(2);
    for (int i = 0; i < 35; ++i) {
        leftHandOpenCap();
    }

    shougong();
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

    QPushButton testBtn("回到待机状态");
    layout.addWidget(&testBtn);
    QObject::connect(&testBtn,&QPushButton::clicked,doTest);

    //抓取的按钮
    QPushButton putUpBtn("右臂准备抓取");
    layout.addWidget(&putUpBtn);
    QObject::connect(&putUpBtn,&QPushButton::clicked,rightHand);

    QPushButton closeBtn("右臂抓取");
    layout.addWidget(&closeBtn);
    QObject::connect(&closeBtn,&QPushButton::clicked,rightHandClose);





    //放下的按钮
    QPushButton putDownBtn("左臂准备扭瓶盖");
    layout.addWidget(&putDownBtn);
    QObject::connect(&putDownBtn,&QPushButton::clicked,leftHand);
  //放下的按钮
    QPushButton leftHandCloseBtn("左臂夹住瓶盖");
    layout.addWidget(&leftHandCloseBtn);
    QObject::connect(&leftHandCloseBtn,&QPushButton::clicked,leftHandClose);

     //左臂扭瓶盖
    QPushButton leftOpenBtn("扭瓶盖");
    layout.addWidget(&leftOpenBtn);
    QObject::connect(&leftOpenBtn,&QPushButton::clicked,leftHandOpenCap);

    //左臂扭瓶盖
    QPushButton shougongBtn("收工");
    layout.addWidget(&shougongBtn);
    QObject::connect(&shougongBtn,&QPushButton::clicked,shougong);

    //左臂扭瓶盖
    QPushButton autoBtn("人工智能");
    layout.addWidget(&autoBtn);
    QObject::connect(&autoBtn,&QPushButton::clicked,autoOpenGap);


    //打开机械爪
    QPushButton openHand("打开右臂机械爪");
    layout.addWidget(&openHand);
    QObject::connect(&openHand,&QPushButton::clicked,[&](){rightRobot->openHand();});

    //关闭机械爪
    QPushButton closeHand("关闭右臂机械爪");
    layout.addWidget(&closeHand);
    QObject::connect(&closeHand,&QPushButton::clicked,[&](){rightRobot->closeHand();});

    //打开机械爪
    QPushButton openLeftHand("打开左臂机械爪");
    layout.addWidget(&openLeftHand);
    QObject::connect(&openLeftHand,&QPushButton::clicked,[&](){leftRobot->openHand();});

    //关闭机械爪
    QPushButton closeLeftHand("关闭左臂臂机械爪");
    layout.addWidget(&closeLeftHand);
    QObject::connect(&closeLeftHand,&QPushButton::clicked,[&](){leftRobot->closeHand();});


    //    //右臂待机状态
    QPushButton standByBtn("测试反解");
    layout.addWidget(&standByBtn);
    QObject::connect(&standByBtn,&QPushButton::clicked,[&](){

        double joint11[6] = {18.819843*DE2RA, -17.610569*DE2RA, -109.448857*DE2RA,-3.861306*DE2RA, -88.872687*DE2RA, -160.007877*DE2RA};
        double point11[6] = {-0.227602, -0.205245, 0.312001,-178.483566*DE2RA, 1.75*DE2RA, 88.870956*DE2RA};

        double resultJoints[6]={0};
        double xyz[]={-0.227602, -0.205245, 0.312001};
        double rpy[]={-178.483566*DE2RA, 1.75*DE2RA, 88.870956*DE2RA};
        rightRobot->IK(xyz,rpy,resultJoints);
        for (int i = 0; i < 6; ++i) {
            cout<<"原值："<<joint11[i]<<" 逆解结果："<<resultJoints[i]<<endl;
        }
        cout<<"==============================="<<endl;

        double resultPoint1[6]={0};
        rightRobot->FK(resultJoints,resultPoint1);
        for (int i = 0; i < 6; ++i) {
            cout<<"原值："<<point11[i]<<" 正解结果："<<resultPoint1[i]<<endl;
        }
        cout<<"==============2222================="<<endl;
    });


    // 将窗口显示出来
    widget.show();

    int exec_code = app.exec();

    return exec_code;
}