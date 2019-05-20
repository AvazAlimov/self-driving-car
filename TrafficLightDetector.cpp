#include "TrafficLightDetector.hpp"

using namespace cv;
using namespace std;

Mat1b getRed(Mat image){

    Mat3b hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);

    Mat1b mask1, mask2;
    inRange(hsv, Scalar(0, 70, 50), Scalar(10, 255, 255), mask1);
    inRange(hsv, Scalar(170, 70, 50), Scalar(180, 255, 255), mask2);

    Mat1b mask = mask1 | mask2;
    
    imshow("Mask", mask);
    waitKey(1);
    
    return mask;
}


TrafficLightDetector::TrafficLightDetector(Mat image)
{
	resize(image, frame, Size(500, 400));
}

bool TrafficLightDetector::detect()
{	
	CascadeClassifier face_cascade;
    if(!face_cascade.load("./cascade/cascade_traffic_light.xml"))
    {
        cout << "Error Loading XML file" << endl;
        return false;
    }

    vector<Rect> right_tl;
    vector<Rect> left_tl;
    Mat1b mask = getRed(frame);
    
    //x,y, width, height
    int x = mask.cols;
    int y = mask.rows;

    int x_right = (int)(x * 0.78);
    int y_right = (int)(y * 0.25);

    int width = (int)(y*0.25);
    int width_x = (int)(x-x_right);

    Mat1b cropped_right = mask(Rect(x_right,y_right,width_x,width));
    face_cascade.detectMultiScale(
		cropped_right,
		right_tl,
		1.1,
		2,
		0 | CV_HAL_CMP_GE,
		Size(30, 30)
	);

    int x_left = 0;
    int y_left = (int)(y*0.15);

    Mat1b cropped_left = mask(Rect(x_left,y_left,width,width));
    face_cascade.detectMultiScale(
		cropped_left,
		left_tl,
		1.1,
		2,
		0 | CV_HAL_CMP_GE,
		Size(30, 30)
	);

    if(!left_tl.empty() || !right_tl.empty()){
		cout << "Traffic Light Detected" << endl;
        return true;
    }

    return false;
}
