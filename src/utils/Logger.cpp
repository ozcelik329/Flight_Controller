#include "Logger.h"
#include <Arduino.h>

void Logger::init() {
  Serial.begin(115200);
}

void Logger::log(const char* message) {
  Serial.println(message);
}

void Logger::logError(const char* error) {
  Serial.print("HATA: ");
  Serial.println(error);
}
