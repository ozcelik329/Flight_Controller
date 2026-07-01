#ifndef USBMANAGER_H
#define USBMANAGER_H

#include <Arduino.h>

class USBManager {
  public:
    static void init(uint32_t baud = 115200, uint32_t timeoutMs = 3000);
    static void update();
    static bool isConnected();
    static void write(const uint8_t* data, size_t len);
    static void write(const char* text);
    static void println(const char* text);
    static void printf(const char* fmt, ...);
    static void read(uint8_t* data, size_t len, size_t& bytesRead);

  private:
    static bool _initialized;
    static bool _connected;
    static uint32_t _baudRate;
};

#endif
