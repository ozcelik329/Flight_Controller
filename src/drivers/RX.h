#ifndef RX_H
#define RX_H

#include <Arduino.h>
#include "../config.h"

// SBUS kütüphanesini burada kullanıyoruz
#include "sbus.h" 

class RXManager {
  public:
    void init();        // Alıcıyı başlatır (Serial1 / GP6)
    void update();      // Veriyi okur ve normalize eder
    
    // İstenen kanal değerini 1000-2000 arası döndürür
    // ch: 0 (Aileron), 1 (Elevator), 2 (Throttle), 3 (Rudder) gibi...
    uint16_t getChannel(int ch); 

  private:
    uint16_t channels[16]; // 16 kanallı veriyi tutar
};

#endif