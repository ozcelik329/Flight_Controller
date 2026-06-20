#ifndef RX_H
#define RX_H

#include <Arduino.h>
#include "../config.h"

// SBUS kütüphanesini burada kullanıyoruz
#include "sbus.h" 

class RXManager {
  public:
    void init();
    void update();
    bool isValid() const;
    uint16_t getChannel(int ch) const;

  private:
    uint16_t channels[16];
    bool valid = false;
};

#endif