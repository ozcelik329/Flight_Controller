#include "SystemTimer.h"
#include "FlightManager.h"
#include "FixedWingMixer.h"
#include "../drivers/Output.h"
#include "../def.h"
#include "PID.h"
#include <pico/time.h>

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
    // Output sadece burada başlatılıyor (FixedWingMixer::init() çağrılmıyor)
    outputInit();

    MixerSettings settings;
    settings.rollGain  = 1.0f;
    settings.pitchGain = 1.0f;
    settings.yawGain   = 0.8f;
    settings.aileronTrim  = 0;
    settings.elevatorTrim = 0;
    settings.rudderTrim   = 0;
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
    // Sabit frekanslı döngü: LOOP_TIME µs = 500 Hz
    uint32_t next_tick = micros();

    while (true) {
        uint32_t now = micros();

        // Zamanlanmış tick'i bekle
        int32_t diff = (int32_t)(next_tick - now);
        if (diff > 0) {
            sleep_us(diff);
            continue;
        }

        // Gerçek dt: bir önceki tick'ten bu yana geçen süre
        float dt = (float)LOOP_TIME / 1000000.0f;
        next_tick += LOOP_TIME;
        if (!flightManager.isArmed()) {
            writeMotors(PWM_MIN, PWM_NEUTRAL, PWM_NEUTRAL, PWM_NEUTRAL);
            continue;
        }

        FlightData flightData = flightManager.getLatestData();

        // RC girişlerini aç sınıra çevir
        float targetRoll    = mapFloat(flightData.aileron,  1000.0f, 2000.0f, -MAX_ROLL_ANGLE,  MAX_ROLL_ANGLE);
        float targetPitch   = mapFloat(flightData.elevator, 1000.0f, 2000.0f, -MAX_PITCH_ANGLE, MAX_PITCH_ANGLE);
        float targetYawRate = mapFloat(flightData.rudder,   1000.0f, 2000.0f, -MAX_YAW_RATE,    MAX_YAW_RATE);

        // Cascaded PID: açı → açısal hız
        float desiredRollRate  = rollAnglePID.compute(targetRoll,  flightData.roll,  dt);
        float desiredPitchRate = pitchAnglePID.compute(targetPitch, flightData.pitch, dt);

        // Açısal hız → servo düzeltmesi
        float rollCorr  = rollRatePID.compute(desiredRollRate,  flightData.gyroX, dt);
        float pitchCorr = pitchRatePID.compute(desiredPitchRate, flightData.gyroY, dt);
        float yawCorr   = yawRatePID.compute(targetYawRate,     flightData.gyroZ, dt);

        mixer.compute(
            flightData.throttle,
            rollCorr,
            pitchCorr,
            yawCorr,
            flightData.aileron,
            flightData.elevator,
            flightData.rudder
        );
    }
}
