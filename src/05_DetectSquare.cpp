//
// Created by kaijun on 7/20/19.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include "utils/05_DetectSquare.h"

using namespace cv;
using namespace std;
/**
 * 小方块识别案例
*/


int main(){
    // 获取原始彩色图像
    Mat src = imread("./assets/square.jpg",IMREAD_COLOR);

    vector<Square> squares;
    findSquare(src, squares);

    imshow("src",src);

    cout<<"最终小方块的数量："<<squares.size()<<endl;

    waitKey(0);

    return 0;
}


