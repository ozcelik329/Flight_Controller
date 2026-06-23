#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include "core/FlightManager.h"
#include "core/SystemTimer.h"
#include "utils/Logger.h"

FlightManager flightManager;

// Core 0: Sensör okuma + RC input
void taskSensor(void* pvParameters) {
    for (;;) {
        flightManager.update();
        vTaskDelay(pdMS_TO_TICKS(2)); // 500Hz
    }
}

// Core 1: PID + servo çıkışı (SystemTimer zaten Core 1'de çalışıyor)
void taskFlight(void* pvParameters) {
    SystemTimer::init();
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000)); // SystemTimer kendi döngüsünü yönetiyor
    }
}

void setup() {
    Serial.begin(115200);
    Logger::init();
    flightManager.init();
    Logger::log("AeroPico FC Baslatildi!");

    // Core 0 task'ı
xTaskCreateAffinitySet(
        taskSensor,
        "SensorTask",
        2048,
        NULL,
        2,
        (1 << 0),   // Core 0
        NULL        // task handle
    );

    xTaskCreateAffinitySet(
        taskFlight,
        "FlightTask",
        2048,
        NULL,
        3,
        (1 << 1),   // Core 1
        NULL        // task handle
    );

    vTaskStartScheduler();
}

void loop() {
    // FreeRTOS scheduler devralıyor, loop boş kalıyor
}