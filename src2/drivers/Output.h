#ifndef OUTPUT_H
#define OUTPUT_H

#include <Arduino.h>
#include "../config.h"

void outputInit();
void writeMotors(int throttle, int roll, int pitch, int yaw);

#endif