/*ロボットアーム制御プログラム:マイコンはサーボの制御に徹する*/
#include "RobotArm.h"


#define LEN 64 //一文字列で扱う最長文字列長

byte count; //現在受け取った文字列数
char string[LEN];//受信した文字列を保持


RobotArm *robotArm;
ServoController *servoController[SERVONUM];

void setup() {
	// ボーレートを指定して通信開始
	Serial.begin(19200);

	//サーボの情報をセット
	servoController[SHOULDER_HORIZON] = new ServoController(2);
	servoController[SHOULDER_VERTEX] = new ServoController(5);
	servoController[ELBOW] = new ServoController(6);
	servoController[WRIST] = new ServoController(9);
	servoController[FINGERS] = new ServoController(10);

	//ロボットアームのインスタンスを作成
	robotArm = new RobotArm(servoController);

	/*
	//初期値をEEPROMに保存
	strcpy(string,"M600 ");
	robotArm->ProcessOrder(string);
	strcpy(string,"M602 ");
	robotArm->ProcessOrder(string);
	*/
}

void loop() {
	if (Serial.available() > 0) {
		count = 0; //初期化
		string[0] = Serial.read();
		//受信した文字が改行でなければシリアル通信からデータ（文字）を受信し続ける
		while (string[count] != '\n' && count < LEN) {
			if (Serial.available() > 0) {
				string[++count] = Serial.read();
			}
		}
		string[count] = '\0';
		Serial.println("受信したデータ");
		Serial.println(string);

		//ロボットアームを操作
		robotArm->ProcessOrder(string);
	}
}
