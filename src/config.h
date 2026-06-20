#ifndef CONFIG_H
#define CONFIG_H
// Sensör Tipi Seçimi
#define USE_GY87          // Eğer sadece MPU6050 kullanacaksan bu satırı yorum satırı yap (//)
#include <Arduino.h>

// --- Donanım Pin Atamaları ---
// I2C Pinleri (Sensörler için)
#define PIN_SDA 4
#define PIN_SCL 5

// RX (Alıcı) Pinleri
#define PIN_SBUS 6  // SBUS Sinyal pini 1 olarak güncellendi

// Çıkış (PWM) Pinleri
#define PIN_AILERON 16
#define PIN_ELEVATOR 17
#define PIN_RUDDER 18
#define PIN_THROTTLE 19

// --- Uçuş Parametreleri ---
#define PID_P_GAIN 0.5
#define PID_I_GAIN 0.01
#define PID_D_GAIN 0.05

#define PWM_MIN 1000
#define PWM_MAX 2000
#define PWM_NEUTRAL 1500

#endif