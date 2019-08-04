#include "Arduino.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <SeeedOLED.h>
#include <LRTC.h>
#include <LBLE.h>
#include <LBLEPeriphral.h>
#include "DHT.h"
#define DHTPIN 2  // D2
#define DHTTYPE DHT22   // DHT 22  (AM2302)

// Hardware configuration
DHT dht(DHTPIN, DHTTYPE);
const int usrBtnPin = 6;

// Bluetooth global
LBLEService dhtService("72312f2b-9419-4dc8-bc95-acdfe81e8cb1");
LBLECharacteristicString switchCharacteristic("72312f2c-9419-4dc8-bc95-acdfe81e8cb1", LBLE_READ);

// Initialize accessible data
float HUMIDITY=0;
int TEMPERATURE=0;

void setHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Humidity: Nan");
  }
  Serial.println("Humidity: " + String(h));
  HUMIDITY=(round(h*100.0)/100.0);
}

void setTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Temperature: Nan");
  }
  Serial.println("Temperature: " + String(t));
  TEMPERATURE=int(t);
}

void handleOLEDOutput(){
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
  SeeedOled.putFloat(HUMIDITY);
  SeeedOled.setTextXY(6, 0);
  SeeedOled.putString("Temperature: ");
  SeeedOled.putNumber(TEMPERATURE);
}

void initLBLE(){
  LBLE.begin();
  while (!LBLE.ready()) {
    delay(100);
  }
  Serial.println("BLE ready");
  Serial.print("Device Address = [");
  Serial.print(LBLE.getDeviceAddress());
  Serial.println("]"); 
  LBLEAdvertisementData advertisement;
  advertisement.configAsConnectableDevice("DHT Sensor");
  LBLEPeripheral.setName("DHT Sensor");
  dhtService.addAttribute(switchCharacteristic);
  LBLEPeripheral.addService(dhtService);
  LBLEPeripheral.begin();
  LBLEPeripheral.advertise(advertisement);
}

void handleJsonCharacteristic(){
  StaticJsonDocument<200> doc;
  String result;
  doc["sensor"] = "dht";
  JsonObject obj = doc.createNestedObject("data");
  obj["humidity"] = String(HUMIDITY);
  obj["temperature"] = String(TEMPERATURE);
  serializeJson(doc, result);
  switchCharacteristic.setValue(result);
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Initialize...");
  // RTC
  LRTC.begin();
  LRTC.set(2077, 1, 1, 0, 0, 0);
  // Oled init
  SeeedOled.init();
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
  // LBLE
  initLBLE();
  // Start dht
  dht.begin();
}

void loop() {
  // Handling global varibles
  setHumidity();
  setTemperature();
  // Refresh and print on screen
  handleOLEDOutput();
  // Bluetooth broadcast data
  handleJsonCharacteristic();
  delay(5000);
}
