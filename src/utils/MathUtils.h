#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <Arduino.h>

class MathUtils {
  public:
    // Açıları -180 ile 180 derece arasına normalize eder
    static float normalizeAngle(float angle);
    
    // Değerleri belirli aralıkta tutar (constrain'in özelleştirilmiş hali)
    static float clamp(float value, float min, float max);
};

#endif