#ifndef RM_LINKGAME_DETECT_H
#define RM_LINKGAME_DETECT_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "Tools.h"

class detect {
private:
    cv::Mat module[8];
    cv::Size img_size = cv::Size(32, 32);
    block data[20][20];
    block_node *begin = NULL;

private:
    cv::Rect locate(cv::Mat input) {
        cv::Mat color[3];
        cv::split(input, color);
        cv::Mat green_thre, blue_thre, red_thre;
        cv::threshold(color[1], green_thre, 200, 255, 0);
        cv::threshold(color[0], blue_thre, 200, 255, 0);
        cv::threshold(color[2], red_thre, 200, 255, 0);
        cv::Mat thre = green_thre & (~blue_thre) & (~red_thre);
        std::vector<std::vector<cv::Point>> counters;
        cv::findContours(thre, counters, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        cv::Rect max_rect = cv::minAreaRect(counters[0]).boundingRect();
        for (int i = 1; i < counters.size(); i++) {
            cv::Rect rect = cv::minAreaRect(counters[i]).boundingRect();
            if (rect.area() > max_rect.area())max_rect = rect;
        }
        return max_rect;
    }

    void insert_data(int num, cv::Point place) {
        auto p = begin;
        if (p == NULL) {
            begin = new block_node(num, place);
            return;
        }
        if (abs(place.y - p->data[0].place.y) < 10) {
            p->data.push_back(block(num, place));
            return;
        }
        if (place.y < p->data[0].place.y) {
            auto new_data = new block_node(num, place);
            new_data->next = p;
            begin = new_data;
            return;
        }
        while (true) {
            if (p->next == NULL) {
                p->next = new block_node(num, place);
                break;
            }
            if (abs(place.y - p->next->data[0].place.y) < 10) {
                p->next->data.push_back(block(num, place));
                break;
            }
            if (place.y < p->next->data[0].place.y) {
                auto new_data = new block_node(num, place);
                new_data->next = p->next;
                p->next = new_data;
                break;
            }
            p = p->next;
        }
    }

    void convert_data() {
        auto p = begin;
        for (int i = 1; p != NULL; i++) {
            if(i>11)break;
            int size=min(p->data.size(),7);
            for (int j = 1; j <= size; j++) {
                data[i][j] = p->data[j-1];
            }
            std::sort(data[i]+1,data[i]+1+size);
            auto un_free = p;
            p = p->next;
            delete (un_free);
        }
        begin=NULL;
    }

public:
    detect() {
        for (int i = 0; i < 8; i++) {
            module[i] = cv::imread("..\\module\\" + int_to_string(i) + ".png");
            cv::resize(module[i], module[i], img_size);
            cv::cvtColor(module[i], module[i], cv::COLOR_BGR2GRAY);
        }
    }

    int classify(cv::Mat img_) {
        cv::Mat img = img_.clone();
        cv::resize(img, img, img_size);
        cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
        std::vector<float> descriptors;
        int min_id = 0;
        auto min_diff = abs_diff(img, module[0]);
        for (int i = 1; i < 8; i++) {
            float diff = abs_diff(img, module[i]);
            if (diff < min_diff) {
                min_id = i;
                min_diff = diff;
            }
        }
        return min_id;
    }

    bool scan(cv::Mat &input) {
        cv::Mat show = input.clone();
        auto interest_rect = locate(input);
        if(interest_rect.area()<10000)return 0;
        cv::Mat interest_area = input(interest_rect);
#ifdef IMSHOW_DEBUG
        cv::imshow("interest", interest_area);
#endif
        cv::Mat color[3];
        cv::split(interest_area, color);
        cv::Mat blue_thre, red_thre;
        cv::threshold(color[0], blue_thre, 200, 255, 0);
        cv::threshold(color[2], red_thre, 200, 255, 0);
        cv::Mat thre_mat = blue_thre & red_thre;
#ifdef IMSHOW_DEBUG
        cv::imshow("thre", thre_mat);
#endif
        std::vector<std::vector<cv::Point>> counters;
        cv::findContours(thre_mat, counters, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < counters.size(); i++) {
            cv::Rect rect = cv::minAreaRect(counters[i]).boundingRect();
            if(!thre_mat.at<uchar>(rect.tl()+cv::Point(3,3))||!thre_mat.at<uchar>(rect.br()-cv::Point(3,3)))continue;
            rect += interest_rect.tl();
            if (rect.area() < 1000)continue;
            cv::Mat img = input(rect);
            int ans = classify(img);
            insert_data(ans, (rect.tl() + rect.br()) / 2);
            cv::putText(show, int_to_string(ans), (rect.tl() + rect.br()) / 2, cv::FONT_HERSHEY_COMPLEX, 1,
                        cv::Scalar(0, 0, 255));
            cv::rectangle(show, rect, cv::Scalar(0, 0, 255), show.cols / 400 + 1);
        }
#ifdef IMSHOW_DEBUG
        cv::imshow("show", show);
#endif
        convert_data();
        return 1;
    }

    template <class T>T at(int i,int j){
        if(i>=12||j>=9||i<0||j<0){
            return T(block(-1,cv::Point(-1,-1)));
        }
        return T(data[i][j]);
    }

    template <class T>T at(cv::Point place){
        if(place.x>=12||place.y>=9||place.x<0||place.y<0){
            return T(block(-1,cv::Point(-1,-1)));
        }
        return T(data[place.x][place.y]);
    }

};

#endif //RM_LINKGAME_DETECT_H
