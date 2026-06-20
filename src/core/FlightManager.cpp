#include "FlightManager.h"

void FlightManager::init() {
    sensors.init();
    fusion.init(0.08f);
    rx.init();

    roll = 0.0f;
    pitch = 0.0f;
    yaw = 0.0f;

    aileron = PWM_NEUTRAL;
    elevator = PWM_NEUTRAL;
    throttle = PWM_MIN;
    rudder = PWM_NEUTRAL;
}

void FlightManager::update() {
    sensors.update();
    performSensorFusion();
    rx.update();

    gyroX = sensors.gx;
    gyroY = sensors.gy;
    gyroZ = sensors.gz;

    if (!rx.isValid()) {
        aileron = PWM_NEUTRAL;
        elevator = PWM_NEUTRAL;
        throttle = PWM_MIN;
        rudder = PWM_NEUTRAL;
    } else {
        aileron = rx.getChannel(RC_ROLL_CHANNEL);
        elevator = rx.getChannel(RC_PITCH_CHANNEL);
        throttle = rx.getChannel(RC_THROTTLE_CHANNEL);
        rudder = rx.getChannel(RC_YAW_CHANNEL);
    }
}

float FlightManager::getGyroX() {
    return gyroX;
}

float FlightManager::getGyroY() {
    return gyroY;
}

float FlightManager::getGyroZ() {
    return gyroZ;
}

void FlightManager::performSensorFusion() {
    float ax = sensors.ax;
    float ay = sensors.ay;
    float az = sensors.az;
    float gx = sensors.gx * DEG_TO_RAD;
    float gy = sensors.gy * DEG_TO_RAD;
    float gz = sensors.gz * DEG_TO_RAD;

    #ifdef USE_GY87
      float mx = sensors.mx;
      float my = sensors.my;
      float mz = sensors.mz;
      fusion.update(gx, gy, gz, ax, ay, az, mx, my, mz);
    #else
      fusion.updateIMU(gx, gy, gz, ax, ay, az);
    #endif

    roll = fusion.getRoll();
    pitch = fusion.getPitch();
    yaw = fusion.getYaw();
}

float FlightManager::getRoll() {
    return roll;
}

float FlightManager::getPitch() {
    return pitch;
}

float FlightManager::getYaw() {
    return yaw;
}

uint16_t FlightManager::getAileron() {
    return aileron;
}

uint16_t FlightManager::getElevator() {
    return elevator;
}

uint16_t FlightManager::getThrottle() {
    return throttle;
}

uint16_t FlightManager::getRudder() {
    return rudder;
}
