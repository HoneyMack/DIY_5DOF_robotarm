/*シリアル通信で、一行文の命令を正しく受信できるかの確認

  概要：
 一行単位で命令を受信する。
*/
#define LEN 64 //一文字列で扱う最長文字列長

byte count; //現在受け取った文字列数
char string[LEN];//受信した文字列を保持

void setup() {
  // ボーレートを指定して通信開始
  Serial.begin(115200);
}

void loop() {
  count = 0; //初期化
  string[0] = ' '; 
  //受信した文字が改行でなければシリアル通信からデータ（文字）を受信し続ける
  while(string[count] != '\0' && count < LEN){
      if(Serial.available() > 0){
        string[count++] = Serial.read();
      }
  }
  //受信した文字列を送信
  Serial.write(string,count);
}
