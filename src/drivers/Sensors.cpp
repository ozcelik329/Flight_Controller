#include "Sensors.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#ifdef USE_GY87
  #include <Adafruit_HMC5883_U.h>
  #include <Adafruit_BMP085_U.h>
#endif

Adafruit_MPU6050 mpu;
#ifdef USE_GY87
  Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
  Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
#endif

void SensorManager::init() {
    Wire.setSDA(PIN_SDA);
    Wire.setSCL(PIN_SCL);
    Wire.begin();

    if (!mpu.begin()) {
        // Hata durumunda buraya bir debug logu eklenebilir
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);

    #ifdef USE_GY87
      mag.begin();
      bmp.begin();
    #endif
}

void SensorManager::update() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    // Basit bir açı hesaplama (Complementary Filter veya Mahony öncesi temel)
    this->roll = atan2(a.acceleration.y, a.acceleration.z) * RAD_TO_DEG;
    this->pitch = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * RAD_TO_DEG;
    
    // Yaw için pusula verisi (GY-87 varsa)
    #ifdef USE_GY87
        sensors_event_t mag_event;
        mag.getEvent(&mag_event);
        this->yaw = atan2(mag_event.magnetic.y, mag_event.magnetic.x) * RAD_TO_DEG;
    #endif
}

float SensorManager::getRoll() { return roll; }
float SensorManager::getPitch() { return pitch; }
float SensorManager::getYaw() { return yaw; }