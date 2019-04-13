#define PIN_IN1  10
#define PIN_IN2  9
#define PIN_VREF 11

#define PIN_MY_SERIAL_RX 6
#define PIN_MY_SERIAL_TX 7

#define PIN_TEST_LED 13
#include <SoftwareSerial.h>
#define FINSNAP_SERVICE_UUID                       "93765f06422711e9b210d663bd873d93"
#define FINSNAP_CHARACTERISTIC_TORIGGER_EVENT_UUID "93766366422711e9b210d663bd873d93"

#define PC_WRITE  0x08
#define PC_READ   0x02
#define PC_NOTIFY 0x10

SoftwareSerial mySerial(PIN_MY_SERIAL_RX, PIN_MY_SERIAL_TX); // RX, TX
String buff;
long oneSec = 0;
int isTriggerON = 0;

void setup() {
  Serial.begin(115200); // ハードウェアシリアルを準備
  mySerial.begin(115200);
  Initialize();
  Serial.println("/******START BLE******/");
  Serial.println("Ready");
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  // 回転速度調整
  analogWrite(PIN_VREF, 127);
}
void loop() {
  delay(10);
  LED_CNT();
  pollingBTcmd();
//  WriteSerialPathThrough();
}

//RN4020 書き込みコマンド受信ポーリング
void pollingBTcmd() {
  String RD;
  if (mySerial.available()) {
    PORTB |= _BV(5);
    RD = mySerial.readString();
    /*BT CMD Check*/
    if (RD.indexOf("WV") != -1) {
      String BTwdata = RD.substring(8, 10);
      if (BTwdata.toInt() == 1) {
        Trigger_ON();
      } else {
        Trigger_OFF();
      }
      Serial.print("!GET FOUND!");
    }
    //    Serial.print(RD);
    PORTB &= ~_BV(5);
  }
}
//PC->RN4020 コマンドWrite
void WriteSerialPathThrough() {
  String TD;
  if (Serial.available()) {
    PORTB |= _BV(5);
    TD = Serial.readString();
    Serial.print(TD);
    mySerial.print(TD);
    PORTB &= ~_BV(5);
  }
}

//初期化
void Initialize() {
  pinMode(PIN_TEST_LED, OUTPUT);
  PORTB &= ~_BV(5);
  Serial.println("/*****Arduino UNO*****/");
  Serial.println("/      Peripheral     /");
  Serial.println("/   BLE Setting now   /");
  /**************************************************************
    子機：ペリフェラル
  **************************************************************/
  //出荷時設定
  mySerial.println("SF,1");
  //ボーレート設定
  mySerial.println("SB,1");
  //Private Serviceリセット
  mySerial.println("PZ");
  //Reboot
  mySerial.println("R,1");
  //ソフトウェアシリアル変更
  mySerial.end();
  mySerial.begin(9600);
  //エコー
  mySerial.println("+");
  //Device Name
  mySerial.println("SDM,Arduino_nano");
  /**************************************************************
    SS:サービス設定
    SetRegister
    0x00000001 : User
  **************************************************************/
  mySerial.println("SS,00000001");
  Register_Private_Service(FINSNAP_SERVICE_UUID);
  Register_Private_Characteristic(FINSNAP_CHARACTERISTIC_TORIGGER_EVENT_UUID, PC_WRITE | PC_READ, 1);
  /**************************************************************
    SR:機能設定
    SetRegister
    0x20000000 : パワーサイクル、再起動、切断後にアドバタイズ開始
    0x10000000 : シリアル非同期転送サービス
    0x04000000 : No Direct Advertisement
    0x02000000 : UART Flow
  **************************************************************/
  mySerial.println("SR,36000000");

  //Reboot
  mySerial.println("R,1");
  delay(500);

  if (mySerial.available()) {
    PORTB |= _BV(5);
  }

  Check_CMD();
  PORTB &= ~_BV(5);
}

//RN4020 コマンド受付チェック
void Check_CMD() {
  int i  = 0;
  while (1) {
    if (mySerial.available()) {
      buff = mySerial.readString();
    }
    if ((buff.indexOf("CMD") >= 63) || (buff.indexOf("CMD") == 0)) {
      i = 0;
      break;
    }
    if (i >= 1000) {
      Serial.println("Reset");
      mySerial.println("R,1");
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
  mySerial.println(cmd);
}

//PC登録
void Register_Private_Characteristic(String uuid, int property, int bytes) {
  char uuid_char[128];
  char cmd[255] = "";
  uuid.toCharArray(uuid_char, uuid.length() + 1);
  sprintf(cmd, "PC,%s,%02x,%02x", uuid_char, property, bytes);
  Serial.println(cmd);
  mySerial.println(cmd);
}


//LED
void LED_CNT() {
  if (millis() >= oneSec) {

    PORTB = PORTB ^ _BV(5);
    oneSec = millis() + 1000;
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

