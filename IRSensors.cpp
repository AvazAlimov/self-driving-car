#include "IRSensors.hpp"

// Initialize IR sensor pins and output mode
IRSensors::IRSensors(int leftPin, int rightPin, bool isReverse)
{
	// Exit app when setup fails
	if (wiringPiSetup() == -1) exit(-1);
	
	mLeftPin = leftPin;
	mRightPin = rightPin;
	mIsReverse = isReverse;
	pinMode(mLeftPin, INPUT);
	pinMode(mRightPin, INPUT);
}

// Get direction
int IRSensors::getDirection()
{
	int left = digitalRead(mLeftPin);
	int right = digitalRead(mRightPin);
	
	if (!left && right)
		return mIsReverse ? Direction::TURN_LEFT : Direction::TURN_RIGHT;
	else if (left && !right)
		return mIsReverse ? Direction::TURN_RIGHT : Direction::TURN_LEFT;
	else if (left && right)
		return Direction::FORWARD;
	else
		return Direction::BACKWARD;
}
