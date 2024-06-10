/*EPPROMで保存しておくデータの型を定義*/
#ifndef SETTINGS_H
#define SETTINGS_H
//EPPROMの最初のアドレス
#define EEP_FADRS 0

//ロボットアームの設計仕様に関する情報
struct RobotArmSettings {
	int initPosX, initPosY, initPosZ,initFrame;		//初期位置
	float initFingerAngle;					//指の開き具合
	float upperArmLength, middleArmLength, foreArmLength; //それぞれの腕の長さ(mm)
};

//各サーボに関する情報
struct ServoSettings {
	int16_t minPulse, maxPulse;
	int16_t minAngle, maxAngle;
};


/*----初期化用初期値----*/
//ロボットアームに関する設定

//初期位置(mm)
const int INITIAL_POS_X = 100;
const int INITIAL_POS_Y = 1;
const int INITIAL_POS_Z = 1;
//一回の移動に要するフレーム数
const int INITIAL_FRAME = 10;

//初期の手の開き度合い(degree)
const double INITIAL_FINGERS_ANGLE = 40.0;

//ロボットアームの長さ指定(mm)
const double UPPERARM = 229.0;  //上腕
const double MIDDLEARM = 20.0;  //肘
const double FOREARM = 247.0;   //前腕


//サーボ最小パルス、最長パルス指定
const int MINPULSE = 400;	//(us)
const int MAXPULSE = 2500;	//(us)

//ロボットアームに関する設定
const RobotArmSettings initRobotArmSettings =
{ INITIAL_POS_X,INITIAL_POS_Y,INITIAL_POS_Z,INITIAL_FRAME,
  INITIAL_FINGERS_ANGLE,
  UPPERARM,MIDDLEARM,FOREARM};

//サーボに関する設定
const ServoSettings initServoSettings = { MINPULSE,MAXPULSE,-10,190 };

#endif
