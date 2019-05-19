#include <wiringPi.h>
#include <cstdlib>

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define TRIG_PIN 28
#define ECHO_PIN 29
#define	DISTANCE 10

class Ultrasonic
{
	private:
		long getDistance();
	public:
		Ultrasonic();
		bool canGo();
};
#endif
