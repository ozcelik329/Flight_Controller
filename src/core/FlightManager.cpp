#include "FlightManager.h"

void FlightManager::init() {
    mutex_init(&_mutex);
    sensors.init();
    fusion.init(0.08f);
    rx.init();
}

void FlightManager::update() {
    sensors.update();
    performSensorFusion();
    rx.update();


    SensorBuffer buf = sensors.getLatest();

    mutex_enter_blocking(&_mutex);
    _gyroX = buf.gx;
    _gyroY = buf.gy;
    _gyroZ = buf.gz;

    if (!rx.isValid()) {
        _aileron  = PWM_NEUTRAL;
        _elevator = PWM_NEUTRAL;
        _throttle = PWM_MIN;
        _rudder   = PWM_NEUTRAL;
    } else {
        _aileron  = rx.getChannel(RC_ROLL_CHANNEL);
        _elevator = rx.getChannel(RC_PITCH_CHANNEL);
        _throttle = rx.getChannel(RC_THROTTLE_CHANNEL);
        _rudder   = rx.getChannel(RC_YAW_CHANNEL);
    }
    mutex_exit(&_mutex);
    updateArmDisarm(_throttle, _rudder);
}

void FlightManager::performSensorFusion() {
    SensorBuffer buf = sensors.getLatest();

    float ax = buf.ax;
    float ay = buf.ay;
    float az = buf.az;
    float gx = buf.gx * DEG_TO_RAD;
    float gy = buf.gy * DEG_TO_RAD;
    float gz = buf.gz * DEG_TO_RAD;

    #ifdef USE_GY87
        fusion.update(gx, gy, gz, ax, ay, az, buf.mx, buf.my, buf.mz);
    #else
        fusion.updateIMU(gx, gy, gz, ax, ay, az);
    #endif

    mutex_enter_blocking(&_mutex);
    _roll  = fusion.getRoll();
    _pitch = fusion.getPitch();
    _yaw   = fusion.getYaw();
    mutex_exit(&_mutex);
}

// getter'lar aynı kalıyor
float FlightManager::getRoll() {
    mutex_enter_blocking(&_mutex);
    float v = _roll;
    mutex_exit(&_mutex);
    return v;
}
float FlightManager::getPitch() {
    mutex_enter_blocking(&_mutex);
    float v = _pitch;
    mutex_exit(&_mutex);
    return v;
}
float FlightManager::getYaw() {
    mutex_enter_blocking(&_mutex);
    float v = _yaw;
    mutex_exit(&_mutex);
    return v;
}
float FlightManager::getGyroX() {
    mutex_enter_blocking(&_mutex);
    float v = _gyroX;
    mutex_exit(&_mutex);
    return v;
}
float FlightManager::getGyroY() {
    mutex_enter_blocking(&_mutex);
    float v = _gyroY;
    mutex_exit(&_mutex);
    return v;
}
float FlightManager::getGyroZ() {
    mutex_enter_blocking(&_mutex);
    float v = _gyroZ;
    mutex_exit(&_mutex);
    return v;
}
uint16_t FlightManager::getAileron() {
    mutex_enter_blocking(&_mutex);
    uint16_t v = _aileron;
    mutex_exit(&_mutex);
    return v;
}
uint16_t FlightManager::getElevator() {
    mutex_enter_blocking(&_mutex);
    uint16_t v = _elevator;
    mutex_exit(&_mutex);
    return v;
}
uint16_t FlightManager::getThrottle() {
    mutex_enter_blocking(&_mutex);
    uint16_t v = _throttle;
    mutex_exit(&_mutex);
    return v;
}
uint16_t FlightManager::getRudder() {
    mutex_enter_blocking(&_mutex);
    uint16_t v = _rudder;
    mutex_exit(&_mutex);
    return v;
}
void FlightManager::updateArmDisarm(uint16_t throttle, uint16_t rudder) {
    uint32_t now = millis();

    if (!_armed) {
        // ARM: throttle aşağı + rudder sağ, 1 saniye tut
        if (throttle < ARM_THROTTLE_MAX && rudder > ARM_RUDDER_MIN) {
            if (_armHoldStart == 0) _armHoldStart = now;
            if (now - _armHoldStart >= ARM_HOLD_MS) {
                _armed = true;
                _armHoldStart = 0;
                Serial.println("[ARM] Sistem arm edildi!");
            }
        } else {
            _armHoldStart = 0;
        }
    } else {
        // DISARM: throttle aşağı + rudder sol, 1 saniye tut
        if (throttle < ARM_THROTTLE_MAX && rudder < DISARM_RUDDER_MAX) {
            if (_disarmHoldStart == 0) _disarmHoldStart = now;
            if (now - _disarmHoldStart >= ARM_HOLD_MS) {
                _armed = false;
                _disarmHoldStart = 0;
                Serial.println("[ARM] Sistem disarm edildi!");
            }
        } else {
            _disarmHoldStart = 0;
        }
    }
}