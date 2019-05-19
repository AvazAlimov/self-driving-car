#include "Ultrasonic.hpp"

Ultrasonic::Ultrasonic()
{
	pinMode(TRIG_PIN, OUTPUT);
	pinMode(ECHO_PIN, INPUT);
}

long Ultrasonic::getDistance()
{
	digitalWrite(TRIG_PIN, HIGH);
	delayMicroseconds(20);
	digitalWrite(TRIG_PIN, LOW);

	int delay = 100000;
	int delta = 0;
	while(digitalRead(ECHO_PIN) == LOW && delta < delay) { delta++; }
	delta = 0;
	long start = micros();
	while(digitalRead(ECHO_PIN) == HIGH && delta < delay) { delta++; }
	long end = micros();
	long distance = (end - start) / 58;
	
	return distance;
}

bool Ultrasonic::canGo()
{
	long prev = getDistance(), next;
	while(std::abs(next - prev) > 10)
	{
		prev = next;
		next = getDistance();
	}
	return next > DISTANCE;
}
