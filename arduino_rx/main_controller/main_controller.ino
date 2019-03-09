#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
String buff;
char KEY  = 0;
String RD;
String TD; 
int i     = 0;
long oneSec = 0;
void setup() {
  Serial.begin(115200); // ハードウェアシリアルを準備
  mySerial.begin(115200);
  
  pinMode(13, OUTPUT);
  PORTB &= ~_BV(5);
  Serial.println("/*****Arduino UNO*****/");
  Serial.println("/      Peripheral     /");
  Serial.println("/   BLE Setting now   /");
//  /**************************************************************
//   子機：ペリフェラル
//  **************************************************************/
//  //出荷時設定
//  mySerial.println("SF,1");
//  //ボーレート設定
//  mySerial.println("SB,1");
//  //Reboot
//  mySerial.println("R,1");
//  //ソフトウェアシリアル変更
//  mySerial.end();
//  mySerial.begin(9600);
//  //エコー
//  mySerial.println("+");
//  //Device Name
//  mySerial.println("SDM,Arduino_nano");
//  /**************************************************************
//   SS:サービス設定
//   SetRegister
//   0x00000001 : User
//  **************************************************************/
//  mySerial.println("SS,00000001");
//  /**************************************************************
//   SR:機能設定
//   SetRegister
//   0x20000000 : パワーサイクル、再起動、切断後にアドバタイズ開始
//   0x10000000 : シリアル非同期転送サービス
//   0x04000000 : No Direct Advertisement
//   0x02000000 : UART Flow
//  **************************************************************/
//  mySerial.println("SR,36000000");
//  //Reboot
//  mySerial.println("R,1");
//  delay(500);
//    
//  if(mySerial.available()){
//    PORTB |= _BV(5);
//  }
//    
//  Check_CMD();
//  PORTB &= ~_BV(5);
  Serial.println("/******START BLE******/");
  Serial.println("Ready");
}
void loop() {
  LED_CNT();
  if (mySerial.available()){
    PORTB |= _BV(5);
    RD = mySerial.readString();
    Serial.print(RD); 
    PORTB &= ~_BV(5);
  }
  
  if(Serial.available()){
    PORTB |= _BV(5);
    TD = Serial.readString();
    Serial.print(TD);
    mySerial.print(TD); 
    PORTB &= ~_BV(5);
    
  }
}
//RN4020 エコー確認
void Echo_Print(){
  while(1){
    if(mySerial.available()>0){
      buff = mySerial.readString();
      Serial.print(buff);
      break;
    }
  }
}
//RN4020 コマンド受付チェック
void Check_CMD(){
  while(1){
    if(mySerial.available()){
      buff = mySerial.readString();
    }
    if((buff.indexOf("CMD") >= 63) || (buff.indexOf("CMD") == 0)){
      i = 0;
      break;
    }
    if(i >= 1000){
      Serial.println("Reset");
      mySerial.println("R,1");
      i = 0;
    }
    i++;
    delay(1);
  }
}
//LED
void LED_CNT(){
  if(millis() >= oneSec){
    PORTB = PORTB ^ _BV(5);
    oneSec = millis() + 1000;
  }
}
