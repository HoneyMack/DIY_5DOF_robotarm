/*
 * ロボットアームの雛形
 */
#include "RobotArm.h"
#include "IOPinHandler.h" 

//可動域(mm)
#define WIDTH 150   //x
#define DEPTH 150   //y
#define HEIGHT 150  //z

#define ANGLE 90

enum PinName{
  X = 0,Y,Z,FINGER,PUTHANDLERNUM
};

RobotArm *robotArm;
ServoController *servoController[SERVONUM];
//IOPinHandler IOpins[PUTHANDLERNUM] = { IOPinHandler(0), IOPinHandler(1), IOPinHandler(2), IOPinHandler(3)};


int i,j;
float x,y,z,fingers;

 
void setup() {
  Serial.begin(115200);
  
  // put your setup code here, to run once:
  //サーボの情報をセット
  
  servoController[SHOULDER_HORIZON] = new ServoController(2);
  servoController[SHOULDER_VERTEX] = new ServoController(5);
  servoController[ELBOW] = new ServoController(6);
  servoController[WRIST] = new ServoController(9,500,2000);
  servoController[FINGERS] = new ServoController(10);

  //ロボットアームのインスタンスを作成
  robotArm = new RobotArm(servoController);
  
  //入出力の設定
  /*
  IOpins[X] = new  IOPinHandler(0);
  IOpins[Y] = new  IOPinHandler(1);
  IOpins[Z] =  new IOPinHandler(2);
  IOpins[FINGERS] = new  IOPinHandler(3);
  */
   
  for(i = 0;i<SERVONUM;i++){
    robotArm->SetAngle(i,PI/2);
  }
}

void loop() {

}
