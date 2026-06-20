#include "PID.h"
#include <Arduino.h>

PID::PID(float p, float i, float d) : kp(p), ki(i), kd(d), prev_error(0), integral(0) {}

float PID::compute(float setpoint, float measured_value, float dt) {
    float error = setpoint - measured_value;
    
    // Proportional
    float P = kp * error;
    
    // Integral (Anti-Windup ile)
    integral += error * dt;
    integral = constrain(integral, -100, 100); // İntegral birikimini sınırla
    float I = ki * integral;
    
    // Derivative (Hız değişimi)
    float D = kd * ((error - prev_error) / dt);
    
    prev_error = error;
    
    return P + I + D;
}

void PID::reset() {
    integral = 0;
    prev_error = 0;
}