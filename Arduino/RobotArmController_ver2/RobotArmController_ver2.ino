/*
 * ロボットアームの雛形
 */
#include "RobotArm.h"
#include "PutHandler.h" 


//移動量(mm)
#define WIDTH 150   //x
#define DEPTH 150   //y
#define HEIGHT 150  //z

#define ANGLE 90

enum PutHandlerName{
  X = 0,Y,Z,FINGER,PUTHANDLERNUM
};

RobotArm *robotArm;
ServoController *servoController[SERVONUM];
PutHandler putHandlers[PUTHANDLERNUM] = { PutHandler(0), PutHandler(1), PutHandler(2), PutHandler(3)};


int i,j;
float x,y,z,fingers;

 
void setup() {
  Serial.begin(115200);
  
  // put your setup code here, to run once:
  //サーボの情報をセット
  
  servoController[SHOULDER_HORIZON] = new ServoController(3);
  servoController[SHOULDER_VERTEX] = new ServoController(5);
  servoController[ELBOW] = new ServoController(6);
  servoController[WRIST] = new ServoController(9,500,2000);
  servoController[FINGERS] = new ServoController(10);

  //ロボットアームのインスタンスを作成
  robotArm = new RobotArm(servoController);
  
  //入出力の設定
  /*
  putHandlers[X] = new  PutHandler(0);
  putHandlers[Y] = new  PutHandler(1);
  putHandlers[Z] =  new PutHandler(2);
  putHandlers[FINGERS] = new  PutHandler(3);
  */
  /*  
  for(i = 0;i<SERVONUM;i++){
    robotArm->SetAngle(i,PI/2);
  }
  */
}

void loop() {
  // put your main code here, to run repeatedly:
  
  x = INITIAL_POS_X + WIDTH*( 2.0 * putHandlers[X].GetValue()/1024.0 - 1.0);
  y = INITIAL_POS_Y + DEPTH*( 2.0 * putHandlers[Y].GetValue()/1024.0 - 1.0);
  z = INITIAL_POS_Z + HEIGHT*(2.0 * putHandlers[Z].GetValue()/1024.0 - 1.0);
  robotArm->SetPosition(x,y,z); 

  fingers = (INITIAL_FINGERS_ANGLE + ANGLE*( 2.0 * putHandlers[FINGER].GetValue()/1024.0 - 1.0)) * (PI/180.0);
  robotArm->SetAngle(FINGERS,fingers);
  //robotArm->UpdateServos(0.01);
  
  //robotArm->SetPosition(0,0,0);
  
  //robotArm->SetAngle(SHOULDER_VERTEX,PI*(putHandlers[X].GetValue()/1024.0));
  //Serial.println(putHandler.GetValue());
  //Serial.print(x);Serial.print(',');Serial.print(y);Serial.print(',');Serial.print(z);Serial.print(',');Serial.println(fingers);
  //delay(10);
}



