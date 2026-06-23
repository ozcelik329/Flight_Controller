#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include "hardware/watchdog.h"
#include "core/FlightManager.h"
#include "core/SystemTimer.h"
#include "utils/Logger.h"
#include "telemetry/MavlinkHandler.h"
#include "telemetry/Blackbox.h"

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

        float roll  = flightManager.getRoll();
        float pitch = flightManager.getPitch();
        float yaw   = flightManager.getYaw();
        float gx    = flightManager.getGyroX();
        float gy    = flightManager.getGyroY();
        float gz    = flightManager.getGyroZ();
        uint16_t thr = flightManager.getThrottle();
        uint16_t ail = flightManager.getAileron();
        uint16_t ele = flightManager.getElevator();
        uint16_t rud = flightManager.getRudder();

        mavlink.sendAttitude(roll, pitch, yaw, gx, gy, gz);
        mavlink.sendRCChannels(ail, ele, thr, rud);
        mavlink.sendSysStatus(true, false);

        // Blackbox: 50 Hz
        blackbox.log(roll, pitch, yaw, gx, gy, gz,
                     thr, ail, ele, rud, false);

        vTaskDelay(pdMS_TO_TICKS(20)); // 50 Hz
    }
}

void setup() {
    Serial.begin(115200);

    watchdog_enable(WATCHDOG_TIMEOUT_MS, true);

    Logger::init();
    flightManager.init();
    mavlink.init();
    blackbox.init();

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