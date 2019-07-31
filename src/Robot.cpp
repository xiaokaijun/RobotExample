#include "Robot.h"
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <fstream>

#include <iostream>
//#include <hand/Hand.h>
//#include <hand/HandCMD.h>
#include <thread>

using namespace std;



Robot::Robot() {
   loginRobot();
   initRobot();
   cout<<"创建了Robot实例"<<endl;
}

Robot::Robot(const char* ipaddr,int port,const char* username,const char* password,const char* deviceName){
    this->ipaddr = ipaddr;
    this->port = port;
    this->username = username;
    this->password = password;
    this->deviceName = deviceName;

    loginRobot();
    initRobot();
}



void Robot::loginRobot() {
    int ret = robotService.robotServiceLogin(ipaddr,port,username,password);
    if(ret == aubo_robot_namespace::InterfaceCallSuccCode){
        cout<<"登录成功!"<<endl;
    }else{
        cout<<"登录失败!"<<endl;
    }
}

void Robot::initRobot() {
    paw = new Paw(deviceName);
//    Hand h =
//    this->hand = new Hand(deviceName);

    /** 如果是连接真实机械臂，需要对机械臂进行初始化　**/
    aubo_robot_namespace::ROBOT_SERVICE_STATE result;

    //工具动力学参数
    aubo_robot_namespace::ToolDynamicsParam toolDynamicsParam;
    memset(&toolDynamicsParam, 0, sizeof(toolDynamicsParam));

    int ret = robotService.rootServiceRobotStartup(toolDynamicsParam/**工具动力学参数**/,
                                               6        /*碰撞等级*/,
                                               true     /*是否允许读取位姿　默认为true*/,
                                               true,    /*保留默认为true */
                                               1000,    /*保留默认为1000 */
                                               result); /*机械臂初始化*/
    if(ret != aubo_robot_namespace::InterfaceCallSuccCode)
    {
        std::cerr<<"机械臂初始化失败:"<<ret<<std::endl;
    }

    /** 模拟业务 **/
    /** 接口调用: 初始化运动属性 ***/
    robotService.robotServiceInitGlobalMoveProfile();

    /** 接口调用: 设置关节型运动的最大加速度 ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxAcc;
    jointMaxAcc.jointPara[0] = 10.0/180.0*M_PI;
    jointMaxAcc.jointPara[1] = 10.0/180.0*M_PI;
    jointMaxAcc.jointPara[2] = 10.0/180.0*M_PI;
    jointMaxAcc.jointPara[3] = 10.0/180.0*M_PI;
    jointMaxAcc.jointPara[4] = 10.0/180.0*M_PI;
    jointMaxAcc.jointPara[5] = 10.0/180.0*M_PI;   //接口要求单位是弧度
    robotService.robotServiceSetGlobalMoveJointMaxAcc(jointMaxAcc);

    /** 接口调用: 设置关节型运动的最大速度 ***/
    aubo_robot_namespace::JointVelcAccParam jointMaxVelc;
    jointMaxVelc.jointPara[0] = 25.0/180.0*M_PI;
    jointMaxVelc.jointPara[1] = 25.0/180.0*M_PI;
    jointMaxVelc.jointPara[2] = 25.0/180.0*M_PI;
    jointMaxVelc.jointPara[3] = 25.0/180.0*M_PI;
    jointMaxVelc.jointPara[4] = 25.0/180.0*M_PI;
    jointMaxVelc.jointPara[5] = 35.0/180.0*M_PI;   //接口要求单位是弧度
    robotService.robotServiceSetGlobalMoveJointMaxVelc(jointMaxVelc);


    /** 接口调用: 初始化运动属性 ***/
    robotService.robotServiceInitGlobalMoveProfile();

    /** 接口调用: 设置末端型运动的最大加速度 　　直线运动属于末端型运动***/
    double endMoveMaxAcc;
    endMoveMaxAcc = 0.2;   //单位米每秒
    robotService.robotServiceSetGlobalMoveEndMaxLineAcc(endMoveMaxAcc);
    robotService.robotServiceSetGlobalMoveEndMaxAngleAcc(endMoveMaxAcc);


    /** 接口调用: 设置末端型运动的最大速度 直线运动属于末端型运动***/
    double endMoveMaxVelc;
    endMoveMaxVelc = 0.2;   //单位米每秒
    robotService.robotServiceSetGlobalMoveEndMaxLineVelc(endMoveMaxVelc);
    robotService.robotServiceSetGlobalMoveEndMaxAngleVelc(endMoveMaxVelc);
}

void Robot::movej(double *joints) {
    int ret = robotService.robotServiceJointMove(joints, true);

    if(ret != aubo_robot_namespace::InterfaceCallSuccCode)
    {
        std::cerr<<"movej移动关节角失败:"<<ret<<std::endl;
    }
}



/**
 *
 * @param position
 */
