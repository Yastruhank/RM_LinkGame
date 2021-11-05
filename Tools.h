#ifndef RM_LINKGAME_TOOLS_H
#define RM_LINKGAME_TOOLS_H
#include <cstring>
#include <string>
#include <sstream>
#define min(a,b) a<b?a:b
#define max(a,b) a>b?a:b
std::string int_to_string(int num){
    std::ostringstream oss;
    oss<<num;
    return oss.str();
}

double abs_diff(cv::Mat mat1,cv::Mat mat2){
    double diff=0;
    CV_Assert(mat1.type()==CV_8UC1&&mat2.type()==CV_8UC1);
    CV_Assert(mat1.cols==mat2.cols&&mat1.rows==mat2.rows);
    for(int i=0;i<mat1.rows;i++){
        for(int j=0;j<mat1.cols;j++){
            diff+=abs(mat1.at<uchar>(i,j)-mat2.at<uchar>(i,j));
        }
    }
    return diff;
}

#endif //RM_LINKGAME_TOOLS_H
