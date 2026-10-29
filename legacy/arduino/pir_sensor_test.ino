#define pir 2
//앞으로 D7핀을 ‘pir’이라 부르겠습니다. 선언합니다.

  void setup() { //여기 안에 있는 코드는 딱 한번만 실행됩니다.
  pinMode(pir,INPUT);
  //아두이노의 D7핀을 INPUT 모드로 설정합니다. 이제 이 핀으로 값을 읽어들일 수 있습니다.
  pinMode(LED_BUILTIN, OUTPUT);
  //아두이노의 내장된 LED를 OUTPUT모드로 설정합니다. 이제 켜고 끌 수 있습니다.
}

  void loop() { //위의 Setup() 실행 이후, 여기 안에 있는 코드는 무한히 반복실행됩니다. 
  
  int data = digitalRead(pir); 
  //여기 loop안에서만, ‘pir’에서 읽히는 정보를 ‘data’라 부르겠습니다.
  if(data == 1){ //만약 읽은 data값이 1이라면,
  digitalWrite(LED_BUILTIN, HIGH); //내장된 LED를 켭니다.
  delay(1000); // 1초동안 켭니다.
  }
  else{ //그게 아니라면,
  digitalWrite(LED_BUILTIN, LOW); //내장된 LED를 끕니다. 
  }
}
