

#include "utils/03_Camera2Base.h"
/**
 * 测试封装的相机工具类
 */
int main(int argc, char **argv)
{

    double cameraPos[3]={0.01,0.01,0.9};
    double basePos[3]={0};
    camera2base(cameraPos, basePos);


    return 0;
}

