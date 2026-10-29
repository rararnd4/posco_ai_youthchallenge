int pir = 7;
int echo = 9; //하단 초음파 
int trig = 8; //하단 초음파 
int water = A0;
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(pir, INPUT);
  lcd.init();
  lcd.backlight();
}

void print_search_bird() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Searching Bird..");
}
void print_find_bird(int dist) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Find Bird!");
  lcd.setCursor(0,1);
  lcd.print("Distance:"+ String(dist) +"cm");
}

void loop() {

  //초음파 센서 거리 인식
  long duration, dist;
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  //초음파 센서 거리 계산 (cm)
  duration = pulseIn(echo, HIGH);
  dist = duration * 17 / 1000;
  Serial.println("SIG-DIS-" + String(dist));

 
  if (dist <=1000) {
      //PIR 센서 값 읽기 
     int pirState = digitalRead(pir);
     Serial.println("SIG-MIC-" + String(pirState));
      if (pirState == HIGH) {
         print_find_bird(dist);
         delay(50);
      }
  } else print_search_bird();


   //빗물 센서 값 읽어오기
  int waterSensor = analogRead(water);
  //빗물이 감지 된다면
  if (waterSensor > 0) Serial.println("SIG-WAWTER-"+String(waterSensor));
  delay(1000);
  
}
