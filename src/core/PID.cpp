#include "PID.h"

PID::PID(float p, float i, float d)
    : kp(p), ki(i), kd(d), prev_error(0), integral(0) {}

float __not_in_flash_func(PID::compute)(float setpoint, float measured_value, float dt,
                                       float outputMin, float outputMax) {
    float error = setpoint - measured_value;

    float P = kp * error;
    float D = kd * ((error - prev_error) / dt);

    float candidateIntegral = integral + error * dt;
    candidateIntegral = constrain(candidateIntegral, -100.0f, 100.0f);
    float I = ki * candidateIntegral;

    float output = P + I + D;
    if (output > outputMax) {
        if (error > 0.0f) {
            candidateIntegral -= error * dt;
            candidateIntegral = constrain(candidateIntegral, -100.0f, 100.0f);
        }
        output = outputMax;
    } else if (output < outputMin) {
        if (error < 0.0f) {
            candidateIntegral -= error * dt;
            candidateIntegral = constrain(candidateIntegral, -100.0f, 100.0f);
        }
        output = outputMin;
    }

    if (output > outputMin && output < outputMax) {
        integral = candidateIntegral;
    }

    prev_error = error;
    return output;
}

void PID::reset() {
    integral   = 0.0f;
    prev_error = 0.0f;
}