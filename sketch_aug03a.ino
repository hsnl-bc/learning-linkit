#include "Arduino.h"
#include <Wire.h>
#include <SeeedOLED.h>
#include <LRTC.h>
#include "DHT.h"
#define DHTPIN 2  // D2
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);
const int usrBtnPin = 6;

float getHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Humidity: Nan");
    return 0;
  }
  Serial.println("Humidity: " + String(h));
  return (round(h*100.0)/100.0);
}

int getTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Temperature: Nan");
    return 0;
  }
  Serial.println("Temperature: " + String(t));
  return int(t);
}

void handleDhtOutput(){
  char fmStr[64];
  SeeedOled.clearDisplay();
  LRTC.get();
  SeeedOled.putString("Date: ");
  SeeedOled.setTextXY(1, 0);
  sprintf(fmStr, "%ld/%ld/%ld",
          LRTC.year(), LRTC.month(), LRTC.day());
  SeeedOled.putString(fmStr);
  SeeedOled.setTextXY(2, 0);
  sprintf(fmStr, "%.2ld:%.2ld:%.2ld",
          LRTC.hour(), LRTC.minute(), LRTC.second());
  SeeedOled.putString(fmStr);
  SeeedOled.setTextXY(4, 0);
  SeeedOled.putString("Humidity: ");
  SeeedOled.putFloat(getHumidity());
  SeeedOled.setTextXY(6, 0);
  SeeedOled.putString("Temperature: ");
  SeeedOled.putNumber(getTemperature());
  return;
}
void setup() {
  Wire.begin();
  SeeedOled.init();
  Serial.begin(9600);
  Serial.println("Initialize...");
  // RTC
  LRTC.begin();
  LRTC.set(2077, 1, 1, 0, 0, 0);
  // Oled init
  SeeedOled.clearDisplay();
  SeeedOled.setNormalDisplay();       //Set display to Normal mode
  SeeedOled.setPageMode();            //Set addressing mode to Page Mode
  SeeedOled.putString("Hi ^_^ /");
  SeeedOled.setTextXY(2, 0);
  SeeedOled.putString("Welcome to the /");
  SeeedOled.setTextXY(4, 0);
  SeeedOled.putString("1st day in /");
  SeeedOled.setTextXY(6, 0);
  SeeedOled.putString("2077!");
  delay(10000);
  // Start dht
  dht.begin();
}

void loop() {
  Serial.println("Refresh...");
  handleDhtOutput()
  delay(5000);
}
