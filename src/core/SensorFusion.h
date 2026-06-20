#ifndef SENSOR_FUSION_H
#define SENSOR_FUSION_H

#include <Arduino.h>

class SensorFusion {
  public:
    SensorFusion();
    void init(float beta = 0.1f);
    void update(float gx, float gy, float gz,
                float ax, float ay, float az,
                float mx, float my, float mz);
    void updateIMU(float gx, float gy, float gz,
                   float ax, float ay, float az);

    float getRoll() const;
    float getPitch() const;
    float getYaw() const;

  private:
    float q0, q1, q2, q3;
    float roll, pitch, yaw;
    float beta;
    uint32_t lastUpdate;

    void computeAngles();
    static float invSqrt(float x);
};

#endif