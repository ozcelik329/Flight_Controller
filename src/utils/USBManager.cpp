#include "USBManager.h"

bool USBManager::_initialized = false;
bool USBManager::_connected = false;
uint32_t USBManager::_baudRate = 115200;

void USBManager::init(uint32_t baud, uint32_t timeoutMs) {
    _baudRate = baud;
    Serial.begin(_baudRate);

    uint32_t start = millis();
    while (!Serial && (millis() - start < timeoutMs)) {
        delay(10);
    }

    _connected = static_cast<bool>(Serial);
    _initialized = true;
}

void USBManager::update() {
    if (!_initialized) {
        return;
    }

    _connected = static_cast<bool>(Serial);
}

bool USBManager::isConnected() {
    return _connected;
}

void USBManager::write(const uint8_t* data, size_t len) {
    if (_initialized && _connected) {
        Serial.write(data, len);
    }
}

void USBManager::write(const char* text) {
    if (_initialized && _connected) {
        Serial.print(text);
    }
}

void USBManager::println(const char* text) {
    if (_initialized && _connected) {
        Serial.println(text);
    }
}

void USBManager::printf(const char* fmt, ...) {
    if (!_initialized || !_connected) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    Serial.print(buffer);
}

void USBManager::read(uint8_t* data, size_t len, size_t& bytesRead) {
    bytesRead = 0;
    if (!_initialized || !_connected) {
        return;
    }

    while (bytesRead < len && Serial.available()) {
        data[bytesRead++] = static_cast<uint8_t>(Serial.read());
    }
}
