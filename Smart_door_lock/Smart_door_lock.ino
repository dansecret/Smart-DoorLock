//Smart Door Lock By AJTekno
#include <LiquidCrystal.h>
LiquidCrystal lcd (7, 6, 5, 4, 3, 2);
#include <SPI.h>
#include <MFRC522.h>         

//inisialisasi pin pada RFID
const int PIN_RST = 9;  
const int PIN_SS  = 10; 

String uidTag = "";     
const int pinrelay = 8;

// Pembuatan objek MFRC522 
MFRC522 mfrc(PIN_SS, PIN_RST); 

void setup() {
  Serial.begin(9600);
  SPI.begin();  
  mfrc.PCD_Init(); 

  pinMode (pinrelay, OUTPUT);

  // Cek versi software MFRC555
  byte versi = mfrc.PCD_ReadRegister(mfrc.VersionReg);
  if (versi == 0x00 || versi == 0xFF) {
    lcd.begin(16, 2);
    lcd.setCursor (0,0);
    lcd.print("Modul tidak ada!");
    while (true);   
  }
}

void loop() {
  lcd.setCursor (0,0); 
  lcd.print(" Tempelkan Tag! ");
  digitalWrite (pinrelay, HIGH);
  // Cek untuk kartu yang baru disisipkan 
  if (!mfrc.PICC_IsNewCardPresent())
    return;

  // Jika nomor tag tidak diperoleh
  if (!mfrc.PICC_ReadCardSerial())
    return;

  // Peroleh UID pada tag
  uidTag = "";
  for (byte j = 0; j < mfrc.uid.size; j++) {
    char teks[3];
    sprintf(teks, "%02X", mfrc.uid.uidByte[j]);
    uidTag += teks;
  }
//  Serial.println(" UID : ");
//  Serial.println(uidTag);
  //jika tag RFID sesuai dengan yang terdaftar
  if  (uidTag.substring(0) == "042D5D7A796180"){
  digitalWrite (pinrelay, LOW);
  lcd.setCursor (0,0);
  lcd.print("ID:");
  lcd.print(uidTag);
  delay (1000);
  lcd.setCursor (0,1);
  lcd.print(" Selamat Datang ");
  delay (2000);
  lcd.setCursor (1,0);
  lcd.print ("   Hamdandih   ");
  delay (5000);
  }
   else if  (uidTag.substring(0) == "431B931D"){ 
  digitalWrite (pinrelay, LOW);
//  lcd.clear();
  lcd.setCursor (0,0);
  lcd.print("ID:");
  lcd.print(uidTag);
  delay (1000);
  lcd.setCursor (0,1);
  lcd.print(" Selamat Datang ");
  delay (1000);
  lcd.setCursor (0,1);
  lcd.print ("   Admin       ");
  delay (5000);
  }
  
//ketika tag RFID tidak sesuai dengan data
  else {
  lcd.clear();
  lcd.setCursor (1,1);
  lcd.print("Akses ditolak !!");
  delay (2000);
  }

  // Ubah status kartu ACTIVE ke status HALT
  mfrc.PICC_HaltA();
}
