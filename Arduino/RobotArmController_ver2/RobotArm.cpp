#include "RobotArm.h"
#include <math.h>

//define constructor
RobotArm::RobotArm(ServoController *_servoControllers[SERVONUM]){
  for(int i = 0;i < SERVONUM;i++){
    servoControllers[i] = _servoControllers[i];
  }
}

//サーボを指定して角度を設定
void RobotArm::SetAngle(ServoList servo,float angle){
    servoControllers[servo]->SetAngle(angle);
}

//位置指定

void RobotArm::SetPosition(float x,float y,float z,bool handParallel = true){
  //Serial.println("CalculatePosition");
  //角度保持
  double psi = 0.0,gamma = 0.0,theta = 0.0 ,alpha = 0.0 ,beta = 0.0,phi = 0.0,kappa = 0.0,myu = 0.0,upperArm;
  upperArm = sqrt(UPPERARM*UPPERARM + MIDDLEARM*MIDDLEARM);
  psi = atan2(y,x);
  gamma = atan2(z,sqrt(x*x + y*y));
  myu = atan2(MIDDLEARM,UPPERARM);
  kappa = PI/2 - myu;
  theta = acos((upperArm*upperArm + x*x + y*y +z*z - FOREARM*FOREARM)/(2.0 * upperArm *sqrt(x*x + y*y + z*z)));
  alpha = acos((upperArm*upperArm -( x*x + y*y + z*z) + FOREARM*FOREARM)/(2.0 * upperArm *FOREARM));
  phi = PI - alpha -kappa;
  beta = alpha + gamma + theta+kappa;

  //それぞれのサーボに命令を送る
  servoControllers[SHOULDER_HORIZON]->SetAngle(PI/2 + psi);
  servoControllers[SHOULDER_VERTEX]->SetAngle(gamma+theta+myu);
  servoControllers[ELBOW]->SetAngle(phi + PI/2);
  if(handParallel)
    servoControllers[WRIST]->SetAngle(beta- PI);

  /*
  Serial.print("Shoulder_H:");Serial.print(servoControllers[SHOULDER_HORIZON]->GetAngle()*180/PI);Serial.print(',');
  Serial.print("Shoulder_V:");Serial.print(servoControllers[SHOULDER_VERTEX]->GetAngle()*180/PI);Serial.print(',');
  Serial.print("Elbow:");Serial.print(servoControllers[ELBOW]->GetAngle()*180/PI);Serial.print(',');
  Serial.print("Wrist:");Serial.print(servoControllers[WRIST]->GetAngle()*180/PI);Serial.println(' ');
  */
  /*
  Serial.print("psi:");Serial.print(psi *180/PI);Serial.print(',');
  Serial.print("gamma:");Serial.print(gamma *180/PI);Serial.print(',');
  Serial.print("theta:");Serial.print(theta *180/PI);Serial.print(',');
  Serial.print("phi:");Serial.print(phi *180/PI);Serial.print(',');
  Serial.print("alpha:");Serial.print(alpha *180/PI);Serial.print(',');
  Serial.print("beta:");Serial.print(beta *180/PI);Serial.print(',');
  Serial.print("upperArm:");Serial.print(upperArm);Serial.print(',');
  Serial.print("myu:");Serial.print(myu *180/PI);Serial.print(',');
  Serial.print("kappa:");Serial.print(kappa *180/PI);Serial.println(',');
  */
}

