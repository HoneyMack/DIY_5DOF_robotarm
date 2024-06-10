/*
 * ロボットアームの雛形
 */
#include <Arduino.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <EEPROM.h>
#include "ServoController.h"
#include "Settings.h"

#ifndef ROBOTARM_H
#define ROBOTARM_H

 //使用するサーボのリスト
enum ServoList
{
	SHOULDER_HORIZON,SHOULDER_VERTEX,  ELBOW, WRIST, FINGERS, SERVONUM
};

//移動の仕方
enum MoveType {
	LINEAR,S_SHAPE
};

class RobotArm
{
private:
	bool handParallel;
	int x, y, z,startX,startY,startZ,desX,desY,desZ,nowFrame,finFrame;
	MoveType moveType;//移動の仕方を保持
	ServoController *servoControllers[SERVONUM]; //サーボを保持
	RobotArmSettings robotArmSettings;//ロボットアームに関する設定を保持

public:
	//コンストラクタ
	RobotArm(ServoController *_servoControllers[SERVONUM]);

	//ロボットアームの初期化を行うメソッド
	void Initialize_ROM();
	void Initialize_EEPROM();

	//位置指定:肩を基準として右手系の座標系で考えた場合の,手の位置を引数に取る
	void MovePosition(int _x, int _y, int _z);

	//終点位置とそれに関わる情報指定
	void SetPosition(int _x, int _y, int _z, int _finFrame);

	//ロボットアーム制御命令を解析し、適切な処理を行うメソッド
	void ProcessOrder(char* order);

	//ロボットアームの座標更新
	void UpdatePosition();
};

#endif
