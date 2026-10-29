#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//lcd의 주소와 lcd의 칸 넓이 정하기
LiquidCrystal_I2C lcd(0x27, 16, 2);
//센서들의 핀 번호 지정
const int pirPin = 7;
const int echoPin = 9;
const int trigPin = 8;
const int waterSensorPin = A0;
const int speakerPin = 6;
const int btnPin = 10;
//씨리얼 통신 9600으로 정하고 핀 모듈들이 입력 또는 출력으로 동작할 수 있게 지정된 핀 구성
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);

  pinMode(pirPin, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(btnPin, INPUT_PULLUP); // INPUT_PULLUP인 이유는 저항없이 버튼을 작동시키기 위해
//lcd의 backlight를 작동
  lcd.init();
  lcd.backlight();
}
//거리측정
long measure_dist() {
  long duration, dist;
  digitalWrite(trigPin, LOW); //초기화
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH); //trigger핀 신호 발생 (10us)
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);//다시 초기화
  //초음파 센서 거리 계산 (cm)
  duration = pulseIn(echoPin, HIGH);
  dist = duration * 17 / 1000;
  return dist;
}
//사용가능 여부확인
String read_string() { // https://forum.arduino.cc/t/read-line-from-serial/98251/5
  String result = "";
  while (Serial.available()) {
    delay(1);  //delay to allow buffer to fill
    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one byte from serial buffer
      result += c; //makes the string readString
    }
  }
  return result; //result의 값 반환
}
//lcd를 초기화 하고 lcd에 나올 문구의 좌표를 지정하고 새를 찾고 있다고 출력
void print_search_bird() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Searching Bird..");
}
//lcd를 초기화 하고 lcd에 나올 문구의 좌표를 지정하고 새를 찾았다고 출력 및 거리의 값 출력
void print_find_bird(int dist) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Find Bird!");
  lcd.setCursor(0, 1);
  lcd.print("Distance:" + String(dist) + "cm");
}

String serialData;//serialData의 값을 문자열로 변환

void loop() {//센서들의 값을 숫자로 변환
  long distance = measure_dist();
  int pirState = digitalRead(pirPin);
  int waterValue = analogRead(waterSensorPin);
  int btnState = digitalRead(btnPin);

  String newSerialData = read_string();//newserialData에 serial의 값을 가져오고 문자열로 반환  
  if(newSerialData != "") serialData = newSerialData;

  boolean condition = (distance <= 100 && pirState == HIGH) || serialData == "detected";//condition을 거리가 1m보다 작거나 같을 때 seriaData를 "detected"로 지정

  if (btnState == LOW) { // INPUT_PULLUP모드이기 때문에 버튼 값이 반대로 측정됨
    if (condition) {
      print_find_bird(distance);
    }
    else {//아니면 새를 찾고 있다고 출력
      print_search_bird();
    }
  }
  else {//아니면 lcd의 값을 초기화 하고 스피커의 톤을 3000~5000사이의 랜덤으로 출력
    lcd.clear();
    if (condition) {
      tone(speakerPin, random(3000, 5001));
      digitalWrite(13, HIGH);
    } else {//아니면 스피커의 톤이 노톤이 나올 때 까지 출력 및 초기화
      noTone(speakerPin);
      digitalWrite(13, LOW);
    }
  }

  delay(1);//1/1000초 동안 기다리기
}
