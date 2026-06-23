#include "Mixer.h"
#include <Arduino.h>

void Mixer::init() {
    // Pico'da Servo PWM pinlerini başlatıyoruz
    srvAileron.attach(PIN_AILERON);
    srvElevator.attach(PIN_ELEVATOR);
    srvRudder.attach(PIN_RUDDER);
    srvMotor.attach(PIN_THROTTLE);

    // Motoru güvenli moda al
    srvMotor.writeMicroseconds(PWM_MIN);
}

void Mixer::compute(float throttle, float roll_pid, float pitch_pid, float yaw_pid) {
    // Sabit kanatlı uçaklarda çıkış değerleri
    // throttle: Kullanıcıdan gelen gaz (0-1000 arası değerler)
    // roll, pitch, yaw: PID'den gelen düzeltme değerleri
    
    // 1. Motor (Throttle) - Sadece gazı yönetir
    // İsteğe bağlı: Gazı "Cut-off" yani 1000'in altına düşmesini engelle
    int motorPWM = constrain(throttle, PWM_MIN, PWM_MAX);

    // 2. Kontrol Yüzeyleri (Servo PWM)
    // Nötr değerin (1500) üzerine PID düzeltmesini ekliyoruz
    int aileronPWM  = PWM_NEUTRAL + (int)roll_pid;
    int elevatorPWM = PWM_NEUTRAL + (int)pitch_pid;
    int rudderPWM   = PWM_NEUTRAL + (int)yaw_pid;

    // 3. Güvenlik Sınırları (Clamping)
    // Servoların fiziksel limitlerini korumak için 1000-2000 arasına sabitle
    srvAileron.writeMicroseconds(constrain(aileronPWM, PWM_MIN, PWM_MAX));
    srvElevator.writeMicroseconds(constrain(elevatorPWM, PWM_MIN, PWM_MAX));
    srvRudder.writeMicroseconds(constrain(rudderPWM, PWM_MIN, PWM_MAX));
    
    // 4. Motor Çıkışı
    srvMotor.writeMicroseconds(motorPWM);
}