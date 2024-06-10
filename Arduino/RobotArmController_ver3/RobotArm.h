/*
 * ロボットアームの雛形
 */
#include <Arduino.h>
#include "ServoController.h"

#ifndef ROBOTARM_H
#define ROBOTARM_H

//初期位置を指定(mm)
const int INITIAL_POS_X = 170;
const int INITIAL_POS_Y = 0;
const int INITIAL_POS_Z = 100;
//初期の手の開き度合い(degree)
const double INITIAL_FINGERS_ANGLE = 80.0;

//ロボットアームの長さ指定(mm)
const double UPPERARM = 229.0;
const double MIDDLEARM = 20.0;
const double FOREARM = 246.0;


enum ServoList
{
  SHOULDER_HORIZON,SHOULDER_VERTEX,ELBOW,WRIST,FINGERS,SERVONUM 
};


class RobotArm
{
  private:
  int x,y,z;
  ServoController *servoControllers[SERVONUM]; //サーボを保持

  public:
  //コンストラクタ
  RobotArm(ServoController *_servoControllers[SERVONUM]);
  //角度設定用
  void SetAngle(ServoList servo,float angle); 

  //位置指定:肩を基準として右手系の座標系で考えた場合の,手の位置を引数に取る
  void SetPosition(float x,float y,float z,bool handParallel = true);
};


#endif
