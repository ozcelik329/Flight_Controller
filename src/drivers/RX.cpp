#include "RX.h"
#include "sbus.h" // Kütüphane adı

// SBUS objesi
bfs::SbusRx sbus_rx(&Serial1);
bfs::SbusData sbus_data;

void RXManager::init() {
    // PIN_SBUS (6) kullanılıyor
    sbus_rx.Begin();
}

void RXManager::update() {
    if (sbus_rx.Read()) {
        sbus_data = sbus_rx.data();
        
        // Kanalları 1000 - 2000 aralığına normalize et
        for (int i = 0; i < 16; i++) {
            channels[i] = map(sbus_data.ch[i], 172, 1811, 1000, 2000);
        }
    }
}

uint16_t RXManager::getChannel(int ch) {
    if (ch >= 0 && ch < 16) {
        return channels[ch];
    }
    return 1500; // Hata durumunda nötr değer
}