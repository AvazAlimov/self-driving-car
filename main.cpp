#include <iostream>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include <wiringPi.h>
#include <thread>
#include <unistd.h>
#include <csignal>

#include "MotorManager.hpp"
#include "IRSensors.hpp"
#include "LaneDetector.hpp"
#include "Ultrasonic.hpp"
#include "SignDetector.hpp"
#include "Permission.hpp"
#include "TrafficLightDetector.hpp"

#define MAX_SPEED 40
#define MIN_SPEED 25

using namespace cv;
using namespace std;
using namespace raspicam;

bool captured = false;
bool finish = false;
bool stopSignFlag = false;
int numberOfDetectedObstacles = 0;

MotorManager motor(0, 100, MAX_SPEED, 70);
RaspiCam_Cv camera;
Mat image;

Permission trafficLightPermissions(0);
Permission lanePermissions(2);
Permission irsPermissions(2);
Permission tracerPermissions(2);
Permission signPermissions(2);

void exitSignalHandler(int signalNumber);
void captureImage();
void detectTrafficLight(Permission &permission);
void detectLane(Permission &permission);
void listenIRs(Permission &permission);
void listenTracers(Permission &permission);
void listenSigns(Permission &permission);
void listenUltrasonic(Permission &permission);
void move(int direction);
void test();

int main()
{
	signal(SIGINT, exitSignalHandler);
	
	thread imageCapturerThread(captureImage);
	thread trafficLightDetectionThread(detectTrafficLight, ref(trafficLightPermissions));
	thread laneDetectorThread(detectLane, ref(lanePermissions));
	thread irsListenerThread(listenIRs, ref(irsPermissions));
	thread tracersListenerThread(listenTracers, ref(tracerPermissions));
	thread signListenerThread(listenSigns, ref(signPermissions));
	//thread ultrasonicListenerThread(listenUltrasonic, ref(permissions[3]));

	while(!finish)
	{
		if(!signPermissions.canGo)
		{
			motor.stop();
			delay(5000);
			signPermissions.canGo = true;
		}
		
		while(!trafficLightPermissions.canGo)
		{
			motor.stop();
			delay(1000);
		}
		
		// Turn back to lane
		while(
			tracerPermissions.direction == Direction::TURN_LEFT ||
			tracerPermissions.direction == Direction::TURN_RIGHT
		)
		{
			if(tracerPermissions.direction == Direction::BACKWARD)
			{
				cout << "Skipping line" << endl;
				cout << "Lane direction " << lanePermissions.direction << endl;
			}
			move(tracerPermissions.direction);
		}
		
		// Move away from side obstacles
		// FIXME: - Work in progress
		while(
			irsPermissions.direction == Direction::BACKWARD
		)
		{
			// move(irsPermissions.direction);
			test();
		}
		
		
		// Stop if there is an obstacle at the front
		//while(!permissions[3].canGo) motor.stop();
		
		// Get direction from lane detector
		int direction = lanePermissions.direction;
		
		move(direction);
	}
	return 0;
}

void exitSignalHandler(int signalNumber)
{
	camera.release();
	motor.~MotorManager();
	exit(1);
}

void captureImage()
{
	camera.set(CAP_PROP_FRAME_WIDTH, 640);
	camera.set(CAP_PROP_FRAME_HEIGHT, 480);
	camera.open();
	while(true)
	{
		camera.grab();
		camera.retrieve(image);
		captured = true;
		//imshow("Original", image);
		//waitKey(1);
	}
}

void detectTrafficLight(Permission &permission)
{
	while(true)
	{
		if(captured)
		{
			permission.canGo = true;//!TrafficLightDetector(image).detect();
		}
	}
}

void detectLane(Permission &permission)
{
	while(true)
	{
		if (captured)
		{
			LaneDetector detector(image);
			permission.direction = detector.getDirection();
		}
	}
}

