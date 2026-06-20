#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "../config.h"

class SensorManager {
  public:
    void init();        // Sensörleri I2C üzerinden başlat
    void update();      // Gyro/Accel verilerini oku
    
    float getRoll();    // Hesaplanan açıları döndür
    float getPitch();
    float getYaw();

  private:
    float roll, pitch, yaw;
};

#endif