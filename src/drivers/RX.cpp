#include "RX.h"
#include "sbus.h"

bfs::SbusRx sbus_rx(&Serial1);
bfs::SbusData sbus_data;

void RXManager::init() {
    sbus_rx.Begin();
    valid = false;
    for (int i = 0; i < 16; i++) {
        channels[i] = PWM_NEUTRAL;
    }
}

void RXManager::update() {
    if (sbus_rx.Read()) {
        sbus_data = sbus_rx.data();
        valid = true;
        for (int i = 0; i < 16; i++) {
            channels[i] = constrain(map(sbus_data.ch[i], 172, 1811, 1000, 2000), 1000, 2000);
        }
    } else {
        valid = false;
    }
}

bool RXManager::isValid() const {
    return valid;
}

uint16_t RXManager::getChannel(int ch) const {
    if (ch >= 0 && ch < 16) {
        return channels[ch];
    }
    return PWM_NEUTRAL;
}