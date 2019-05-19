#include "SignDetector.hpp"
#include <iostream>

using namespace cv;
using namespace std;

SignDetector::SignDetector()
{	
    leftSignCascade.load("./cascade/cascade_left.xml");
	rightSignCascade.load("./cascade/cascade_right.xml");
	parkingSignCascade.load("./cascade/cascade_parking.xml");
	stopSignCascade.load("./cascade/cascade_stop.xml");
	pedestrianSignCascade.load("./cascade/cascade_pedestrian.xml");
//	girlSignCascade.load("./cascade/cascade_girl_pedestrian.xml");
}

vector<int> SignDetector::detectSign(Mat image) {
	vector<int> detectedSigns;
	
	if(isSignDetected(image, leftSignCascade)) {
		detectedSigns.push_back(Sign::SIGN_LEFT);
	} else if(isSignDetected(image, rightSignCascade)) {
		detectedSigns.push_back(Sign::SIGN_RIGHT);
	} else if(isSignDetected(image, parkingSignCascade)) {
		detectedSigns.push_back(Sign::SIGN_PARKING);
	} else if(isSignDetected(image, stopSignCascade)) {
		detectedSigns.push_back(Sign::SIGN_STOP);
	} else if(isSignDetected(image, pedestrianSignCascade)) {
		detectedSigns.push_back(Sign::SIGN_PEDESTRIAN);
	}
	
	// FIXME: - THINK ABOUT GIRL CASCADE
	/*
	else if isSignDetected(image, girlSignCascade) {
		detectedSigns.push_back(Sign::SIGN_GIRL);
	}
	* */
	
	return detectedSigns;
}

bool SignDetector::isSignDetected(Mat image, CascadeClassifier classifier) {
	vector<Rect> sign;
	classifier.detectMultiScale(image, sign, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(45, 45), Size(100, 100));
	
	return !sign.empty();	
}
