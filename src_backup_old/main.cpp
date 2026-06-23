#include <Arduino.h>
#include "core/FlightManager.h"
#include "core/SystemTimer.h"
#include "utils/Logger.h"

// Modül nesnelerimiz
FlightManager flightManager;

void setup() {
  Serial.begin(115200);
  Logger::init();
  
  flightManager.init();
  SystemManager::init();
  
  Logger::log("AeroPico FC Baslatildi!");
}

void loop() {
  flightManager.update();
}