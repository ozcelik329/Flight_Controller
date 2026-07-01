#include "HealthManager.h"

HealthSnapshot HealthManager::_snapshot = {};

void HealthManager::init() {
    _snapshot = {};
    _snapshot.usbOk = true;
}

void HealthManager::update(uint32_t loopRateHz, bool imuOk, bool rcOk, bool usbOk, bool batteryOk, bool gpsOk, bool failsafe, bool armed, uint32_t heapBytes) {
    _snapshot.loopRateHz = loopRateHz;
    _snapshot.heapBytes = heapBytes;
    _snapshot.stackHighWater = 0;
    _snapshot.imuOk = imuOk;
    _snapshot.rcOk = rcOk;
    _snapshot.usbOk = usbOk;
    _snapshot.batteryOk = batteryOk;
    _snapshot.gpsOk = gpsOk;
    _snapshot.failsafe = failsafe;
    _snapshot.armed = armed;
}

HealthSnapshot HealthManager::getSnapshot() {
    return _snapshot;
}
