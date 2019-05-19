#include "LaneDetector.hpp"
#include <iostream>

using namespace cv;
using namespace std;

LaneDetector::LaneDetector(Mat image)
{
    //Resize and set a given image into sourceImage;
    resize(image, sourceImage, Size(320, 240));
    
    // Set default function components for left and right lines
    // y = m * x + b
    leftB = sourceImage.rows;
    rightB = -sourceImage.rows;
    leftM = -sourceImage.rows / (double)(sourceImage.cols / 2);
    rightM = sourceImage.rows / (double)(sourceImage.cols / 2);

    // Detect edges of an image
    Mat edges = detectEdges(sourceImage);
    // Cut ROI from edges image
    Mat segment = cutInterestArea(edges);
    // Get lines from ROI
    vector<Vec4i> lines = getLines(segment);
    // Set function components from lines
    buildLines(lines);
}

double LaneDetector::average(vector<double> slopes)
{
    double sum = 0;
    for (int i = 0; i < slopes.size(); sum += slopes[i], i++);
    return sum / slopes.size();
}

Mat LaneDetector::detectEdges(Mat image)
{
    Mat output;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    cvtColor(image, output, COLOR_BGR2GRAY);
    GaussianBlur(output, output, Size(5, 5), 0, 0);
    Canny(output, output, 100, 255);

    findContours(output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
    output = Mat::zeros(output.size(), output.type());
    for (int i = 0; i < contours.size(); i++)
        drawContours(output, contours, i, Scalar::all(255), 2, 8, hierarchy, 0, Point());

    return output;
}

Mat LaneDetector::cutInterestArea(Mat image)
{
    Mat output;
    Point points[4] = {
        Point(0, sourceImage.rows),
        Point(0, sourceImage.rows * 3 / 4),
        Point(sourceImage.cols, sourceImage.rows * 3 / 4),
        Point(sourceImage.cols, sourceImage.rows)};

    Mat mask = Mat::zeros(image.size(), image.type());
    fillConvexPoly(mask, points, 4, Scalar(255, 0, 0));
    bitwise_and(image, mask, output);

    return output;
}

vector<Vec4i> LaneDetector::getLines(Mat image)
{
    vector<Vec4i> lines;

    HoughLinesP(image, lines, 1, CV_PI / 180, 40, 30);

    return lines;
}

void LaneDetector::buildLines(vector<Vec4i> lines)
{
    vector<double> rightSlopes, leftSlopes;
    vector<Point> rightPoints, leftPoints;
    Vec4d right, left;

    // Filtering lines
    for (auto line : lines)
    {
        Point start = Point(line[0], line[1]);
        Point end = Point(line[2], line[3]);
        double slope = (end.y - start.y) / (double)(end.x - start.x);
        if (abs(slope) > 0.2 && abs(slope) < 10)
        {
            if (slope > 0)
            {
                rightPoints.push_back(start);
                rightPoints.push_back(end);
                rightSlopes.push_back(slope);
            }
            else
            {
                leftPoints.push_back(start);
                leftPoints.push_back(end);
                leftSlopes.push_back(slope);
            }
        }
    }

    // Detecting a left line
    if (leftPoints.size() > 0)
    {
        fitLine(leftPoints, left, DIST_L2, 0, 0.01, 0.01);
        leftM = average(leftSlopes);
        leftB = left[3] - leftM * left[2];
    }

    // Detecting a right line
    if (rightPoints.size() > 0)
    {
        fitLine(rightPoints, right, DIST_L2, 0, 0.01, 0.01);
        rightM = average(rightSlopes);
        rightB = right[3] - rightM * right[2];
    }
}

int LaneDetector::getDirection()
{
    double x = (leftB - rightB) / (rightM - leftM);
    double y = leftM * x + leftB;

    double centerX = sourceImage.cols / 2;
    double centerY = sourceImage.rows / 2;
    double rangeX = sourceImage.cols / 12;
    double rangeY = sourceImage.rows / 3;
    
    if (x < centerX - rangeX)
    {
        if (y > centerY + rangeY)
            return Direction::TURN_LEFT;
        else if (y > centerY - rangeY / 3)
            return Direction::GO_LEFT;
    }
    else if (x > centerX + rangeX)
    {
        if (y > centerY + rangeY)
            return Direction::TURN_RIGHT;
        else if (y > centerY - rangeY / 3)
            return Direction::GO_RIGHT;
    }

    return Direction::FORWARD;
}

int LaneDetector::drawLane()
{
    int direction = getDirection();
    double x = (leftB - rightB) / (rightM - leftM);
    double y = leftM * x + leftB;

    Point leftStart = Point(0, leftB);
    Point leftEnd = Point(x, y);

    Point rightStart = Point(sourceImage.cols, rightB + sourceImage.cols * rightM);
    Point rightEnd = Point(x, y);

    // Drawing two lines and intersection point
    circle(sourceImage, Point(x, y), 10, Scalar(0, 0, 255), 10);
    line(sourceImage, leftStart, leftEnd, Scalar(0, 255, 255), 10, LINE_AA);
    line(sourceImage, rightStart, rightEnd, Scalar(0, 255, 255), 10, LINE_AA);
    putText(sourceImage, to_string(direction), Point(100, 100), FONT_HERSHEY_TRIPLEX, 2, Scalar(0, 0, 255));

    // Show output
    namedWindow("Window", WINDOW_NORMAL);
    resizeWindow("Window", 600, 600);
    imshow("Window", sourceImage);
    
    return direction;
}
