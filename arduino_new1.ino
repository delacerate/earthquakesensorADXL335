// HIlangkan COMMENT untuk menggunakan modul dfplayer
// dibuat oleh Iska Darman pada tanggal 24 juni 2020
// UNIVERSITAS TRUNOJOYO MADURA
// Prodi MEKATRONIKA 
//bisa menggunakan library dari sini https://github.com/codedecaybr/ADXL335
#include <EEPROM.h> 
#include <EEPROMAnything.h> // ada di folder lib
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Library LCD I2C
//#include <SoftwareSerial.h>
//#include <DFPlayer_Mini_Mp3.h>  
 
const int buzzerPin = 8;
const int relayPin = 12;
int Xacc, Yacc, Zacc, threshold = 0, thresholdSET = 25;
long debouncing_time = 500; 
volatile unsigned long last_micros;
LiquidCrystal_I2C lcd(0x27, 16, 2);
//SoftwareSerial mySerial(10, 11); 
//int suara=25;
struct sensorValue
{
  int X;
  int Y;
  int Z;
};
 
sensorValue acceleration;
 
void debounceInterrupt_Increment()
{
  if ((long)(micros() - last_micros) >= debouncing_time * 1000) {
    IncrementThreshold();
    last_micros = micros();
  }
}
 
void debounceInterrupt_Decrement()
{
  if ((long)(micros() - last_micros) >= debouncing_time * 1000) {
    DecrementThreshold();
    last_micros = micros();
  }
}
void IncrementThreshold() {
  thresholdSET = EEPROM.read(500);
  thresholdSET++;
  EEPROM.write(500, thresholdSET);
}
 
void DecrementThreshold() {
  thresholdSET = EEPROM.read(500);
  thresholdSET--;
  EEPROM.write(500, thresholdSET);
}
 
void setup() {
  lcd.init();
  lcd.backlight();
  pinMode (buzzerPin, OUTPUT);
  attachInterrupt(0, debounceInterrupt_Increment, RISING);
  attachInterrupt(1, debounceInterrupt_Decrement, RISING);
  //pinMode(suara,OUTPUT);
  //digitalWrite(suara,HIGH);
  //mp3_set_serial (mySerial);
  Serial.begin (9600); 
  //mySerial.begin (9600); 
  //mp3_set_serial (Serial);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  EEPROM.write(500, thresholdSET);
  digitalWrite(buzzerPin, LOW);
  lcd.setCursor(0, 0);
  lcd.print("Mencari getaran....");
  delay(5000);
  //mp3_reset();  
  //delay(10);
  //mp3_set_volume (25);
  //delay(5);
 
  sensorValue acceleration = { analogRead(A1) , analogRead(A2) , analogRead(A3) };
  EEPROM_writeAnything(0, acceleration);
  EEPROM_readAnything(0, acceleration);
  lcd.clear();
}
 
void loop() {
  EEPROM_readAnything(0, acceleration);
  threshold = EEPROM.read(500);
  lcd.setCursor(0, 0);
  lcd.print("Memonitor Batas");
  lcd.setCursor(0,1);
  lcd.print("Getaran = ");
  lcd.print(threshold);
  Xacc = analogRead(A1);
  Yacc = analogRead(A2);
  Zacc = analogRead(A3);
 
  if ((Xacc >= (acceleration.X + threshold)) || (Xacc <= (acceleration.X - threshold))||(Yacc >= (acceleration.Y + threshold)) || (Yacc <= (acceleration.Y - threshold))||(Zacc >= (acceleration.Z + threshold)) || (Zacc <= (acceleration.Z - threshold))) {
    //mp3_play (1);
    delay(1000);
    digitalWrite(relayPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    //digitalWrite(suara,HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ALARM !!!!!");
    lcd.setCursor(0,1);
    lcd.print(" CEPAT EVAKUASI!!");
    delay(9000);
    digitalWrite(relayPin, LOW);
    digitalWrite(buzzerPin, LOW);
    //digitalWrite(suara, LOW);
    lcd.clear();
  }
}
