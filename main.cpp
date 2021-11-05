#include <iostream>
#include <opencv2/opencv.hpp>
#include"detect.h"
detect detection;
int main() {
    cv::Mat src=cv::imread("E:\\workspace\\RM_LinkGame\\src1.png");
    detection.scan(src);
    cv::waitKey(0);
    return 0;
}
