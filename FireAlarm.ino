// 연기 A0
// 조도 A2
// 온도 A5
// 불꽃 D8
// 부저 D10
// R = D5 , G = D6, B = D7

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2,3);

int aout = A0;
int cds = A2;
int lm35Pin = 5;
int firePin = 8;
int R = 5; 
int G = 6;
int B = 7;
int pinSpeaker = 10;

void setup() 
{
  pinMode(pinSpeaker, OUTPUT);
  pinMode(R,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop()
{
  char data[26]={0,};
  
  unsigned int t;
  int f = digitalRead(firePin);   
  int a = analogRead(aout);      
  unsigned int reading = analogRead(lm35Pin);            
  t = (500* reading)/1024;
  int l = analogRead(cds);               
  
  if (!f == 1 || a >= 400 || t >= 45 ){ 
      playTone(300, 160);    // 부저
      Serial.println("FIRE!!!!!!!!!!!!!!!!!!!!!");   
  }
  
  sprintf(data,"%4d%4d%d%4d",a,t,!f,l);
  Serial.println(data); 
  BTSerial.print(data);

  char LEDdata;
  if(BTSerial.available()){
    LEDdata = BTSerial.read();
    Serial.println(data);
    delay(1);

    if(LEDdata == 'T'){
      analogWrite(R,255);
      analogWrite(G,255);
      analogWrite(B,255);
      delay(150);
    }

     if(LEDdata == 'F'){
      analogWrite(R,0);
      analogWrite(G,0);
      analogWrite(B,0);
      delay(150);
    }
  }
  delay(3000);
}

void playTone(long duration, int freq) {
    duration *= 1000;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    while (elapsed_time < duration) {
        digitalWrite(pinSpeaker,HIGH);
        delayMicroseconds(period / 2);
        digitalWrite(pinSpeaker, LOW);
        delayMicroseconds(period / 2);
        elapsed_time += (period);
    }
}
