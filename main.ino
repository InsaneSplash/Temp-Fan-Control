#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

float tempPin = A1; // the output pin of LM35
int RELAY1 = 12;  // the output pin of relay
int led1 = 13;    // led pin
int led2 = 11;    // led pin
float temp;
int tempDisp;
int tempMin = 29; // the temperature to start the fan
int fanRunning = 0;
String fanLCD;

byte customChar[8] = {
  0b00000,
  0b10001,
  0b11011,
  0b01110,
  0b01110,
  0b11011,
  0b10001,
  0b00000
};

void setup() {
  analogReference(INTERNAL);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(tempPin, INPUT);
  pinMode(RELAY1, OUTPUT);
  lcd.createChar(0, customChar);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Temp Fan Control");
  lcd.setCursor(0, 1);
  lcd.print("Version 1.3");
  delay(2000);
  lcd.clear();
}

void loop() {

  temp = readTemp();  // get the temperature
  tempDisp = temp;

  if (temp < tempMin) { // if temp is lower than minimum temp
    if (fanRunning == 1) {
      tempMin = tempMin + 2;
      fanRunning = 0;
    }
    fanLCD = "Standby";
    digitalWrite(RELAY1, LOW);
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
  } else { // if temperature is higher than minimum temp
    fanLCD = "Running";
    if (fanRunning == 0) {
      tempMin = tempMin - 2;
      fanRunning = 1;
    }
    digitalWrite(RELAY1, HIGH);
    digitalWrite(led1, HIGH);  // turn on led
    digitalWrite(led2, LOW);  // turn on led
  }

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempDisp);      // display the temperature
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(11, 0);
  if (fanRunning == 1) {
    lcd.write((uint8_t)0);
    lcd.print(tempMin);
    lcd.print((char)223);
    lcd.print("C ");
  }
  else {
    lcd.print("     ");
  }
  lcd.setCursor(0, 1);  // move cursor to next line
  lcd.print("Fan : ");
  lcd.print(fanLCD);    // display the fan status
}

float readTemp() {  // get the temperature and convert it to celsius
  temp = analogRead(tempPin);
  return temp / 9.31;
}
