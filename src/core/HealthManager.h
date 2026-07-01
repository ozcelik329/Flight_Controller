#ifndef HEALTHMANAGER_H
#define HEALTHMANAGER_H

#include <Arduino.h>

struct HealthSnapshot {
    uint32_t loopRateHz;
    uint32_t heapBytes;
    uint32_t stackHighWater;
    bool imuOk;
    bool rcOk;
    bool usbOk;
    bool batteryOk;
    bool gpsOk;
    bool failsafe;
    bool armed;
};

class HealthManager {
  public:
    static void init();
    static void update(uint32_t loopRateHz, bool imuOk, bool rcOk, bool usbOk, bool batteryOk, bool gpsOk, bool failsafe, bool armed, uint32_t heapBytes);
    static HealthSnapshot getSnapshot();

  private:
    static HealthSnapshot _snapshot;
};

#endif
