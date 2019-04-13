# MSVision 工程

## 1. 需要实现的功能
1. 相机控制模块
   * ~~实现增益、Gamma 值和曝光时间的调节~~
   * 保存相机参数文件
2. 光源控制模块
   * ~~实现对光源亮度的调节~~
   * ~~完善光源控制功能~~
   * 检测未连接的情况
   * ~~关闭程序时关闭光源~~
3. 标定模块
   * 实现双目标定
   * 实现手眼标定
4. 测量模块
   * ~~添加图像处理的选择~~
   * ~~检测结果的显示和保存~~
   * ~~显示单步测量耗时~~
5. 图像处理
   * ~~Zernike 正交矩亚像素检测~~
   * Snake 算法测试
   * 椭圆曲线段的聚类
     * ~~系统聚类~~
     * k-means 聚类
   * RANSAC 算法的改进
   * 添加调整图像亮度功能
   * ~~添加清晰度评价函数~~
6. 其他
   * ~~修改当前的采集模式为点击检测~~ (**只进行有限次采集和测量，并取平均结果**)
   * 生成安装包

## 2. 当前存在的问题
1. ~~Qt 中显示的图像颜色通道不正确~~
2. ~~解决 PyrEllipse 中的 ROI 起点坐标问题~~
3. ~~图片 100% 显示时的十字线崩溃问题~~ (**采用了子类化 Qt 控件的方法实现**)
4. ~~需要解决两幅画面不同步的问题~~
5. 当前的检测错误率还是太高了
   * 需要较亮的光源
   * ~~边缘检测和椭圆提取的稳定性需要提高~~ (**通过边缘跟踪算法和曲线连接算法提高稳定性**)
6. ~~针对椭圆曲线重建，提高了参数测量精度~~
7. Qt 界面逻辑仍然存在问题
   * 改变尺度显示会报错
8. 程序运行速度需要提高