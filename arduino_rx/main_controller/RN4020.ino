#include "key_settings.h"
#include <SoftwareSerial.h>

#define PC_WRITE  0x08
#define PC_READ   0x02
#define PC_NOTIFY 0x10


void Initialize() {
  Serial.begin(115200);
  /**************************************************************
    子機：ペリフェラル
  **************************************************************/
  //出荷時設定
  Serial.println("SF,1");
  //ボーレート設定
  Serial.println("SB,1");
  //Private Serviceリセット
  Serial.println("PZ");
  //Reboot
  Serial.println("R,1");
  //ソフトウェアシリアル変更
  Serial.end();
  Serial.begin(9600);
  //エコー
  Serial.println("+");
  //Device Name
  Serial.println("SDM,Arduino_nano");
  /**************************************************************
    SS:サービス設定
    SetRegister
    0x00000001 : User
  **************************************************************/
  Serial.println("SS,00000001");
  /*カスタムPriveteService立ち上げ*/
  Register_Private_Service(FINSNAP_SERVICE_UUID);
  /*カスタムPriveteCharacteristic設定*/
  Register_Private_Characteristic(FINSNAP_CHARACTERISTIC_TORIGGER_EVENT_UUID, PC_WRITE | PC_READ, 1);
  /**************************************************************
    SR:機能設定
    SetRegister
    0x20000000 : パワーサイクル、再起動、切断後にアドバタイズ開始
    0x10000000 : シリアル非同期転送サービス
    0x04000000 : No Direct Advertisement
    0x02000000 : UART Flow
  **************************************************************/
  Serial.println("SR,36000000");

  //Reboot
  Serial.println("R,1");
  delay(500);

  if (Serial.available()) {
    PORTB |= _BV(5);
  }

  Check_CMD();
  PORTB &= ~_BV(5);
}

//RN4020 コマンド受付チェック
void Check_CMD() {
  int i  = 0;
  String buff;
  while (1) {
    if (Serial.available()) {
      buff = Serial.readString();
    }
    if ((buff.indexOf("CMD") >= 63) || (buff.indexOf("CMD") == 0)) {
      i = 0;
      break;
    }
    if (i >= 1000) {
      Serial.println("R,1");
      i = 0;
    }
    i++;
    delay(1);
  }
}

//PS登録
void Register_Private_Service(String uuid) {
  String cmd = "PS,";
  cmd.concat(uuid);
  Serial.println(cmd);
}

//PC登録
void Register_Private_Characteristic(String uuid, int property, int bytes) {
  char uuid_char[128];
  char cmd[255] = "";
  uuid.toCharArray(uuid_char, uuid.length() + 1);
  sprintf(cmd, "PC,%s,%02x,%02x", uuid_char, property, bytes);
  Serial.println(cmd);
}

int getBLEAccess(){
    String RD;
  if (Serial.available()) {
    PORTB |= _BV(5);
    RD = Serial.readString();
    /*BT CMD Check*/
    if (RD.indexOf("WV") != -1) {
      String BTwdata = RD.substring(8, 10);
      if (BTwdata.toInt() == 1) {
        return 1;
      } else {
        return 0;
      }
    }
    return -1;
    PORTB &= ~_BV(5);
  }
}