void listenIRs(Permission &permission)
{
	IRSensors irs(27, 26);
	while(true)
	{
		permission.direction = irs.getDirection();
	}
}

void listenTracers(Permission &permission)
{
	IRSensors tracers(10, 11);
	while(true)
	{
		permission.direction = tracers.getDirection();
	}
}

void listenSigns(Permission &permission)
{
	while(true)
	{
		if (captured)
		{
			Mat frame;
			resize(image, frame, Size(640, 480));
			
			Rect myROI(0, 0, 640, 240);
			frame = frame(myROI);
    
			SignDetector signDetector = SignDetector();
			vector<int> detectedSigns;
			detectedSigns = signDetector.detectSign(frame);
			
			if (!detectedSigns.empty()) {
				int detectedSign = detectedSigns[0];
				
				if(detectedSign == Sign::SIGN_LEFT) {
					// FIXME: 
					cout << "LEFT DETECTED" << endl;
				} else if(detectedSign == Sign::SIGN_RIGHT) {
					// FIXME:
					cout << "RIGHT DETECTED" << endl;
				} else if(detectedSign == Sign::SIGN_PARKING) {
					// FIXME:
					cout << "PARKING DETECTED" << endl;
					motor.stop();
					finish = true;
				} else if(detectedSign == Sign::SIGN_STOP) {
					if (!stopSignFlag)
					{
						stopSignFlag = true;
						permission.canGo = false;
						cout << "STOP DETECTED" << endl;
					}
				} else if(detectedSign == Sign::SIGN_PEDESTRIAN) {
					cout << "PEDESTRIAN ENTER" << endl;
					motor.setSpeed(MIN_SPEED);
					while(tracerPermissions.direction != Direction::BACKWARD);
					delay(1000);
					motor.setSpeed(MAX_SPEED);
					cout << "PEDESTRIAN LEAVE" << endl;
				}
			}
		}
	}
}

void listenUltrasonic(Permission &permission)
{
	Ultrasonic ultrasonic;
	while(true)
	{
		permission.canGo = ultrasonic.canGo();
	}
}

void move(int direction)
{
	//direction = -1;
	switch(direction)
	{
		case Direction::BACKWARD:
			motor.goBackward();
			break;
		case Direction::FORWARD:
			motor.goForward();
			break;
		case Direction::TURN_LEFT:
			motor.turnLeft();
			break;
		case Direction::TURN_RIGHT:
			motor.turnRight();
			break;
		case Direction::GO_LEFT:
			motor.goLeft();
			break;
		case Direction::GO_RIGHT:
			motor.goRight();
			break;
		default:
			motor.stop();
			break;
	}
}


// MARK: - Obstacle turnaround

void test() {
	double delayTime = 600;
	double coefficient = 0.8;
	
	// MARK: - Pedestrian Girl
	
	if(numberOfDetectedObstacles == 0) {
		cout<<"Obstacle #0 - Waiting 6 seconds\n";
		int pedestrianTimeOut = 6000;
		motor.stop();
		delay(pedestrianTimeOut);
	}
	
	// MARK: - Obstacle #1
	
	else if(numberOfDetectedObstacles == 1) {
		cout<<"Obstacle #1 - turnaround left\n";
		motor.turnLeft();
		delay(delayTime);
		motor.goForward();
		delay(delayTime*coefficient);
		motor.turnRight();
	}
	
	// MARK: - Obstacle #2
	
	else if(numberOfDetectedObstacles == 2) {
		cout<<"Obstacle #2 - turnaround right\n";
		motor.turnRight();
		delay(delayTime);
		motor.goForward();
		delay(delayTime*coefficient);
		motor.turnLeft();
	}
	
	numberOfDetectedObstacles++;
	
	// FIXME - Obstacle Counter Reset
	
	if(numberOfDetectedObstacles > 2) {
		cout<<"Obstacle Counter Reset\n";
		numberOfDetectedObstacles = 0;
	}
}
