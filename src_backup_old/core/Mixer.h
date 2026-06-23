#ifndef MIXER_H
#define MIXER_H

#include "../config.h"
#include <Servo.h> // Sabit kanatta PWM sinyali servo kütüphanesi ile daha stabil

class Mixer {
  public:
    void init();
    // Sabit kanat için kontrol yüzeyleri ve motor kontrolü
    void compute(float throttle, float roll_pid, float pitch_pid, float yaw_pid);
    
  private:
    Servo srvAileron, srvElevator, srvRudder, srvMotor;
};
#endif