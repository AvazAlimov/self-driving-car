#include <iostream>
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include "MotorManager.hpp"

#ifndef LANEDETECTOR_H
#define LANEDETECTOR
class LaneDetector
{
    private:
        double leftB, rightB, leftM, rightM;
        
        double average(std::vector<double> numbers);
        cv::Mat detectEdges(cv::Mat image);
        cv::Mat cutInterestArea(cv::Mat image);
        std::vector<cv::Vec4i> getLines(cv::Mat image);
        void buildLines(std::vector<cv::Vec4i> lines);

    public:
        cv::Mat sourceImage;
        
        LaneDetector(cv::Mat sourceImage);
        int getDirection();
        int drawLane();
};
#endif
