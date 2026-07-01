#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PID {
  public:
    PID(float kp, float ki, float kd);
    float compute(float setpoint, float measured_value, float dt,
                  float outputMin = -1000.0f, float outputMax = 1000.0f);
    void reset();

  private:
    float kp, ki, kd;
    float prev_error;
    float integral;
};

#endif