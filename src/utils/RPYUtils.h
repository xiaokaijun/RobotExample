//
// Created by kaijun on 6/29/19.
//

#ifndef STUDY_ROBOT_RPYUTILS_H
#define STUDY_ROBOT_RPYUTILS_H
#include <iostream>
#include <rw/math/Rotation3D.hpp>
#include <rw/math/Vector3D.hpp>
#include <rw/math/RPY.hpp>

using namespace std;
using namespace rw::math;

//// 问题描述：
/**
 * 给定法线方向，需要构建出一个坐标系
 * @return
 */
vector<double> calcRPY(vector<double> norm)
{

    /// 1.法线方向(normal)
    Vector3D<double> norVec( norm[0], norm[1], norm[2] );
    norVec = normalize(norVec);

    /// 2.给定参考方向，此方向根据实际情况指定
//    Vector3D<double> refVec( norm[0], norm[1], 0.0 );
    Vector3D<double> refVec( 1,0, 0 );

    /// 3.通过叉乘，计算x轴向量
    Vector3D<double> xVec = cross( norVec, refVec );
//    std::cout<<xVec<<std::endl;
    xVec = normalize( xVec );
//    std::cout<<xVec<<std::endl;

    /// 4.通过叉乘，计算y轴向量
    Vector3D<double> yVec = cross( norVec, xVec );
    yVec = normalize( yVec );

    /// 5.根据三个坐标轴，可以构建一个旋转矩阵，
    /// 输入依次是
    /// x轴向量
    /// y轴向量
    /// z轴向量(法向量)
    Rotation3D<double> rot( xVec, yVec, norVec );


    /// 通过旋转矩阵，可以直接构建出RPY对象
    RPY<double> rpy( rot );

    ///最后，需要注意的是，此处的RPY，是绕动坐标系变换的，顺序是z,y,x
    ///而UR驱动中,moveL的输入里，RPY是按x,y,z顺序输入的，
    ///因此，实际使用时，需要逆转输出
    std::cout << rpy[0] << "," << rpy[1] << "," << rpy[2]<<std::endl;



    std::vector<double> result{rpy[0],rpy[1],rpy[2]};
//    result = normalize(rpy);
    return result;
}


#endif //STUDY_ROBOT_RPYUTILS_H
