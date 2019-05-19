#include "opencv2/opencv.hpp"

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
