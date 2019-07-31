//
// Created by kaijun on 7/30/19.
//
#include <utils/RPYUtils.h>
/**
 *  测试RobWork函数库是否已经正确导入
 */
int main(){
    vector<double> norm = {0.1,0.1,0.1};
    vector<double> result = calcRPY(norm);

    cout<<result[0]<<endl;
    cout<<result[1]<<endl;
    cout<<result[2]<<endl;

    return 0;
}
