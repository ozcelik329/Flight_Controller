#include "Logger.h"
#include "USBManager.h"
#include <Arduino.h>

void Logger::init() {
    USBManager::println("[Logger] Baslatildi.");
}

void Logger::log(const char* message) {
    USBManager::println(message);
}

void Logger::logError(const char* error) {
    USBManager::println("[HATA] ");
    USBManager::println(error);
}
