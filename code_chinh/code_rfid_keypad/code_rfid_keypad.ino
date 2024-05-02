#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

#define SS_PIN 10
#define RST_PIN 9
#define chot A3
#define loa A2

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 rfid(SS_PIN, RST_PIN);

String UID1 = "23 91 AE C3";
String UID2 = "17 E3 BE 5F";

//Tạo ký tự đặc biệt &
 byte va[] = {
  B00000,
  B01100,
  B10010,
  B10100,
  B01000,
  B10101,
  B10010,
  B01101
};

const byte ROWS = 4; 
const byte COLS = 4; 
char password[5];
char On_equip[]="1412"; // Mật khẩu mở cửa
int i = 0;
int on=0;

char MatrixKey[ROWS][COLS] = 
{
  {'A','B','C','D'},
  {'3','6','9','#'},
  {'2','5','8','0'},
  {'1','4','7','*'}
};
byte rowPins[ROWS] = {A1, 8, 7, 6}; // (R1,R2,R3,R4)
byte colPins[COLS] = {5, 4, 3, 2}; // (C1,C2,C3,C4)

Keypad Mykeys = Keypad( makeKeymap(MatrixKey), rowPins, colPins, ROWS, COLS); 


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, va); // Tạo ký tự đặc biệt &
  SPI.begin();
  rfid.PCD_Init();
  pinMode(chot, OUTPUT);
  pinMode(loa, OUTPUT);
  digitalWrite(chot, 0);
  digitalWrite(loa, 0);
}

void loop() {
  RFID();
  keypadss();
}



void RFID ()
{
  lcd.setCursor(4, 0);
  lcd.print("Xin Chao!");
  lcd.setCursor(0, 1);
  lcd.print("RFID / Password");
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dang quet");
  Serial.print("NUID tag is :");
  String ID = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    lcd.print(".");
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
    delay(300);
  }
  ID.toUpperCase();

  if ((ID.substring(1) == UID1 || ID.substring(1) == UID2)) {
    digitalWrite(chot, 1);
    digitalWrite(loa, 1);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cua mo");
    delay(300);
    digitalWrite(loa, 0);
    delay(100);
    digitalWrite(loa, 1);
    delay(300);
    digitalWrite(loa, 0);
    for (int i=1; i<5; i++)
    {
      delay(1000);
    }
    digitalWrite(chot, 0);
    lcd.clear();

  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sai the");
    digitalWrite(loa, 1);
    delay(1500);
    digitalWrite(loa, 0);
    lcd.clear();
  }
}


void keypadss ()
{
  lcd.setCursor(4, 0);
  lcd.print("Xin Chao!");
  lcd.setCursor(0, 1);
  lcd.print("RFID / Password");
   char EnterKey = Mykeys.getKey();
  
 if (EnterKey)
 {
    password[i]=EnterKey; // Nhập lần lượt các ký tự vào biến chuỗi ký tự Psssword
    i++;
    on++;
    Serial.println(password);
    lcd.clear();
    // Nhập mật khẩu
    if (i == 0) {
      password[0] = EnterKey;
      lcd.setCursor(6, 1);
      lcd.print(password[0]);
      delay(500); // Ký tự hiển thị trên màn hình LCD trong 0.5s
      lcd.setCursor(6, 1);
      lcd.print("*"); // Ký tự được thay bởi dấu *
    }
    if (i == 1) {
      password[1] = EnterKey;
      lcd.setCursor(7, 1);
      lcd.print(password[1]);
      delay(500);
      lcd.setCursor(7, 1);
      lcd.print("*");
    }
    if (i == 2) {
      password[2] = EnterKey;
      lcd.setCursor(8, 1);
      lcd.print(password[2]);
      delay(500);
      lcd.setCursor(8, 1);
      lcd.print("*");
    }
    if (i == 3) {
      password[3] = EnterKey;
      lcd.setCursor(9, 1);
      lcd.print(password[3]);
      delay(500);
      lcd.setCursor(9, 1);
      lcd.print("*");
    }
 }
  if(on==4) 
  {
  if(!strcmp(password,On_equip))
    {
      digitalWrite(chot, 1);
      digitalWrite(loa, 1);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cua mo");
      delay(300);
      digitalWrite(loa, 0);
      delay(100);
      digitalWrite(loa, 1);
      delay(300);
      digitalWrite(loa, 0);
      for (int i=1; i<5; i++)
      {
        delay(1000);
      }
      digitalWrite(chot, 0);
      // lcd.print("    Correct!");
      // myservo.write(180); // Mở cửa!
      // digitalWrite(RedLed,LOW);
      // delay(1000);
      lcd.clear();
       i=0;
      Serial.println(" Dung mat khau mo cua!");
    }
                  
    if(strcmp(password,On_equip))
    {
       lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sai password");
      digitalWrite(loa, 1);
      delay(1500);
      digitalWrite(loa, 0);
      lcd.clear();
      i = 0;
      Serial.println(" Sai mat khau nhap lai.............");
    }
    on=0;
  }
}