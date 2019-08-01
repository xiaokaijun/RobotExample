# 机械臂示例工程

该项目正在不断更新优化过程中

本示例工程包含如下内容:
1. 机械臂基本操作
2. 机械抓串口通讯
3. 相片平面坐标转相机三维坐标
4. 相机坐标系转机械臂坐标系
5. 手眼标定
6. 自动抓取小方块
7. 机械臂绘画
9. 通过调整姿态画圆


## 1.环境搭建
进行代码调试之前,请确认您是否有以下硬件设备
### 1.1 测试条件
1. 系统环境: ubuntu14.04
2. 开发工具: clion
3. 机械臂:Auboi3
4. 因时机械抓
5. Kinect2相机

### 1.2 下载依赖
当您下载完该项目之后该项目之后,您还需要下载这个项目的依赖库.

下载robotdependents文件夹,这个文件大概1G,所以大家可以去百度网盘中下载.

在工程的cmake文件夹中有很多.cmake文件,您需要打开这些文件,将robotdependents的路径修改正确


## 2. 代码测试
每个文件都是一个独立的程序,直接进去运行main函数即可!
## 3. 注意
如果出现std::__cxx11::baisc_string 相关的错误.请按如下步骤进行修改:
```
cd /usr/lib/x86_64-linux-gnu/

ls -ll libstdc*

查看是否列出 libstdc++.so.6.0.25 ,如果没有,就去别人的电脑上拷贝一个放到这个路径下面
创建6.0.25的软链接
sudo -sdf libstdc++.so.6.0.25 libstdc++.so.6 
```

## 4.参考文档
```$xslt
https://blog.csdn.net/zhangrelay/article/details/81177851

https://blog.csdn.net/UAV_FRESHMAN/article/details/52832874
```