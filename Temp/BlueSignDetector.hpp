#include "opencv2/opencv.hpp"
#include <vector>

#ifndef BLUE_SIGN_DETECTOR_H
#define BLUE_SIGN_DETECTOR_H

enum BlueSign
{
	SIGN_NONE = 0,
	SIGN_FORWARD = 1,
	SIGN_LEFT = 2,
	SIGN_RIGHT = 3,
	SIGN_PARKING = 4
};

class BlueSignDetector
{
    private:
        cv::Mat blue;
    public:
        BlueSignDetector(cv::Mat image);
        int detect();
};

#endif
