#include "SystemTimer.h"

void SystemManager::init() {
    // 1. İşlemciyi 250MHz'e çıkar
    set_sys_clock_khz(250000, true);
    
    // 2. Core 1'i başlat
    multicore_launch_core1(core1_entry);
}

void SystemManager::core1_entry() {
    // Bu döngü SADECE Core 1'de çalışacak
    // Output motor;
    while (true) {
        // PID.compute();
        // Output.update();
        // PID::compute(); // İleride bu sınıfı yazınca aktif edilecek
        // motor.write(throttle); // İleride motor çıkışı buraya gelecek
        
        // Sabit 400Hz (2.5ms) döngü süresi için:
        sleep_us(2500); 
    }
}