#include <Arduino.h>
#include "config.h"
#include "drivers/Sensors.h"
#include "drivers/RX.h"
#include "core/SystemTimer.h"

// Modüllerimizin nesnelerini oluşturuyoruz
SensorManager sensors;
RXManager rx;

void setup() {
  // Debug ve GCS için Seri port
  Serial.begin(115200);
  
  // 1. Sistem ayarlarını yap ve Core 1'i başlat
  SystemManager::init();
  
  // 2. Sürücüleri başlat
  sensors.init();
  rx.init();
  
  Serial.println("AeroPico FC Baslatildi ve RX Hazir!");
}

void loop() {
  // --- Core 0: Sensör ve Veri İşleme ---
  
  // Sensör verilerini güncelle
  sensors.update();
  
  // Kumanda sinyallerini oku
  rx.update();
  
  // Örnek: Kumanda 2. kanal (Throttle) değerini seri porttan izle
  // static uint32_t lastPrint = 0;
  // if (millis() - lastPrint > 100) {
  //   Serial.print("Throttle: ");
  //   Serial.println(rx.getChannel(2));
  //   lastPrint = millis();
  // }
  
  // Not: PID ve Output döngüleri SystemTimer.cpp (Core 1) içinde çalışıyor.
}