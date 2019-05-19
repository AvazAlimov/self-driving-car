#include <softPwm.h>
#include <wiringPi.h>
#include <thread>

#ifndef MOTORMANAGER_H
#define MOTORMANAGER_H

#define IN1_PIN 1
#define IN2_PIN 4
#define IN3_PIN 5
#define IN4_PIN 6

enum Direction
{
	STOP = 0,
	FORWARD = 1,
	BACKWARD = 2,
	TURN_LEFT = 3,
	TURN_RIGHT = 4,
	GO_LEFT = 5,
	GO_RIGHT = 6
};

class MotorManager
{
	private:
		int mMinSpeed;
		int mMaxSpeed;
		int mSpeed;
		int mRotateSpeed;
	
	public:
		MotorManager(
			int minSpeed = 0,
			int maxSpeed = 100,
			int speed = 90,
			int rotateSpeed = 70
		);
		
		void setSpeed(int speed);
		void goForward();
		void goBackward();
		void turnRight();
		void turnLeft();
		void goRight();
		void goLeft();
		void stop();
		
		~MotorManager();
};
#endif
