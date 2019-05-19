#!/bin/bash

g++ main.cpp 					\
	MotorManager.cpp 			\
	IRSensors.cpp 				\
	LaneDetector.cpp 			\
	Ultrasonic.cpp 				\
	Permission.cpp				\
	SignDetector.cpp			\
	TrafficLightDetector.cpp	\
	-o app						\
	-lwiringPi -lpthread 		\
	-I/usr/local/include 		\
	-L/usr/local/lib -L/opt/vc/lib \
	-lraspicam_cv -lmmal -lmmal_core -lmmal_util \
	`pkg-config --cflags --libs opencv`
