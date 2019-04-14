#include "RN4020.h"
#include "key_settings.h"
void setup() {
  Serial.begin(115200); // ハードウェアシリアルを準備
  pinMode(PIN_TEST_LED, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  Serial.println("/*****Arduino UNO*****/");
  Serial.println("/      Peripheral     /");
  Serial.println("/   BLE Setting now   /");
  Initialize();
  // 回転速度調整
  analogWrite(PIN_VREF, 127);
}

void loop() {
  delay(10);
  pollingBTcmd();
}

//RN4020 書き込みコマンド受信ポーリング
void pollingBTcmd() {
  int ret=getBLEAccess();
  if(ret<0){
    if(ret==1){
      Trigger_ON();
    }else{
      Trigger_OFF();
    }
    Serial.print("!GET FOUND!");
  }
}


void Trigger_ON() {
  // 回転
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  delay(5000);
  // ブレーキ
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, HIGH);
}

void Trigger_OFF() {
  // 逆回転
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  delay(5000);
  // ブレーキ
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, HIGH);
}

