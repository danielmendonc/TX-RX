//Arduino Metal Detector Robot 
//Receiver Sktech
//Created by DIY Builder
// Contact me https://www.instagram.com/diy.builder/
// Before uploading the sketch you have to install the required libraries
// First go to sketch section >> include library >> manage libaries >> Search RF24 and LiquidCrystal >> Install it >> Done

#include<RF24.h>
#include<nRF24L01.h>
#include<SPI.h>
#define sensor A7
                                                                                      
int PWM1 = 6;
int DIR1 = 7;
int PWM2 = 5;
int DIR2 = 4;

int sensorValue = 0;
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

RF24 radio(10,9);
const byte address[6] = "00001";
const byte address1[6] = "00003";

struct Data_Package {
  byte x1Value;
  byte y1Value;
  byte x2Value;
  byte y2Value;
  byte sValue;
  
  
};
 Data_Package data;
 


void setup() {
  Serial.begin(9600);

  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(sensor, INPUT);

 
 
  radio.begin();
  radio.openReadingPipe(1,address);
  radio.openWritingPipe(address1);
  radio.setPALevel(RF24_PA_HIGH);
  
  
  resetData();
}

void loop() {
   
    radio.startListening();
    
    if(radio.available()) {
  radio.read(&data, sizeof(Data_Package));
  lastReceiveTime = millis();
    }
    currentTime = millis();
    if(currentTime - lastReceiveTime > 1000) {
      resetData();
    }
  Serial.print("j1PotX: ");
  Serial.println(data.x1Value);
  Serial.print("j1PotY: ");
  Serial.println(data.y1Value);
  
  if(data.y1Value > 200 ) {
    analogWrite(PWM1, 100);
    analogWrite(PWM2, 100);
    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, HIGH);
    
    
  }else if(data.y1Value < 100) {
    analogWrite(PWM1, 100);
    analogWrite(PWM2, 100);
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, LOW);
 
  }
  else if(data.x1Value > 200 ) {
    analogWrite(PWM1, 100);
    analogWrite(PWM2, 100);
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, HIGH);
    
    
  }else if(data.x1Value < 100 ) {
    analogWrite(PWM1, 100);
    analogWrite(PWM2, 100);
    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, LOW);
    
  }else if(data.y2Value > 200) {
    analogWrite(PWM1, 255);
    analogWrite(PWM2, 255);
    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, HIGH);
    
  }else if(data.y2Value < 100) {
    analogWrite(PWM1, 255);
    analogWrite(PWM2, 255);
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, LOW);
    
  }else if(data.x2Value > 200) {
    analogWrite(PWM1, 255);
    analogWrite(PWM2, 255);
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, HIGH);
    
  }else if(data.x2Value < 100) {
    analogWrite(PWM1, 255);
    analogWrite(PWM2, 255);
    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, LOW);
  }
    
    else {
 
    analogWrite(PWM1, 0);
    analogWrite(PWM2, 0);
  }
  delay(5);
  
  radio.stopListening();
  sensorValue = analogRead(sensor);
  data.sValue = sensorValue;
  Serial.print("sensor");
  Serial.println(sensorValue);
  radio.write(&data, sizeof(Data_Package));
 
 delay(5);
}
    void resetData() {
      data.x1Value = 127;
      data.y1Value = 127;
      data.x2Value = 127;
      data.y2Value = 127;
      data.sValue = 0;    
      }
    
