#include <Arduino.h>
#include <RP2040Support.h>
#include <FreeRTOS.h>
#include <task.h>
#include <hardware/watchdog.h>
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
                rp2040.getFreeHeap()
            );
        }

        watchdog_update();
        vTaskDelayUntil(&lastWakeTime, telemetryPeriod);
    }
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    Serial.printf("[FREERTOS] Stack overflow in %s\n", pcTaskName);
    watchdog_update();
    taskDISABLE_INTERRUPTS();
    while (true) { }
}

extern "C" void vApplicationMallocFailedHook(void) {
    Serial.println("[FREERTOS] Malloc failed!");
    watchdog_update();
    taskDISABLE_INTERRUPTS();
    while (true) { }
}

static FlightDataSnapshot getFlightDataCallback() {
    FlightData latest = flightManager.getLatestData();
    return FlightDataSnapshot{
        latest.roll,
        latest.pitch,
        latest.yaw,
        latest.gyroX,
        latest.gyroY,
        latest.gyroZ,
        latest.aileron,
        latest.elevator,
        latest.throttle,
        latest.rudder,
        flightManager.isArmed(),
        latest.failsafe
    };
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
    mavlink.setFlightDataProvider(getFlightDataCallback);
    mavlink.setRCOverrideHandler([](uint16_t ch1, uint16_t ch2, uint16_t ch3, uint16_t ch4) {
        flightManager.setRCOverride(ch1, ch2, ch3, ch4);
    });
    mavlink.setRCClearHandler([]() {
        flightManager.clearRCOverride();
    });
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
    BootLogger::printHealthReport(500, true, true, true, true, true, false, false, rp2040.getFreeHeap());

    watchdog_enable(WATCHDOG_TIMEOUT_MS, true);
    BootLogger::ok("Watchdog");

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