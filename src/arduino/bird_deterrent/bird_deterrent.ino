#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const int pirPin = 7;
const int echoPin = 9;
const int trigPin = 8;

// Use pins 2 and 3 to communicate with DFPlayer Mini
static const uint8_t PIN_MP3_TX = 2; // Connects to module's RX
static const uint8_t PIN_MP3_RX = 3; // Connects to module's TX
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;

unsigned long startTime;

void setup() {
  Serial.begin(9600);
  softwareSerial.begin(9600);
  pinMode(13, OUTPUT);

  pinMode(pirPin, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(6, INPUT_PULLUP);

  player.begin(softwareSerial);
  player.volume(30);
  // Play the first MP3 file on the SD card
  player.play(1);
  delay(1);
  player.stop();
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

int lastValue = LOW;

String serialData;
bool startPlay = false;
bool isPlaying = false;

void loop() {
  unsigned long currTime = millis();
  long distance = measure_dist();
  int pirState = digitalRead(pirPin);

  String newSerialData = read_string();
  if (newSerialData != "") serialData = newSerialData;

  boolean condition;
  if(digitalRead(6) == LOW){
    condition = (distance <= 100 && pirState == HIGH) || serialData == "detected";
  }else{
    condition = serialData == "detected";
  }

  if (condition) {
    if (!isPlaying) {
      startTime = currTime;
      startPlay = true;
      player.randomAll();
      digitalWrite(13, HIGH);
    }
  } else {
    if (!isPlaying) {
      startPlay = false;
    }
  }

  if (startPlay) {
    if (currTime - startTime > 10000) {
      player.stop();
      isPlaying = false;
      digitalWrite(13, LOW);
    }else{
      isPlaying = true;
    }
  }


  delay(1);
}
