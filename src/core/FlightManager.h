#ifndef FLIGHT_MANAGER_H
#define FLIGHT_MANAGER_H

#include <Arduino.h>
#include "pico/mutex.h"
#include "../drivers/Sensors.h"
#include "../drivers/RX.h"
#include "SensorFusion.h"

// Arm/Disarm kombini için eşik değerler
#define ARM_THROTTLE_MAX    1050  // Throttle bu değerin altında olmalı
#define ARM_RUDDER_MIN      1900  // Rudder bu değerin üzerinde olmalı (sağ)
#define DISARM_RUDDER_MAX   1100  // Rudder bu değerin altında olmalı (sol)
#define ARM_HOLD_MS         1000  // 1 saniye basılı tutulmalı

class FlightManager {
  public:
    void init();
    void update();

    float getRoll();
    float getPitch();
    float getYaw();
    float getGyroX();
    float getGyroY();
    float getGyroZ();
    uint16_t getAileron();
    uint16_t getElevator();
    uint16_t getThrottle();
    uint16_t getRudder();

    bool isArmed() const { return _armed; }

  private:
    SensorManager sensors;
    RXManager rx;
    SensorFusion fusion;

    mutex_t _mutex;

    float _roll   = 0.0f;
    float _pitch  = 0.0f;
    float _yaw    = 0.0f;
    float _gyroX  = 0.0f;
    float _gyroY  = 0.0f;
    float _gyroZ  = 0.0f;
    uint16_t _aileron  = 1500;
    uint16_t _elevator = 1500;
    uint16_t _throttle = 1000;
    uint16_t _rudder   = 1500;

    // Arm/Disarm
    bool     _armed         = false;
    uint32_t _armHoldStart  = 0;
    uint32_t _disarmHoldStart = 0;

    void performSensorFusion();
    void updateArmDisarm(uint16_t throttle, uint16_t rudder);
};

extern FlightManager flightManager;

#endif