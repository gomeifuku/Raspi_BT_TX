#include "RN4020.h"
#include "key_settings.h"

volatile int isLightON = 0;

void setup() {
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_FOWARD_STOP, INPUT);
  pinMode(PIN_REVERSE_STOP, INPUT);
  pinMode(PIN_LIGHT_ON_LED, OUTPUT);
  pinMode(PIN_LIGHT_OFF_LED, OUTPUT);
  pinMode(PIN_DBG_LED1, OUTPUT);
  Initialize();
  attachInterrupt(0, TriggerLigthOn, FALLING);
  attachInterrupt(1, TriggerLigthOff, FALLING);
  isLightON=getLightState();
  // 回転速度調整
  analogWrite(PIN_VREF, 240);
  /*Light OFFの状態までモータを回す*/
}

void loop() {
  delay(10);
  if(isLightON==1){
    digitalWrite(PIN_LIGHT_ON_LED,HIGH);
    digitalWrite(PIN_LIGHT_OFF_LED,LOW);
  }else{
    digitalWrite(PIN_LIGHT_ON_LED,LOW);
    digitalWrite(PIN_LIGHT_OFF_LED,HIGH);
  }
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
/*ブレーキ*/
void RotateStop(){
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
}

void Trigger_ON() {
  // 回転
  RotateFoward();
//  delay(5000);
  // SANITY::ブレーキ
//  RotateStop();
}

void Trigger_OFF() {
  // 逆回転
  RotateReverse();
//  delay(5000);
  // SANITY::ブレーキ
//  RotateStop();
}

void TriggerLigthOn(){
  if(isLightON==0){
    isLightON=1;
    RotateStop();
  }
}
void TriggerLigthOff(){
  if(isLightON==1){
    isLightON=0;
    RotateStop();
  }
}

int getLightState(){
  if(digitalRead(PIN_FOWARD_STOP)==HIGH&&digitalRead(PIN_REVERSE_STOP)==LOW){
    return 1;
  }
  if(digitalRead(PIN_FOWARD_STOP)==LOW&&digitalRead(PIN_REVERSE_STOP)==HIGH){
    return 0;
  }
  if(digitalRead(PIN_FOWARD_STOP)==LOW&&digitalRead(PIN_REVERSE_STOP)==LOW){
    return 1;
  }
  /*TODO:Exception*/
  return 1; 
}
