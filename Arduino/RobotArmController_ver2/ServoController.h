#include <Arduino.h>
#include <Servo.h>

#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

//servo initial value
#define MINPULSE 500 //(us)
#define MAXPULSE 2500 //(us)

//ServoController is class that treats servo. 
class ServoController{
private:
  int8_t pin;            //this pin outputs PWM.
  int16_t nowPulse,minPulse,maxPulse;  //Recent PWM Pulse time (minPulse ~ maxPulse  (us)),PWM minimum Pulse time (us),PWM maximum Pulse time (us)

  float angle;        //servo's angle ( 0 ~ 180 (degree) )

  Servo servo;
  
public:
  //constructor
  ServoController(int8_t _pin,int16_t _minPulse = MINPULSE,int16_t _maxPulse = MAXPULSE); 
  void SetAngle(float _angle);
  float GetAngle();
};

#endif
