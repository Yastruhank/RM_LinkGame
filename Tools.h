#ifndef RM_LINKGAME_TOOLS_H
#define RM_LINKGAME_TOOLS_H
#include <cstring>
#include <string>
#include <sstream>
#include<vector>
#include<opencv2/opencv.hpp>
#include<windows.h>
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
        next=NULL;
        data.clear();
        data.push_back(block(num,point));
    }
};

cv::Mat CopyScreenToMat(int &nWidth, int &nHeight)
{
    //屏幕和内存设备描述表
    HDC hScrDC, hMemDC;
    //位图句柄
    HBITMAP hBitmap,hOldBitmap;
    //屏幕分辨率
    int xScrn,yScrn;
    //为屏幕创建设备描述表
    hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    //为屏幕设备描述表创建兼容的内存设备描述表
    hMemDC = CreateCompatibleDC(hScrDC);
    //获得屏幕分辨率
    xScrn = GetDeviceCaps(hScrDC, HORZRES);
    yScrn = GetDeviceCaps(hScrDC, VERTRES);

    //存储屏幕的宽度
    nWidth = xScrn;
    nHeight = yScrn;

    //创建一个与屏幕设备描述表兼容的 位图
    hBitmap = CreateCompatibleBitmap(hScrDC, xScrn,yScrn);
    //把新位图选到内存设备描述表中
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    //把屏幕设备描述表拷贝到内存设备描述表中
    BitBlt(hMemDC, 0, 0, xScrn, yScrn, hScrDC, 0, 0, SRCCOPY);
    //得到屏幕位图句柄
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
    //清除
    DeleteDC(hScrDC);
    DeleteDC(hMemDC);
    BITMAP bmp;
    GetObject(hBitmap,sizeof(BITMAP),&bmp);
    int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel/8 ;
    cv::Mat screen;
    screen.create(cv::Size(nWidth,nHeight),CV_8UC4);
    GetBitmapBits(hBitmap,nHeight*nWidth*nChannels,screen.data);
    cvtColor(screen , screen , cv::COLOR_BGRA2BGR);//COLOR_RGBA2BGR表示4通道转成3通道
    return screen;
}

void MouseClick(cv::Point place){
    bool move=SetCursorPos(place.x,place.y);
    if(!move){
        std::cout<<"Move Mouse Error:"<<GetLastError<<std::endl;
    }
    mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP,0,0,0,0);
}


#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
#endif //RM_LINKGAME_TOOLS_H
