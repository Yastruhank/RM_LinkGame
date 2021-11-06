#ifndef RM_LINKGAME_SOLVE_H
#define RM_LINKGAME_SOLVE_H
#include"detect.h"
class solve{
private:
    detect* detector;
    bool vis_dfs[20][20];
    cv::Point dfs(cv::Point cnt, cv::Point start,int direction, int turn) {
        if(vis_dfs[cnt.x][cnt.y])return cv::Point(-1,-1);
        if(detector->at<int>(cnt)==-1||cnt==start)return cv::Point(-1,-1);
        if(detector->at<int>(cnt)==detector->at<int>(start))return cnt;
        if(detector->at<int>(cnt)==0){
            auto ret=dfs(cnt+cv::Point(abs(direction)==1?direction:0,abs(direction)==2?direction/2:0),start,direction,turn);
            if(ret!=cv::Point(-1,-1))return ret;
            if(turn<2) {
                for (int i = -2; i <= 2; i++) {
                    if (i == 0 || i == direction)continue;
                    ret = dfs(cnt + cv::Point(abs(i) == 1 ? i : 0, abs(i) == 2 ? i / 2 : 0), start, i, turn + 1);
                    if(ret!=cv::Point(-1,-1))return ret;
                }
            }
        }
        return cv::Point(-1,-1);
    };

    int times=0;

public:
    solve(detect* detector_){
        detector=detector_;
    };

    void update(){
        memset(vis_dfs,0,sizeof(vis_dfs));
        for(int i=1;i<11;i++){
            for(int j=1;j<8;j++){
                if(detector->at<int>(cv::Point(i,j))==0)continue;
                for(int k=-2;k<=2;k++){
                    auto ret = dfs(cv::Point(i,j) + cv::Point(abs(k) == 1 ? k : 0, abs(k) == 2 ? k / 2 : 0), cv::Point(i,j), k, 0);
                    if(ret!=cv::Point(-1,-1)){
                        vis_dfs[i][j]=1;
                        vis_dfs[ret.x][ret.y]=1;
                        MouseClick(detector->at<cv::Point>(i,j));
                        MouseClick(detector->at<cv::Point>(ret));
                        if(KEY_DOWN('Q'))exit(0);
                        cv::waitKey(50);
                    }
                }
            }
        }
    }

};
#endif //RM_LINKGAME_SOLVE_H
