#include "SystemTimer.h"
#include "FlightManager.h"
#include "FixedWingMixer.h"
#include "../drivers/Output.h"
#include <pico/stdlib.h>

static PID rollAnglePID(ANGLE_P_GAIN, ANGLE_I_GAIN, ANGLE_D_GAIN);
static PID pitchAnglePID(ANGLE_P_GAIN, ANGLE_I_GAIN, ANGLE_D_GAIN);
static PID rollRatePID(RATE_P_GAIN, RATE_I_GAIN, RATE_D_GAIN);
static PID pitchRatePID(RATE_P_GAIN, RATE_I_GAIN, RATE_D_GAIN);
static PID yawRatePID(RATE_P_GAIN, RATE_I_GAIN, RATE_D_GAIN);
static FixedWingMixer mixer;

volatile bool SystemManager::is_running = false;

static float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void SystemManager::init() {
    outputInit();

    MixerSettings settings;
    settings.rollGain = 1.0f;
    settings.pitchGain = 1.0f;
    settings.yawGain = 0.8f;
    settings.aileronTrim = 0;
    settings.elevatorTrim = 0;
    settings.rudderTrim = 0;
    settings.throttleTrim = 0;
    mixer.setSettings(settings);

    rollAnglePID.reset();
    pitchAnglePID.reset();
    rollRatePID.reset();
    pitchRatePID.reset();
    yawRatePID.reset();

    is_running = true;
    multicore_launch_core1(core1_entry);
}

void SystemManager::core1_entry() {
    uint32_t last_time = time_us_32();

    while (true) {
        uint32_t now = time_us_32();
        float dt = (now - last_time) / 1000000.0f;
        if (dt <= 0.0f) {
            dt = 0.001f;
        }
        last_time = now;

        float targetRoll = mapFloat(flightManager.getAileron(), 1000.0f, 2000.0f, -MAX_ROLL_ANGLE, MAX_ROLL_ANGLE);
        float targetPitch = mapFloat(flightManager.getElevator(), 1000.0f, 2000.0f, -MAX_PITCH_ANGLE, MAX_PITCH_ANGLE);
        float targetYawRate = mapFloat(flightManager.getRudder(), 1000.0f, 2000.0f, -MAX_YAW_RATE, MAX_YAW_RATE);

        float desiredRollRate = rollAnglePID.compute(targetRoll, flightManager.getRoll(), dt);
        float desiredPitchRate = pitchAnglePID.compute(targetPitch, flightManager.getPitch(), dt);

        float rollCorr = rollRatePID.compute(desiredRollRate, flightManager.getGyroX(), dt);
        float pitchCorr = pitchRatePID.compute(desiredPitchRate, flightManager.getGyroY(), dt);
        float yawCorr = yawRatePID.compute(targetYawRate, flightManager.getGyroZ(), dt);

        mixer.compute(
            flightManager.getThrottle(),
            rollCorr,
            pitchCorr,
            yawCorr,
            flightManager.getAileron(),
            flightManager.getElevator(),
            flightManager.getRudder());

        sleep_us(2500);
    }
}