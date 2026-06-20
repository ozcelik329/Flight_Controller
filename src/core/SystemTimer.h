#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

#include <Arduino.h>
#include "pico/multicore.h"

class SystemManager {
  public:
    static void init();             // 250MHz ve Core 1 başlatma
    static void core1_entry();      // PID ve Motor kontrol döngüsü
    
    // Core 1'in döngü kontrolü için flag
    static volatile bool is_running;
};

#endif