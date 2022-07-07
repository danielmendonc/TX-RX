#include <SPI.h>
#include <Wire.h>
#include "RF24.h"
#include "printf.h"
RF24 myRadio(9, 10);
#define sensor A7

const int PWM1 = 6;
const int DIR1 = 7;
const int PWM2 = 5;
const int DIR2 = 4;

int sensorValue = 0;
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

int value;
byte addresses[][6] = { "0" };

const uint8_t num_channels = 126;
uint8_t values[num_channels];

struct Data_Package {
  byte x1Value;
  byte y1Value;
  byte x2Value;
  byte y2Value;
  byte sValue;
};
Data_Package data;

void ReadData() {
  myRadio.openReadingPipe(1, 0xF0F0F0F066);  //Which pipe to read, 40 bit Address
  myRadio.startListening();                  //Stop Transminting and start Reveicing
  if (myRadio.available()) {
    while (myRadio.available()) {
      myRadio.read(&data, sizeof(data));
    }
    lastReceiveTime = millis();
    Serial.print("ReadData");
    Serial.print(".........");
    Serial.print("x1: ");
    Serial.print(data.x1Value);
    Serial.print(" y1: ");
    Serial.print(data.y1Value);
    Serial.print(" x2: ");
    Serial.print(data.x2Value);
    Serial.print(" y2: ");
    Serial.println(data.y2Value);
    myRadio.stopListening();
  }
  currentTime = millis();
  if (currentTime - lastReceiveTime > 1000) {
    resetData();
  }

  if (data.y1Value > 200) {
    analogWrite(PWM1, 100);
    analogWrite(PWM2, 100);
    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, HIGH);


  } else if (data.y1Value < 100) {
    analogWrite(PWM1, 100);
    analogWrite(PWM2, 100);
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, LOW);

  } else if (data.x1Value > 200) {
    analogWrite(PWM1, 100);
    analogWrite(PWM2, 100);
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, HIGH);


  } else if (data.x1Value < 100) {
    analogWrite(PWM1, 100);
    analogWrite(PWM2, 100);
    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, LOW);

  } else if (data.y2Value > 200) {
    analogWrite(PWM1, 255);
    analogWrite(PWM2, 255);
    digitalWrite(DIR1, HIGH);
    digitalWrite(DIR2, HIGH);

  } else if (data.y2Value < 100) {
    analogWrite(PWM1, 255);
    analogWrite(PWM2, 255);
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, LOW);

  } else if (data.x2Value > 200) {
    analogWrite(PWM1, 255);
    analogWrite(PWM2, 255);
    digitalWrite(DIR1, LOW);
    digitalWrite(DIR2, HIGH);

  } else if (data.x2Value < 100) {
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

  sensorValue = analogRead(sensor);
  data.sValue = sensorValue;
  // Serial.print("sensor");
  // Serial.println(sensorValue);
  myRadio.write(&data, sizeof(Data_Package));

  delay(5);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(sensor, INPUT);

  printf_begin();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println(F("\n\RF_NANO v3.0 Test"));
  Serial.println();
  Serial.println();


  //
  // Setup and configure rf radio
  //

  myRadio.begin();
  myRadio.setAutoAck(false);

  // Get into standby mode
  myRadio.startListening();
  myRadio.stopListening();
  myRadio.printDetails();

  myRadio.begin();
  myRadio.setChannel(115);          //115 band above WIFI signals
  myRadio.setPALevel(RF24_PA_MAX);  //MIN power low rage
  myRadio.setDataRate(RF24_1MBPS);  //Minimum speed
                                    //  Serial.print("Setup Initialized");
  delay(500);
}

void loop() {
  ReadData();
}

void resetData() {
  data.x1Value = 127;
  data.y1Value = 127;
  data.x2Value = 127;
  data.y2Value = 127;
  data.sValue = 0;
}