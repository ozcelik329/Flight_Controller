#include "Output.h"
#include <Servo.h>

static Servo srvAileron;
static Servo srvElevator;
static Servo srvRudder;
static Servo srvMotor;

void outputInit() {
    srvAileron.attach(PIN_AILERON);
    srvElevator.attach(PIN_ELEVATOR);
    srvRudder.attach(PIN_RUDDER);
    srvMotor.attach(PIN_THROTTLE);

    srvAileron.writeMicroseconds(PWM_NEUTRAL);
    srvElevator.writeMicroseconds(PWM_NEUTRAL);
    srvRudder.writeMicroseconds(PWM_NEUTRAL);
    srvMotor.writeMicroseconds(PWM_MIN);
}

void writeMotors(int throttle, int roll, int pitch, int yaw) {
    srvAileron.writeMicroseconds(constrain(roll, PWM_MIN, PWM_MAX));
    srvElevator.writeMicroseconds(constrain(pitch, PWM_MIN, PWM_MAX));
    srvRudder.writeMicroseconds(constrain(yaw, PWM_MIN, PWM_MAX));
    srvMotor.writeMicroseconds(constrain(throttle, PWM_MIN, PWM_MAX));
}
