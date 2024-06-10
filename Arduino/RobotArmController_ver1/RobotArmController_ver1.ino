#include "ServoController.h"

#define NUM 1 //The number of servos

int i,j;
ServoController *servos[NUM];

void setup() {
  //サーボ信号出力ピン設定
  servos[0] = new ServoController(12);
  //コントローラの入力ピン設定
}

void loop() {
  //read Analog pin0
  i = analogRead(0);
  servos[0]->SetAngle(i/1023.0f * 180.0);
  delay(10);
}
