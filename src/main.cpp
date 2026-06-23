#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include "hardware/watchdog.h"
#include "core/FlightManager.h"
#include "core/SystemTimer.h"
#include "utils/Logger.h"
#include "telemetry/MavlinkHandler.h"

FlightManager flightManager;

void taskSensor(void* pvParameters) {
    for (;;) {
        flightManager.update();
        watchdog_update();
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}

void taskFlight(void* pvParameters) {
    SystemTimer::init();
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void taskTelemetry(void* pvParameters) {
    for (;;) {
        mavlink.update();

        // 10 Hz attitude gönder
        mavlink.sendAttitude(
            flightManager.getRoll(),
            flightManager.getPitch(),
            flightManager.getYaw(),
            flightManager.getGyroX(),
            flightManager.getGyroY(),
            flightManager.getGyroZ()
        );

        mavlink.sendRCChannels(
            flightManager.getAileron(),
            flightManager.getElevator(),
            flightManager.getThrottle(),
            flightManager.getRudder()
        );

        mavlink.sendSysStatus(true, !flightManager.getRudder());

        vTaskDelay(pdMS_TO_TICKS(100)); // 10 Hz
    }
}

void setup() {
    Serial.begin(115200);

    watchdog_enable(WATCHDOG_TIMEOUT_MS, true);

    Logger::init();
    flightManager.init();
    mavlink.init();

    if (watchdog_caused_reboot()) {
        Logger::log("[WATCHDOG] Onceki oturum watchdog ile resetlendi!");
    }

    Logger::log("AeroPico FC Baslatildi!");

    xTaskCreateAffinitySet(
        taskSensor, "SensorTask",
        2048, NULL, 2,
        (1 << 0), NULL
    );

    xTaskCreateAffinitySet(
        taskFlight, "FlightTask",
        2048, NULL, 3,
        (1 << 1), NULL
    );

    xTaskCreateAffinitySet(
        taskTelemetry, "TelemetryTask",
        2048, NULL, 1,
        (1 << 0), NULL
    );

    vTaskStartScheduler();
}

void loop() {}