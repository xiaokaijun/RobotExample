#ifndef AUBOINTERFACEEXAMPLE_ROBOT_H
#define AUBOINTERFACEEXAMPLE_ROBOT_H
#include "AuboRobotMetaType.h"
#include "serviceinterface.h"
//#include "hand/Hand.h"
#include "paw/Paw.h"
#define DE2RA M_PI/180
class Robot {
    public:
        // 创建机器人服务
        ServiceInterface robotService;
    private:
        const char* ipaddr="192.168.1.101";
        int port = 8899;
        const char* username="admin";
        const char* password="1";
        const char* deviceName="/dev/ttyUSB0";
//        Hand* hand = nullptr;
        Paw* paw=nullptr;
    public:
        Robot();
        Robot(const char* ipaddr,int port,const char* username,const char* password,const char* deviceName);
        /*登录机械臂*/
        void loginRobot();
        /*初始化机械臂*/
        void initRobot();
        /*移动机械臂*/
        void movej(double* joints);
        void movel(double* position);
        void movel(double *xyz,double* rpy);
        /*退出机械臂*/
        ~Robot();
        /*打开机械抓*/
        void openHand();
        /*关闭机械抓*/
        void closeHand();

        void IK(double* xyz,double* rpy,double* resultJoints);
        void FK(double* jointAngle,double* resultPoint);

        ServiceInterface getService();
};

#endif //AUBOINTERFACEEXAMPLE_ROBOT_H
