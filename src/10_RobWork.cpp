//
// Created by kaijun on 7/30/19.
//
#include <utils/RPYUtils.h>
#include "Robot.h"
/**
 *  测试RobWork函数库是否已经正确导入
 *
 *  1. 项目问题
 *  2. 姿态问题
 */
int main(){
    Robot robot;

    vector<double> norm = {0,0,-1};
    double alpha = 5;


    vector<double> ref = {cos(alpha),sin(alpha),0};

    vector<double> result = calcRPY(norm,ref);

    cout<<cos(alpha)<<endl;
    cout<<sin(alpha)<<endl;
    cout<<result[0]<<endl;
    cout<<result[1]<<endl;
    cout<<result[2]<<endl;


    double xyz[] = {0.07,-0.32,0.25};
    double rpy[] = {result[0],result[1],result[2]};
    robot.movel(xyz,rpy);


    return 0;
}
