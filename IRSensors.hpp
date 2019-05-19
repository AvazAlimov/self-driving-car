#include <wiringPi.h>
#include <thread>
#include "MotorManager.hpp"

#ifndef IRSENSORS_H
#define IRSENSORS_H

// Tracer pins: left = 10, right = 11
// IR pins: left = 27, right = 26

class IRSensors
{
	private:
		bool mIsReverse;
		int mLeftPin;
		int mRightPin;
		
	public:
		IRSensors(int leftPin, int rightPin, bool isReverse = false);
		int getDirection();
};
#endif
