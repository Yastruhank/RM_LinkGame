#ifndef RM_LINKGAME_TOOLS_H
#define RM_LINKGAME_TOOLS_H
#include <cstring>
#include <string>
#include <sstream>
#include<vector>
#include<opencv2/opencv.hpp>
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

//@note 本结构体比较为坐标位置横坐标比较
struct block{
    int num;
    cv::Point place;
    bool operator<(const block b)const{
        return this->place.x<b.place.x;
    }
    operator int(){return num;}
    operator cv::Point(){return place;}
    block(int num_,cv::Point point_):num(num_),place(point_){};
    block():num(0),place(cv::Point (0,0)){};
};

struct block_node{
    std::vector<block> data;
    block_node* next=NULL;
    block_node(int num,cv::Point point){
        data.push_back(block(num,point));
    }
};

#endif //RM_LINKGAME_TOOLS_H
