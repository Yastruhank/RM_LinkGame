#include <iostream>
#include <opencv2/opencv.hpp>
#include"detect.h"
#include"solve.h"
#define IMSHOW_DEBUG
int main() {
    detect detection;
    solve solver(&detection);

    cv::Mat src = cv::imread("E:\\workspace\\RM_LinkGame\\src2.png");
    bool find = detection.scan(src);
//    if (find)
//        solver.update();
    cv::waitKey(0);

    return 0;
}
