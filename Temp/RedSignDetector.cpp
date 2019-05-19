#include "RedSignDetector.hpp"

using namespace cv;
using namespace std;

#define MIN_PERIMETER 100
#define MIN_AREA 200

RedSignDetector::RedSignDetector(Mat image)
{
    Mat frame;    
    resize(image, frame, Size(640, 480));
    frame = frame(Rect(0, 0, frame.cols, frame.rows / 2));
    cvtColor(frame, frame, COLOR_BGR2HSV);
    inRange(frame, Scalar(0, 50, 50), Scalar(15, 255, 255), red);
    inRange(frame, Scalar(150, 50, 50), Scalar(179, 255, 255), red);
    
    imshow("ROI", red);
    waitKey(1);
    
    //frame = Scalar::all(255) - frame;
}

int RedSignDetector::detect()
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(red, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    for (int i = 0; i < contours.size(); i++)
    {
        double perimeter = arcLength(contours[i], true);
        double area = contourArea(contours[i]);
        approxPolyDP(contours[i], contours[i], 0.02 * perimeter, true);

        if (perimeter > MIN_PERIMETER && area > MIN_AREA && isContourConvex(contours[i]))
        {
            if (contours[i].size() == 8)
            {
                cout << "STOP DETECTED" << endl;
                return RedSign::STOP_SIGN;
            }
            if (contours[i].size() == 3)
            {
                cout << "PEDESTRIAN" << endl;
                return RedSign::PEDESTRIAN_SIGN;
            }
        }
    }
    return RedSign::NONE;
}
