#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include "core/FlightManager.h"
#include "core/SystemTimer.h"
#include "utils/Logger.h"
#include "telemetry/MavlinkHandler.h"
#include "telemetry/Blackbox.h"

FlightManager flightManager;

void taskSensor(void* pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t sensorPeriod = pdMS_TO_TICKS(2);

    for (;;) {
        flightManager.update();
        vTaskDelayUntil(&lastWakeTime, sensorPeriod);
    }
}

void taskTelemetry(void* pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t telemetryPeriod = pdMS_TO_TICKS(20);

    for (;;) {
        mavlink.update();  // Tüm stream'ler burada yönetiliyor

        FlightData data = flightManager.getLatestData();
        blackbox.log(
            data.roll,
            data.pitch,
            data.yaw,
            data.gyroX,
            data.gyroY,
            data.gyroZ,
            data.throttle,
            data.aileron,
            data.elevator,
            data.rudder,
            data.failsafe
        );

        vTaskDelayUntil(&lastWakeTime, telemetryPeriod);
    }
}

void setup() {
    Serial.begin(115200);

    Logger::init();
    flightManager.init();
    SystemTimer::init();
    mavlink.init();
    blackbox.init();

    Logger::log("AeroPico FC Baslatildi!");

    xTaskCreateAffinitySet(
        taskSensor, "SensorTask",
        2048, NULL, 2,
        (1 << 0), NULL
    );

    xTaskCreateAffinitySet(
        taskTelemetry, "TelemetryTask",
        2048, NULL, 1,
        (1 << 0), NULL
    );

    vTaskStartScheduler();
}

void loop() {}