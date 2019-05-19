#include "BlueSignDetector.hpp"

using namespace cv;
using namespace std;

#define MIN_PERIMETER 200
#define MIN_COUNT 8
#define MAX_COUNT 2

BlueSignDetector::BlueSignDetector(Mat image)
{
    Mat frame;
    resize(image, frame, Size(640, 480));
    cvtColor(frame, frame, COLOR_BGR2HSV);
    inRange(frame, Scalar(80, 65, 65, 0), Scalar(140, 255, 255, 0), blue);

    imshow("BLUE", blue);
    waitKey(1);
}

int BlueSignDetector::detect()
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(blue, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    for (int i = 0; i < contours.size(); i++)
    {
        double perimeter = arcLength(contours[i], true);
        
        if (perimeter > MIN_PERIMETER)
        {
            Rect boundary = boundingRect(contours[i]);
            Mat segment = blue(boundary);

            if (contours[i].size() > 100)
            {
                int centerX = segment.cols / 2;
                int centerY = segment.rows / 2;

                int deltaX = centerX, left = 0, right = 0;
                while (deltaX > 0)
                {
                    left += ((int)segment.at<uchar>(centerY, centerX - deltaX) > 0);
                    right += ((int)segment.at<uchar>(centerY, centerX + deltaX) > 0);
                    deltaX--;
                }

                int deltaY = centerY, top = 0, bottom = 0;
                while (deltaY > 0)
                {
                    top += ((int)segment.at<uchar>(centerY - deltaY, centerX) > 0);
                    bottom += ((int)segment.at<uchar>(centerY + deltaY, centerX) > 0);
                    deltaY--;
                }

                if (left < MAX_COUNT && right > MIN_COUNT)
                {
					cout << "RIGHT DETECTED" << endl;
					return BlueSign::SIGN_RIGHT;
				}
                if (right < MAX_COUNT && left > MIN_COUNT)
                {
					cout << "LEFT DETECTED" << endl;
                	return BlueSign::SIGN_LEFT;
				}
                if (bottom < MAX_COUNT && top > MIN_COUNT)
                {
					cout << "FORWARD DETECTED" << endl;
					return BlueSign::SIGN_FORWARD;
				}
            } else
            {
                approxPolyDP(contours[i], contours[i], 0.04 * perimeter, true);
                if (contours[i].size() == 4)
                {
					cout << "PARKING" << endl;
					return BlueSign::SIGN_PARKING;
				}
            }
        }
    }

    return BlueSign::SIGN_NONE;
}
