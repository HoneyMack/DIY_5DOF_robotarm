#include <Arduino.h>
#include <Servo.h>
#include "Settings.h"


#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H


//ServoController is class that treats servo. 
class ServoController {
private:
	int8_t pin;			//this pin outputs PWM.
	Servo servo;
	float angle;		//servo's angle ( 0 ~ 180 (degree) )

public:
	int16_t nowPulse;  //Recent PWM Pulse time (minPulse ~ maxPulse  (us)),PWM minimum Pulse time (us),PWM maximum Pulse time (us)
	ServoSettings servoSettings;		 //ÉTÅ[É{ÇÃê›íËÇï€éù
	//constructor
	ServoController(int8_t _pin, ServoSettings _servoSettings = initServoSettings);
	void SetAngleDEG(float _angle);
	void SetAngleRAD(float _angle);
	void SetPulse(int pulse);
	float GetAngleDEG();
	float GetAngleRAD();

	void SetServoSettings(ServoSettings _servoSettings);
	void ReAttach();
};

#endif