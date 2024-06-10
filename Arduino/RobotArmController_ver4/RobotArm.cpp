#include "RobotArm.h"

#define MAX_LEN 8
#define MAX_WORDS_NUM 8

//define constructor
RobotArm::RobotArm(ServoController *_servoControllers[SERVONUM]) {
	for (int i = 0; i < SERVONUM; i++) {
		servoControllers[i] = _servoControllers[i];
	}
	Initialize_EEPROM();
}

//ロボットアームの初期化を行うメソッド
void RobotArm::Initialize_ROM() {
	int i;
	ServoSettings settings;
	//ROMから設定を取り出す
	Serial.println("RobotArmInitialization_ROM");
	//ロボットアームに関する設定を取り出して設定
	robotArmSettings = initRobotArmSettings;

	//各サーボに関する設定をROMから取り出して設定
	for (i = 0; i < SERVONUM; i++) {
		servoControllers[i]->SetServoSettings(initServoSettings);
	}
	//初期位置に移動
	SetPosition(robotArmSettings.initPosX, robotArmSettings.initPosY, robotArmSettings.initPosZ);

	//手の開閉を調整
	servoControllers[FINGERS]->SetAngleDEG(robotArmSettings.initFingerAngle);
}

//ロボットアームの初期化を行うメソッド
void RobotArm::Initialize_EEPROM() {
	int i;
	ServoSettings settings;
	//EEPROMから設定を取り出す
	Serial.println("RobotArmInitialization");
	//ロボットアームに関する設定を取り出す
	EEPROM.get(EEP_FADRS, robotArmSettings);

	//各サーボに関する設定を取り出す
	for (i = 0; i < SERVONUM; i++) {
		EEPROM.get(EEP_FADRS + sizeof(RobotArmSettings) + sizeof(ServoSettings)*i, settings);
		servoControllers[i]->SetServoSettings(settings);
	}
	//初期位置に移動
	SetPosition(robotArmSettings.initPosX, robotArmSettings.initPosY, robotArmSettings.initPosZ);

	//手の開閉を調整
	servoControllers[FINGERS]->SetAngleDEG(robotArmSettings.initFingerAngle);
}

//位置指定
void RobotArm::SetPosition(int _x, int _y, int _z, bool handParallel = true) {
	//位置情報を保存
	this->x = _x;
	this->y = _y;
	this->z = _z;

  //作業変数の値の大きさの関係でintをlongに
  long x = _x,y=_y,z = _z;  

	//角度保持
	float theta, theta0, theta1, alpha, alpha_, beta, gamma, delta;
	//長さ保持
	float p;
	float l1 = robotArmSettings.upperArmLength, l1_, l2 = robotArmSettings.middleArmLength, l3 = robotArmSettings.foreArmLength;

	p = sqrt(x*x + y * y + z * z);
	l1_ = sqrt(l1*l1 + l2 * l2);

	theta = acos((l1*l1 + l2 * l2 + p * p - l3 * l3) / (2 * l1_*p));
	theta0 = atan2(l2, l1);
	theta1 = atan2(z, sqrt(x * x + y * y));
	alpha = acos((l1*l1 + l2 * l2 + l3 * l3 - p * p) / (2 * l1_*l3));
	alpha_ = alpha - theta0;
	beta = alpha + theta - PI / 2 + theta1;
	gamma = theta + theta0 + theta1;
	delta = atan2(y, x);

	//nan判定
	if (isnan(p)) {
		Serial.println("p_nan");
	}
	if (isnan(theta)) {
		Serial.println("theta_nan");
	}
	if (isnan(alpha)) {
		Serial.println("alpha_nan");
	}

	//それぞれのサーボに命令を送る
	servoControllers[SHOULDER_HORIZON]->SetAngleRAD(delta);
	servoControllers[SHOULDER_VERTEX]->SetAngleRAD(gamma);
	servoControllers[ELBOW]->SetAngleRAD(PI - alpha_);
	if (handParallel)
		servoControllers[WRIST]->SetAngleRAD(beta);
}

