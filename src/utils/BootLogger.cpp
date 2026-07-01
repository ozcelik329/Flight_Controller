#include "BootLogger.h"
#include "USBManager.h"

void BootLogger::printBanner() {
    USBManager::println("==============================");
    USBManager::println(" AeroPico FC v0.2.0");
    USBManager::println(" Target : Raspberry Pi Pico 2");
    USBManager::println(" CPU    : RP2350");
    USBManager::println("==============================");
}

void BootLogger::printReadyMessage() {
    USBManager::println("");
    USBManager::println("System Ready.");
    USBManager::println("Loop Frequency: 500Hz");
}

void BootLogger::printHealthReport(
    uint32_t loopRateHz,
    bool imuOk,
    bool baroOk,
    bool magOk,
    bool receiverOk,
    bool dmaOk,
    bool armed,
    bool failsafe,
    uint32_t heapBytes
) {
    USBManager::println("");
    USBManager::println("========================");
    USBManager::println("SYSTEM HEALTH");
    USBManager::println("========================");
    USBManager::printf("CPU Temp........%s\n", "N/A");
    USBManager::printf("Loop Rate.......%lu Hz\n", loopRateHz);
    USBManager::printf("IMU..............%s\n", imuOk ? "OK" : "FAIL");
    USBManager::printf("Barometer........%s\n", baroOk ? "OK" : "FAIL");
    USBManager::printf("Compass..........%s\n", magOk ? "OK" : "FAIL");
    USBManager::printf("Receiver.........%s\n", receiverOk ? "OK" : "FAIL");
    USBManager::printf("GPS..............Not Connected\n");
    USBManager::printf("DMA..............%s\n", dmaOk ? "OK" : "FAIL");
    USBManager::printf("Heap.............%lu KB\n", heapBytes / 1024);
    USBManager::printf("Failsafe.........%s\n", failsafe ? "ON" : "OFF");
    USBManager::printf("Armed............%s\n", armed ? "YES" : "NO");
    USBManager::println("Flight Mode......MANUAL");
}

void BootLogger::ok(const char* name) {
    USBManager::printf("[BOOT] %-18s OK\n", name);
}

void BootLogger::okWithValue(const char* name, const char* value) {
    USBManager::printf("[BOOT] %-18s OK (%s)\n", name, value);
}

void BootLogger::fail(const char* name, const char* reason) {
    USBManager::printf("[BOOT] %-18s FAILED\n", name);
    USBManager::printf("       %s\n", reason);
}

void BootLogger::warn(const char* name, const char* reason) {
    USBManager::printf("[BOOT] %-18s WARN\n", name);
    USBManager::printf("       %s\n", reason);
}

void BootLogger::info(const char* text) {
    USBManager::println(text);
}
