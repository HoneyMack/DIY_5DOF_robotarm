#include  "ServoController.h"

//define constructor
ServoController::ServoController(int8_t _pin, ServoSettings _servoSettings = initServoSettings) {
	pin = _pin;

	servoSettings = _servoSettings;

	//attach pin which outputs PWM.
	servo.attach(pin, servoSettings.minPulse, servoSettings.maxPulse);
}

void ServoController::SetAngleDEG(float _angle) {

  if (servoSettings.minAngle > _angle ){
    angle = servoSettings.minAngle;
  }
	else if (servoSettings.maxAngle < _angle) {
	  angle = servoSettings.maxAngle;
	}
  else{
		angle = _angle;
	}
	//Calculate nowPulse
  nowPulse = servoSettings.minPulse + (angle / 180.0) * (servoSettings.maxPulse - servoSettings.minPulse);   
	//Update servo
  servo.writeMicroseconds(nowPulse);
}

void  ServoController::SetAngleRAD(float _angle) {
	SetAngleDEG(_angle / PI * 180.0);
}

void ServoController::SetPulse(int pulse) {
	//if (pulse > servoSettings.minPulse && pulse < servoSettings.maxPulse) 
	{
		nowPulse = pulse;

		angle = ((nowPulse - servoSettings.minPulse)*180.0) / (servoSettings.maxPulse - servoSettings.minPulse);

		servo.writeMicroseconds(nowPulse);
	}
}

float ServoController::GetAngleDEG() {
	return angle;
}

float ServoController::GetAngleRAD() {
	return angle / 180.0 * PI;
}

void ServoController::SetServoSettings(ServoSettings _servoSettings) {
	servoSettings = _servoSettings;
	ReAttach();
}

void ServoController::ReAttach() {
	servo.detach();
	servo.attach(pin, servoSettings.minPulse, servoSettings.maxPulse);
}
