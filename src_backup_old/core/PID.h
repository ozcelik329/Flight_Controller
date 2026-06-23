#ifndef PID_H
#define PID_H

class PID {
  public:
    PID(float kp, float ki, float kd);
    float compute(float setpoint, float measured_value, float dt);
    void reset();

  private:
    float kp, ki, kd;
    float prev_error;
    float integral;
};

#endif