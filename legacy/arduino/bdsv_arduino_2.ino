#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pirPin = 7;
const int echoPin = 9;
const int trigPin = 8;
const int waterSensorPin = A0;
const int speakerPin = 6;
const int btnPin = 10;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  pinMode(pirPin, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(btnPin, INPUT_PULLUP); // INPUT_PULLUP인 이유는 저항없이 버튼을 작동시키기 위해

  lcd.init();
  lcd.backlight();
}

long measure_dist() {
  long duration, dist;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //초음파 센서 거리 계산 (cm)
  duration = pulseIn(echoPin, HIGH);
  dist = duration * 17 / 1000;
  return dist;
}

String read_string() { // https://forum.arduino.cc/t/read-line-from-serial/98251/5
  String result = "";
  while (Serial.available()) {
    delay(1);  //delay to allow buffer to fill
    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one byte from serial buffer
      result += c; //makes the string readString
    }
  }
  return result;
}

void print_search_bird() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Searching Bird..");
}

void print_find_bird(int dist) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Find Bird!");
  lcd.setCursor(0, 1);
  lcd.print("Distance:" + String(dist) + "cm");
}

int lastValue = LOW;

String serialData;

void loop() {
  long distance = measure_dist();
  int pirState = digitalRead(pirPin);
  int waterValue = analogRead(waterSensorPin);
  int btnState = digitalRead(btnPin);

  String newSerialData = read_string();
  if(newSerialData != "") serialData = newSerialData;

  boolean condition = (distance <= 100 && pirState == HIGH) || serialData == "detected";

  if (btnState == LOW) { // INPUT_PULLUP모드이기 때문에 버튼 값이 반대로 측정됨
    if (condition) {
      print_find_bird(distance);
    }
    else {
      print_search_bird();
    }
  }
  else {
    lcd.clear();
    if (condition) {
      tone(speakerPin, random(3000, 5001));
      digitalWrite(13, HIGH);
    } else {
      noTone(speakerPin);
      digitalWrite(13, LOW);
    }
  }

  delay(1);
}
