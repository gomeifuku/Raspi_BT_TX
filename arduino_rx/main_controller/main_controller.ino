#include "RN4020.h"
#include "key_settings.h"

volatile int isLightON = 0;

void setup() {
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_FOWARD_STOP, INPUT);
  pinMode(PIN_REVERSE_STOP, INPUT);
  pinMode(PIN_DBG_LED1, OUTPUT);
  Initialize();
  attachInterrupt(0, TriggerLigthOn, FALLING);
  attachInterrupt(1, TriggerLigthOff, FALLING);
  // 回転速度調整
  analogWrite(PIN_VREF, 127);
  /*Light OFFの状態までモータを回す*/
}

void loop() {
  delay(10);
  pollingBTcmd();
}

//RN4020 書き込みコマンド受信ポーリング
void pollingBTcmd() {
  int ret=getBLEAccess();
  if(ret==1){
    if(isLightON){
      Trigger_OFF();
    }else{
      Trigger_ON();      
    }
  }
}
/*モータ正転*/
void RotateFoward(){
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
}
/*モータ逆転*/
void RotateReverse(){
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
}
/*モータ正転*/
void RotateStop(){
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, HIGH);
}

void Trigger_ON() {
  // 回転
  RotateFoward();
  delay(5000);
  // SANITY::ブレーキ
  RotateStop();
}

void Trigger_OFF() {
  // 逆回転
  RotateReverse();
  delay(5000);
  // SANITY::ブレーキ
  RotateStop();
}

void TriggerLigthOn(){
  isLightON=1;
  RotateStop();
}
void TriggerLigthOff(){
  isLightON=0;
  RotateStop();
}

