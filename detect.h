#ifndef RM_LINKGAME_DETECT_H
#define RM_LINKGAME_DETECT_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "Tools.h"

class detect {
private:
    cv::Mat module[8];
    cv::Size img_size = cv::Size(32, 32);
    block data[10][20];
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
        for (int i = 0; p != NULL; i++) {
            for (int j = 0; j < p->data.size(); j++) {
                data[i][j] = p->data[j];
            }
            std::sort(data[i],data[i]+p->data.size());
            auto un_free = p;
            p = p->next;
            delete (p);
        }
        begin=NULL;
    }

public:
    detect() {
        for (int i = 0; i < 8; i++) {
            module[i] = cv::imread("E:\\workspace\\RM_LinkGame\\module\\" + int_to_string(i) + ".png");
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

    void scan(cv::Mat &input) {
        cv::Mat show = input.clone();
        auto interest_rect = locate(input);
        cv::Mat interest_area = input(interest_rect);
        cv::imshow("interest", interest_area);
        cv::Mat color[3];
        cv::split(interest_area, color);
        cv::Mat blue_thre, red_thre;
        cv::threshold(color[0], blue_thre, 200, 255, 0);
        cv::threshold(color[2], red_thre, 200, 255, 0);
        cv::Mat thre_mat = blue_thre & red_thre;
        std::vector<std::vector<cv::Point>> counters;
        cv::findContours(thre_mat, counters, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < counters.size(); i++) {
            cv::Rect rect = cv::minAreaRect(counters[i]).boundingRect();
            rect += interest_rect.tl();
            if (rect.area() < 100)continue;
            cv::Mat img = input(rect);
            int ans = classify(img);
            insert_data(ans, (rect.tl() + rect.br()) / 2);
            cv::putText(show, int_to_string(ans), (rect.tl() + rect.br()) / 2, cv::FONT_HERSHEY_COMPLEX, 1,
                        cv::Scalar(0, 0, 255));
            cv::rectangle(show, rect, cv::Scalar(0, 0, 255), show.cols / 400 + 1);
            cv::imshow("show", show);
        }
        convert_data();
        for(int i=0;i<10;i++){
            for(int j=0;j<7;j++){
                std::cout<<data[i][j].num<<" ";
            }
            std::cout<<std::endl;
        }
    }

    template <class T>T& at(int i,int j){
        CV_Assert(i<10&&j<7);
        return T(data[i][j]);
    }

};

#endif //RM_LINKGAME_DETECT_H
