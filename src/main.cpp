#include <Arduino.h>
#include <functional>
#include <ArduinoLog.h>

#include "Servo.h"

#if !( defined(ESP32) ) && !( defined(ESP8266) )
  #error This code is intended to run on ESP8266 platform! Please check your Tools->Board setting.
#endif

#include <vector>

#include "wifi/WifiManager.h"

CWifiManager *wifiManager;

unsigned long tsSmoothBoot;
bool smoothBoot;

unsigned long tsS;
Servo s;

void setup() {
  delay( 1000 ); // power-up safety delay

  Serial.begin(115200);
  while(!Serial && !Serial.available()){}
  randomSeed(analogRead(0));

  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  Log.noticeln("******************************************");  

#ifdef LED_PIN_BOARD
  pinMode(LED_PIN_BOARD, OUTPUT);
  digitalWrite(LED_PIN_BOARD, HIGH);
#endif

  if (EEPROM_initAndCheckFactoryReset() >= 3) {
    Log.warningln("Factory reset conditions met!");
    EEPROM_wipe();    
  }

  tsSmoothBoot = millis();
  smoothBoot = false;

  EEPROM_loadConfig();

  wifiManager = new CWifiManager();

  s.attach(D7);
  tsS = millis();

  Log.noticeln("Setup completed!");
}

void loop() {
  static unsigned long tsMillis = millis();

  if (!smoothBoot && millis() - tsSmoothBoot > FACTORY_RESET_CLEAR_TIMER_MS) {
    smoothBoot = true;
    EEPROM_clearFactoryReset();
    Log.noticeln("Device booted smoothly!");
  }
  
  wifiManager->loop();

  if (wifiManager->isRebootNeeded()) {
    return;
  }

  //
  // Code goes here
  //
  if (millis() - tsS > 2000) {
    tsS = millis();
    s.write(s.read() == 0 ? 180 : 0);
  }

  delay(50);


}
