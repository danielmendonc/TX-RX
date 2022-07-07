#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Wire.h>
#include "RF24.h"
#include "printf.h"
RF24 myRadio(9, 10);

int value;
byte addresses[][6] = { "0" };

const uint8_t num_channels = 126;
uint8_t values[num_channels];

LiquidCrystal_I2C lcd(0x27, 16, 2);

struct Data_Package {
  byte x1Value;
  byte y1Value;
  byte x2Value;
  byte y2Value;
  byte sValue;
};
Data_Package data;

void WriteData() {
  delay(5);
  data.y1Value = map(analogRead(A0), 0, 1023, 0, 255);
  data.x1Value = map(analogRead(A1), 0, 1023, 0, 255);
  data.y2Value = map(analogRead(A2), 0, 1023, 0, 255);
  data.x2Value = map(analogRead(A3), 0, 1023, 0, 255);
  myRadio.stopListening();                //Stop Receiving and start transminitng
  myRadio.openWritingPipe(0xF0F0F0F066);  //Sends data on this 40-bit address
  myRadio.write(&data, sizeof(data));
  Serial.print("WriteData");
  Serial.print(".........");
  Serial.print("x1: ");
  Serial.print(data.x1Value);
  Serial.print(" y1: ");
  Serial.print(data.y1Value);
  Serial.print(" x2: ");
  Serial.print(data.x2Value);
  Serial.print(" y2: ");
  Serial.println(data.y2Value);
  delay(5);
  ReadData();
  delay(5);
}

void ReadData() {
  myRadio.openReadingPipe(1, 0xF0F0F0F066);  //Which pipe to read, 40 bit Address
  myRadio.startListening();                  //Stop Transminting and start Reveicing
  if (myRadio.available()) {
    while (myRadio.available()) {
      myRadio.read(&data, sizeof(data));
    }
    int sensor = data.sValue;
    lcd.setCursor(0, 0);
    lcd.print("Robot Connected");
    Serial.print("ReadData");
    Serial.print(".........");
    Serial.println(value);
    if (sensor < 200) {
      lcd.setCursor(0, 1);
      lcd.print("Metal Searching ");
    } else {
      lcd.setCursor(0, 1);
      lcd.print("  Metal Found   ");
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("  Hello World!  ");
    lcd.setCursor(0, 1);
    lcd.print("Robot Disconnected");
  }
  myRadio.stopListening();
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

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
  WriteData();
}