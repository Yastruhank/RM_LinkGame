#include <iostream>
#include <opencv2/opencv.hpp>
#include"detect.h"
#include"solve.h"
int main() {
    detect detection;
    solve solver(&detection);
    bool start=0;
    while(true){
        try{
            if (!start) {
                if (KEY_DOWN('S'))start = 1;
                continue;
            }
            int width, height;
            cv::Mat src = CopyScreenToMat(width, height);
            bool find = detection.scan(src);
            if (find)
                solver.update();
            static double last_tick = cv::getTickCount();
            std::cout << "FPS=" << cv::getTickFrequency() / (cv::getTickCount() - last_tick) << std::endl;
            last_tick = cv::getTickCount();
            char input = cv::waitKey(1);
            if (input == 'q')exit(0);
        }catch(...){
            detection=detect();
            solver=solve(&detection);
            continue;
        }
    }
    return 0;
}
