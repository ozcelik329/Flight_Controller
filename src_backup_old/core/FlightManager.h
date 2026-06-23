#ifndef FLIGHT_MANAGER_H
#define FLIGHT_MANAGER_H

#include <Arduino.h>
#include "../drivers/Sensors.h"
#include "../drivers/RX.h"
#include "SensorFusion.h"

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

  private:
    SensorManager sensors;
    RXManager rx;
    SensorFusion fusion;

    volatile float roll;
    volatile float pitch;
    volatile float yaw;

    volatile float gyroX;
    volatile float gyroY;
    volatile float gyroZ;

    volatile uint16_t aileron;
    volatile uint16_t elevator;
    volatile uint16_t throttle;
    volatile uint16_t rudder;

    void performSensorFusion();
};

extern FlightManager flightManager;

#endif