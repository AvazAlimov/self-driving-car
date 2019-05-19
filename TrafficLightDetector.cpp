#include "TrafficLightDetector.hpp"

using namespace cv;
using namespace std;

TrafficLightDetector::TrafficLightDetector(Mat image)
{
	resize(image, frame, Size(320, 480));
	frame = image(Rect(0, 0, 320, 44));
}

bool TrafficLightDetector::detect()
{	
	//filter image for noise immunity
	medianBlur(frame, frame, 3);
		
	//Convert BGR image to HSV
	Mat hsv;
	cvtColor(frame, hsv, COLOR_BGR2HSV);

	Mat lower_red, upper_red;
	inRange(hsv, Scalar(0, 120, 70), Scalar(10, 255, 255), lower_red);
	inRange(hsv, Scalar(170, 120, 70), Scalar(180, 255, 255), upper_red);
        
	//combine pictures with red circles and add gaussian blur to avoid false positives
	Mat red_image;
	addWeighted(lower_red, 1.0, upper_red, 1.0, 0.0, red_image);
	
	//erode(red_image, red_image, getStructuringElement(MORPH_ELLIPSE, Size(9, 9)));
	//dilate(red_image, red_image, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	GaussianBlur(red_image, frame, Size(9, 9), 2, 2);
	
	//Do Hough Transform to detect circles and put them in vector
	vector<Vec3f> circles;
	HoughCircles(frame, circles, CV_HOUGH_GRADIENT, 1, frame.rows / 8, 110, 20, 0, 0);
		
	if(circles.size() > 0) {
		cout << "RED LIGHT" << endl;
		return true;
	}
	
	return false;
}