//ロボットアーム制御命令を解析し、適切な処理を行うメソッド(引数には配列を渡してください[文字列リテラルはNG])
void RobotArm::ProcessOrder(char* order) {
	int i;
	int x1 = x, y1 = y, z1 = z, f1 = robotArmSettings.initFrame;
	int w_count = 0;
	char c;
	char *p;
	char words[MAX_WORDS_NUM][MAX_LEN]; //orderを区切り文字で分割した単語たちを保持
	float angle;


	//文字列を区切り文字" "で分割
	p = strtok(order, " ");
	//オペランドがなくなるまで続ける
	while (p != NULL) {
		//オペランドの部分をコピー
		strcpy(words[w_count++], p);
		//文字列（オペランド)を区切り文字" "で分割
		p = strtok(NULL, " ");
	}

	//命令ごとに分岐
	if (strcmp(words[0], "G90") == 0) {
		//アブソリュート命令
		Serial.println("Command G90");
		c = 'L';

		//各オペランドの値を取り出す
		while (--w_count >= 1) {
			switch (words[w_count][0]) {
			case 'X':
				Serial.println("X");
				x1 = atoi(&words[w_count][1]);
				break;
			case 'Y':
				Serial.println("Y");
				y1 = atoi(&words[w_count][1]);
				break;
			case 'Z':
				Serial.println("Z");
				z1 = atoi(&words[w_count][1]);
				break;
			case 'F':
				Serial.println("F");
				f1 = atoi(&words[w_count][1]);
				break;
			case 'S':
				Serial.println("change:p");
				c = 'S';
				break;
			}
		}
		//位置を移動する
		if (c == 'L') {
			//直線補間で移動
			SetPosition(x1, y1, z1);
		}
		else {
			//S字補間で移動
		}
	}
	else if (strcmp(words[0], "G91") == 0) {
		//インクリメンタル命令
		Serial.println("Command G91");

	}
	else if (strcmp(words[0], "G92") == 0) {
		//サーボ角度指定
		Serial.println("Command G92");
		//サーボを回転させる
		for (i = 1; i < w_count; i += 2) {
			if (words[i][0] == 'S' && words[i + 1][0] == 'A')
				servoControllers[atoi(&words[i][1])]->SetAngleDEG(atoi(&words[i + 1][1]));
		}
	}
	else if (strcmp(words[0], "G93") == 0) {
		//サーボ角度指定
		Serial.println("Command G93");
		//サーボへのパルス幅を指定
		for (i = 1; i < w_count; i += 2) {
			if (words[i][0] == 'S' && words[i + 1][0] == 'P')
				servoControllers[atoi(&words[i][1])]->SetPulse(atoi(&words[i + 1][1]));
		}
	}
	else if (strcmp(words[0], "M500") == 0) {
		//サーボのパルス幅設定
		if (words[1][0] == 'S') {
			Serial.println("Command M500");
			//サーボの番号を取り出す
			int index = atoi(&words[1][1]);
			ServoSettings settings = servoControllers[index]->servoSettings;
			//各オペランドの値を取り出す
			while (--w_count >= 2) {
				switch (words[w_count][0]) {
				case 'M':
					Serial.println("M");
					settings.maxPulse = atoi(&words[w_count][1]);
					break;
				case 'm':
					Serial.print("m");
					settings.minPulse = atoi(&words[w_count][1]);
					break;
				}
			}
			servoControllers[index]->SetServoSettings(settings);
		}
	}
	else if (strcmp(words[0], "M501") == 0) {
		//サーボ最大・最小角度設定
		if (words[1][0] == 'S') {
			Serial.println("Command M501");
			//サーボの番号を取り出す
			int index = atoi(&words[1][1]);
			ServoSettings settings = servoControllers[index]->servoSettings;
			//各オペランドの値を取り出す
			while (--w_count >= 2) {
				switch (words[w_count][0]) {
				case 'M':
					Serial.println("M");
					settings.maxAngle = atoi(&words[w_count][1]);
					break;
				case 'm':
					Serial.print("m");
					settings.minAngle = atoi(&words[w_count][1]);
					break;
				}
			}
			servoControllers[index]->SetServoSettings(settings);
		}
	}
	else if (strcmp(words[0], "M600") == 0) {
		Serial.println("Command M600");
		Initialize_ROM();
	}
	else if (strcmp(words[0], "M601") == 0) {
		Serial.println("Command M601");
		Initialize_EEPROM();
	}
	else if (strcmp(words[0], "M602") == 0) {
		//現在の設定をEEPROMに書き込む
		Serial.println("Command M602");
		//ロボットアームに関する設定を保存
		EEPROM.put(EEP_FADRS, robotArmSettings);
		//各サーボに関する設定を保存
		for (i = 0; i < SERVONUM; i++) {
			EEPROM.put(EEP_FADRS + sizeof(RobotArmSettings) + sizeof(ServoSettings)*i,
				servoControllers[i]->servoSettings);
		}
	}
	else if (strcmp(words[0], "M604") == 0) {
		//サーボ角度更新
		Serial.println("Command M604");
		for (i = 0; i < SERVONUM; i++) {
			angle = servoControllers[i]->GetAngleDEG();
			servoControllers[i]->SetAngleDEG(angle);
		}
	}
	else if (strcmp(words[0], "M605") == 0) {
		//位置更新
		Serial.println("Command M605");
		SetPosition(x, y, z);
	}
	else if (strcmp(words[0], "M666") == 0) {
		//設定の表示
		char buf[64];

		Serial.println("Command M666");
		Serial.print("initPos(x,y,z) = ");
		sprintf(buf, "(%d,%d,%d)",
			robotArmSettings.initPosX,
			robotArmSettings.initPosY,
			robotArmSettings.initPosZ
		);
		Serial.println(buf);

		Serial.print("Fingers:");
		Serial.println(robotArmSettings.initFingerAngle);

		Serial.print("(uArm,mArm,fArm) = (");
		Serial.print(robotArmSettings.upperArmLength); Serial.print(",");
		Serial.print(robotArmSettings.middleArmLength); Serial.print(",");
		Serial.print(robotArmSettings.foreArmLength); Serial.println(")");

		for (i = 0; i < SERVONUM; i++) {
			sprintf(buf, "[%d]", i);
			Serial.print(buf);
			Serial.print("(minPulse,maxPulse,minAngle,maxAngle) = ");
			sprintf(buf, "(%d,%d,%d,%d)",
				servoControllers[i]->servoSettings.minPulse,
				servoControllers[i]->servoSettings.maxPulse,
				servoControllers[i]->servoSettings.minAngle,
				servoControllers[i]->servoSettings.maxAngle
			);
			Serial.println(buf);
		}
	}
	else if (strcmp(words[0], "M667") == 0) {
		//現在の状態の表示
		char buf[64];

		Serial.println("Command M667");
		Serial.print("Pos(x,y,z) = ");
		sprintf(buf, "(%d,%d,%d)", x, y, z);
		Serial.println(buf);

		Serial.print("Fingers:");
		Serial.println(servoControllers[FINGERS]->GetAngleDEG());

		for (i = 0; i < SERVONUM; i++) {
			sprintf(buf, "[%d]", i);
			Serial.print(buf);
			Serial.print("(nowPulse,nowAngle) = (");
			Serial.print(servoControllers[i]->nowPulse); Serial.print(",");
			Serial.print(servoControllers[i]->GetAngleDEG()); Serial.println(")");
		}
	}
	else if (strcmp(words[0], "H") == 0) {
	//初期位置に移動
	SetPosition(robotArmSettings.initPosX, robotArmSettings.initPosY, robotArmSettings.initPosZ);
	}
}
