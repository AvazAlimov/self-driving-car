#include "opencv2/opencv.hpp"
#include <vector>

#ifndef RED_SIGN_DETECTOR_H
#define RED_SIGN_DETECTOR_H

enum RedSign
{
    NONE = 0,
    PEDESTRIAN_SIGN = 1,
    STOP_SIGN = 2,
};

class RedSignDetector
{
    private:
        cv::Mat red;
    public:
        RedSignDetector(cv::Mat image);
        int detect();
};
#endif
