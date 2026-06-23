#include "Sensors.h"

void SensorManager::init() {
    Wire.begin();
    Wire.setClock(400000);

    if (!mpu.begin()) {
        Serial.println("MPU6050 bulunamadi!");
    } else {
        mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
        mpu.setGyroRange(MPU6050_RANGE_500_DEG);
        mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    }

    #ifdef USE_GY87
      hasMag = mag.begin();
      if (!hasMag) {
          Serial.println("HMC5883L (magnetometre) bulunamadi!");
      }

      hasBaro = bmp.begin();
      if (!hasBaro) {
          Serial.println("BMP085 (barometre) bulunamadi!");
      }
    #endif
}

void SensorManager::update() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    this->ax = a.acceleration.x;
    this->ay = a.acceleration.y;
    this->az = a.acceleration.z;
    this->gx = g.gyro.x;
    this->gy = g.gyro.y;
    this->gz = g.gyro.z;

    #ifdef USE_GY87
      if (hasMag) {
          sensors_event_t mag_event;
          mag.getEvent(&mag_event);
          this->mx = mag_event.magnetic.x;
          this->my = mag_event.magnetic.y;
          this->mz = mag_event.magnetic.z;
      } else {
          this->mx = 0.0f;
          this->my = 0.0f;
          this->mz = 0.0f;
      }

      if (hasBaro) {
          sensors_event_t bmp_event;
          bmp.getEvent(&bmp_event);
          this->pressure = bmp_event.pressure;
      } else {
          this->pressure = 1013.25f;
      }
    #endif
}