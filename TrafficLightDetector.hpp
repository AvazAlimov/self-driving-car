#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#ifndef TRAFFIC_LIGHT_DETECTOR_H
#define TRAFFIC_LIGHT_DETECTOR_H

class TrafficLightDetector
{
	private:
		cv::Mat frame;
	public:
		TrafficLightDetector(cv::Mat image);
		bool detect();
};

#endif
