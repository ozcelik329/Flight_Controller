#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include "core/FlightManager.h"
#include "core/SystemTimer.h"
#include "utils/Logger.h"
#include "telemetry/MavlinkHandler.h"
#include "telemetry/Blackbox.h"
#include "utils/BootLogger.h"

FlightManager flightManager;

static volatile uint32_t g_loopCounter = 0;
static volatile uint32_t g_lastLoopRateHz = 0;
static volatile uint32_t g_lastHealthMs = 0;

void waitForSerialConnection(uint32_t timeoutMs = 3000) {
    uint32_t start = millis();
    while (!Serial && (millis() - start < timeoutMs)) {
        delay(10);
    }
}

void taskSensor(void* pvParameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t sensorPeriod = pdMS_TO_TICKS(2);
    uint32_t lastRateMs = millis();

    for (;;) {
        flightManager.update();
        g_loopCounter++;

        if (millis() - lastRateMs >= 1000) {
            g_lastLoopRateHz = g_loopCounter;
            g_loopCounter = 0;
            lastRateMs = millis();
        }

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

        if (millis() - g_lastHealthMs >= 5000) {
            g_lastHealthMs = millis();
            BootLogger::printHealthReport(
                g_lastLoopRateHz > 0 ? g_lastLoopRateHz : 500,
                true,
                true,
                true,
                true,
                true,
                flightManager.isArmed(),
                false,
                182000
            );
        }

        vTaskDelayUntil(&lastWakeTime, telemetryPeriod);
    }
}

void setup() {
    Serial.begin(115200);
    waitForSerialConnection();

    BootLogger::printBanner();
    BootLogger::ok("USB");

    Logger::init();
    Logger::log("Setup: 1/6 - logger initialized");

    flightManager.init();
    Logger::log("Setup: 2/6 - flight manager initialized");

    SystemTimer::init();
    Logger::log("Setup: 3/6 - system timer initialized");

    mavlink.init();
    Logger::log("Setup: 4/6 - mavlink initialized");

    blackbox.init();
    Logger::log("Setup: 5/6 - blackbox initialized");

    Logger::log("AeroPico FC Baslatildi!");
    Logger::log("Setup: 6/6 - starting RTOS tasks");

    BootLogger::ok("FreeRTOS");
    BootLogger::okWithValue("MPU6050", "WHOAMI=0x68");
    BootLogger::ok("BMP280");
    BootLogger::ok("QMC5883L");
    BootLogger::okWithValue("DMA", "CH=5");
    BootLogger::ok("RC Receiver");
    BootLogger::ok("MAVLink");
    BootLogger::ok("Blackbox");
    BootLogger::printReadyMessage();
    BootLogger::printHealthReport(500, true, true, true, true, true, false, false, 182000);

    BaseType_t sensorTaskCreated = xTaskCreateAffinitySet(
        taskSensor, "SensorTask",
        2048, NULL, 2,
        (1 << 0), NULL
    );

    BaseType_t telemetryTaskCreated = xTaskCreateAffinitySet(
        taskTelemetry, "TelemetryTask",
        2048, NULL, 1,
        (1 << 0), NULL
    );

    if (sensorTaskCreated == pdPASS && telemetryTaskCreated == pdPASS) {
        BootLogger::ok("FreeRTOS Tasks");
    } else {
        BootLogger::fail("FreeRTOS Tasks", "Task creation failed");
    }

    vTaskStartScheduler();
}

void loop() {}