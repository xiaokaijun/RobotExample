
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "paw/Paw.h"
#include <iostream>

using namespace std;

int main() {
    Paw paw("/dev/ttyUSB0");
    // 关闭机械抓
    paw.catchXG();
    // 打开机械抓
    paw.release();
    // 断开与机械抓的连接
    paw.close();

    return 0;
}
