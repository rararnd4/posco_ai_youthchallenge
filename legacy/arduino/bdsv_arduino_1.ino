int pir = 7;
int echo = 9;
int trig = 8;
int water = A0;
int speaker = 6;
int value = 5000;
int btn = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(pir,INPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(speaker, OUTPUT);
  pinMode(btn,INPUT);
  lcd.init();
  lcd.backlight();
}

void print_search_bird() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Searching Bird..");
}
void print_find_bird(int dist) {
  tone(speaker,value);
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

  int pirState = digitalRead(pir);

  int waterSensor = analogRead(water);

  int btnState = digitalRead(btn);

  Serial.println("ACK")

  String data=Serial.readString();

  if(btnState==HIGH){
    print_find_bird(dist);
  }
  if((dist<=1000 && pirState == HIGH)||data=="detected"){
    tone(speaker,value);
  }
}