void Robot::movel(double *position) {
    aubo_robot_namespace::Pos cartPos;
    cartPos.x = position[0];
    cartPos.y = position[1];
    cartPos.z = position[2];

    // 末端的姿态，aubo中需要用的是四元素，所以要转换成四元素才行
    aubo_robot_namespace::Rpy rpy;
    aubo_robot_namespace::Ori ori;

    rpy.rx = position[3];
    rpy.ry = position[4];
    rpy.rz = position[5];

    // 转成四元素
    robotService.RPYToQuaternion(rpy,ori);

    // 设置Base坐标系
    aubo_robot_namespace::CoordCalibrateByJointAngleAndTool userCoord;
    userCoord.coordType = aubo_robot_namespace::BaseCoordinate;



    /** 设置工具端参数 **/
    aubo_robot_namespace::ToolInEndDesc toolDesc;
    toolDesc.toolInEndPosition.x = 0;
    toolDesc.toolInEndPosition.y = 0;
    toolDesc.toolInEndPosition.z = 0;

    // 设置工具末端的姿态
    toolDesc.toolInEndOrientation.x = 0;
    toolDesc.toolInEndOrientation.y = 0;
    toolDesc.toolInEndOrientation.z = 0;
    toolDesc.toolInEndOrientation.w = 1;



       // 该api并不会修改机械臂末端的姿态
    int ret = robotService.robotMoveLineToTargetPosition(userCoord,cartPos,toolDesc,true);

//    robotService.robotServiceLineMove()
    if(ret != aubo_robot_namespace::InterfaceCallSuccCode){
        cerr<<"movel移动失败:"<<ret<<endl;
    }else{
        cout<<"movel移动成功"<<endl;
    }
}




/*
* 调用该函数，会保持当前姿态进行直线运动
*/
void Robot::movel(double *xyz,double* rpy){
    double joints[6]={0};
    this->IK(xyz,rpy,joints);
    // 移动机械臂
    int ret = robotService.robotServiceLineMove(joints,true);

    if(ret != aubo_robot_namespace::InterfaceCallSuccCode){
        cout<<"movel调用失败："<<ret<<endl;
    }
}

void Robot::IK(double* xyz,double* rpy,double* resultJoints){
    // 反解
    // 获取当前的姿态
//    aubo_robot_namespace::JointParam jointParam;
    aubo_robot_namespace::wayPoint_S currentWaypoint;
    // 获取当前关节角信息
    robotService.robotServiceGetCurrentWaypointInfo(currentWaypoint);
    // 当前的关节角度
    double jointsAngle[] = {
            currentWaypoint.jointpos[0]*DE2RA,
            currentWaypoint.jointpos[1]*DE2RA,
            currentWaypoint.jointpos[2]*DE2RA,
            currentWaypoint.jointpos[3]*DE2RA,
            currentWaypoint.jointpos[4]*DE2RA,
            currentWaypoint.jointpos[5]*DE2RA
    };
    // 目标点位置
    aubo_robot_namespace::Pos targetPosition;
    targetPosition.x = xyz[0];
    targetPosition.y = xyz[1];
    targetPosition.z = xyz[2];

    aubo_robot_namespace::Rpy rpyObj;
    aubo_robot_namespace::Ori targetOri;

    rpyObj.rx = rpy[0];
    rpyObj.ry = rpy[1];
    rpyObj.rz = rpy[2];


    robotService.RPYToQuaternion(rpyObj, targetOri);

    aubo_robot_namespace::wayPoint_S resultWayPoint;
    int ret = robotService.robotServiceRobotIk(jointsAngle,targetPosition,targetOri,resultWayPoint);
    if(ret != aubo_robot_namespace::InterfaceCallSuccCode){
        cout<<"反解失败拉～"<<endl;
    }

    for (int i = 0; i < 6 ; ++i) {
        resultJoints[i] = resultWayPoint.jointpos[i];
    }
}


void Robot::FK(double* jointAngle,double* resultPoint){

    ServiceInterface robotService;

    aubo_robot_namespace::wayPoint_S wayPoint;

    int ret = aubo_robot_namespace::InterfaceCallSuccCode;



    ret = robotService.robotServiceRobotFk(jointAngle, aubo_robot_namespace::ARM_DOF, wayPoint);

    if(ret != aubo_robot_namespace::InterfaceCallSuccCode)
    {
        cout<<"正解失败"<<endl;
    }
    resultPoint[0] = wayPoint.cartPos.position.x;
    resultPoint[1] = wayPoint.cartPos.position.y;
    resultPoint[2] = wayPoint.cartPos.position.z;

    aubo_robot_namespace::Ori ori;
    ori = wayPoint.orientation;
    aubo_robot_namespace::Rpy rpy;
    robotService.quaternionToRPY(ori,rpy);


    resultPoint[3] = rpy.rx;
    resultPoint[4] = rpy.ry;
    resultPoint[5] = rpy.rz;
}


void Robot::openHand(){
    cout<<"打开机械抓"<<endl;
    /*std::thread t([=](){
        this->hand->send(CMD_MC_MOVE_RELEASE,new ReleaseData(1000));
    });
    t.detach();*/

//    Hand hand = Hand::getInstance(deviceName);
//    bool flag = hand.send(CMD_MC_MOVE_RELEASE, new ReleaseData(1000));
//    usleep(500*1000);
    this->paw->release(1000);
}

void Robot::closeHand(){
    cout<<"关闭机械抓"<<endl;
//    cout<<"执行关闭操作："<<hand<<endl;
//    Hand hand = Hand::getInstance(deviceName);
   /* std::thread t([=](){
        this->hand->send(CMD_MC_MOVE_CATCH_XG,new CatchXgData(1000,1000));
    });
    t.detach();*/
//    Hand hand = Hand::getInstance(deviceName);
//    bool flag = hand.send(CMD_MC_MOVE_CATCH_XG,new CatchXgData(1000,1000));
//    usleep(500*1000);

    this->paw->catchXG();
}

Robot::~Robot() {
    cerr<<"机械臂被释放了"<<endl;

//    Hand hand = Hand::getInstance(deviceName);
//    hand.disconnect();
    this->paw->close();
//    hand->disconnect();
//    delete hand;
    robotService.robotServiceLogout();
}

ServiceInterface Robot::getService(){

    return robotService;
}