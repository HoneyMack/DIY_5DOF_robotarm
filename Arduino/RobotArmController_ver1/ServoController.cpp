#include  "ServoController.h"

//define constructor
ServoController::ServoController(int8_t _pin,int16_t _minPulse = MINPULSE,int16_t _maxPulse = MAXPULSE){
  pin = _pin;
  minPulse = _minPulse;
  maxPulse = _maxPulse;

  //attach pin which outputs PWM.
  servo.attach(pin,minPulse,maxPulse); 
}

void ServoController::SetAngle(float _angle){
  angle = _angle;

  //Calculate nowPulse
  nowPulse = minPulse + angle/180.0f *(maxPulse - minPulse);
  //Update servo
  servo.writeMicroseconds(nowPulse);
}

