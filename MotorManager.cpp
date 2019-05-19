#include "MotorManager.hpp"

// Initialize motor pins
MotorManager::MotorManager(
	int minSpeed,
	int maxSpeed,
	int speed,
	int rotateSpeed
)
{
	// Exit app if setup failed
	if (wiringPiSetup() == -1) exit(-1);
	
	mMinSpeed = minSpeed;
	mMaxSpeed = maxSpeed;
	mSpeed = speed;
	mRotateSpeed = rotateSpeed;
	
	pinMode(IN1_PIN, SOFT_PWM_OUTPUT);
	pinMode(IN2_PIN, SOFT_PWM_OUTPUT);
	pinMode(IN3_PIN, SOFT_PWM_OUTPUT);
	pinMode(IN4_PIN, SOFT_PWM_OUTPUT);
	
	softPwmCreate(IN1_PIN, mMinSpeed, mMaxSpeed);
	softPwmCreate(IN2_PIN, mMinSpeed, mMaxSpeed);
	softPwmCreate(IN3_PIN, mMinSpeed, mMaxSpeed);
	softPwmCreate(IN4_PIN, mMinSpeed, mMaxSpeed);
}

// Release motor pins
MotorManager::~MotorManager()
{
	softPwmWrite(IN1_PIN, mMinSpeed);
	softPwmWrite(IN2_PIN, mMinSpeed);
	softPwmWrite(IN3_PIN, mMinSpeed);
	softPwmWrite(IN4_PIN, mMinSpeed);
	
	pinMode(IN1_PIN, INPUT);
	pinMode(IN2_PIN, INPUT);
	pinMode(IN3_PIN, INPUT);
	pinMode(IN4_PIN, INPUT);
}


void MotorManager::setSpeed(int speed)
{
	mSpeed = speed;
}

// Go forward with delay
void MotorManager::goForward()
{
	softPwmWrite(IN1_PIN, mSpeed);
	softPwmWrite(IN2_PIN, mMinSpeed);
	softPwmWrite(IN3_PIN, mSpeed);
	softPwmWrite(IN4_PIN, mMinSpeed);
}

// Go backward with delay
void MotorManager::goBackward()
{
	softPwmWrite(IN1_PIN, mMinSpeed);
	softPwmWrite(IN2_PIN, mSpeed);
	softPwmWrite(IN3_PIN, mMinSpeed);
	softPwmWrite(IN4_PIN, mSpeed);
}

// Turn right at the point with delay
void MotorManager::turnRight()
{
	softPwmWrite(IN1_PIN, mSpeed);
	softPwmWrite(IN2_PIN, mMinSpeed);
	softPwmWrite(IN3_PIN, mMinSpeed);
	softPwmWrite(IN4_PIN, mSpeed);
}

// Turn left at the point with delay
void MotorManager::turnLeft()
{
	softPwmWrite(IN1_PIN, mMinSpeed);
	softPwmWrite(IN2_PIN, mSpeed);
	softPwmWrite(IN3_PIN, mSpeed);
	softPwmWrite(IN4_PIN, mMinSpeed);
}

// Turn right with delay
void MotorManager::goRight()
{
	softPwmWrite(IN1_PIN, mRotateSpeed);
	softPwmWrite(IN2_PIN, mMinSpeed);
	softPwmWrite(IN3_PIN, mMinSpeed);
	softPwmWrite(IN4_PIN, mMinSpeed);
}

// Turn left with delay
void MotorManager::goLeft()
{
	softPwmWrite(IN1_PIN, mMinSpeed);
	softPwmWrite(IN2_PIN, mMinSpeed);
	softPwmWrite(IN3_PIN, mRotateSpeed);
	softPwmWrite(IN4_PIN, mMinSpeed);
}

// Stop motors
void MotorManager::stop()
{
	softPwmWrite(IN1_PIN, mMinSpeed);
	softPwmWrite(IN2_PIN, mMinSpeed);
	softPwmWrite(IN3_PIN, mMinSpeed);
	softPwmWrite(IN4_PIN, mMinSpeed);
}
