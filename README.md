![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange)
![RP2040](https://img.shields.io/badge/Platform-RP2040-7546C6)
![C++](https://img.shields.io/badge/Language-C++-00599C)
![MAVLink](https://img.shields.io/badge/Protocol-MAVLink-red)
![License](https://img.shields.io/badge/License-MIT-green)

# AeroPico FC : Fixed-Wing Flight Controller

**AeroPico FC** is an open-source flight controller firmware for fixed-wing UAVs, built on the RP2040 (Raspberry Pi Pico). It uses the chip's dual-core architecture to keep sensor fusion and flight control on separate cores — the same separation professional autopilots rely on, now accessible to anyone.

Whether you are a hobbyist building your first fixed-wing or a developer researching custom autopilot stacks, AeroPico FC gives you a clean, readable codebase you can actually understand and extend.

> **Note:** This project is currently in the **prototype phase** and is intended for educational and experimental use. It is not certified for commercial or safety-critical applications.

---

## Why AeroPico FC?

Most hobby-grade firmware is a black box. AeroPico FC is written to be readable first — every module has a single responsibility, every design decision is traceable in the code. You can start by tuning PID gains in `config.h` without touching anything else, and work your way down to the sensor fusion math when you are ready.

At the same time, the architecture does not cut corners. Dual-core task isolation, mutex-protected cross-core data sharing, a cascaded PID structure, and a hardware abstraction layer are patterns borrowed from professional embedded systems — not academic exercises.

---

## Key Features

- **Dual-core isolation** — Core 0 handles sensor reading and RC input; Core 1 runs the PID loop and drives servo outputs. They never block each other.
- **Cascaded PID** — Outer angle loop feeds a target rate into the inner rate loop, giving smoother and more precise attitude control than a single-loop approach.
- **Madgwick filter** — Efficient quaternion-based attitude estimation that works well even on resource-constrained hardware.
- **MAVLink ready** — Telemetry layer is structured for MAVLink integration with any standard GCS.
- **MPU6050 & GY-87 support** — Works with both a basic IMU and a 9-DOF module (with magnetometer).
- **Configurable** — Pins, PID constants, RC ranges, and loop frequency are all in one place: `config.h`.

---

## System Architecture

```mermaid
flowchart TD
    subgraph C0["Core 0 — Sensör & RX"]
        IMU["MPU6050 IMU"]
        SP["Sensör işleme\nHam ivme & jiroskop"]
        MF["Madgwick filtresi\nQuaternion → Euler açıları"]
        RC["PWM RC alıcısı\nKanal 1–4 (1000–2000 µs)"]
        MX["🔒 Mutex\nÇift-core veri koruması"]

        IMU --> SP --> MF --> MX
        RC --> MX
    end

    subgraph C1["Core 1 — PID & Çıkış"]
        OP["Outer loop: Angle PID\nHedef roll / pitch açısı"]
        IP["Inner loop: Rate PID\nHedef açısal hız → düzeltme"]
        FM["FixedWingMixer\nPID + RC → servo PWM"]
        SC["Güvenlik & sınırlama\nPWM 1000–2000 µs arası kısıt"]

        OP --> IP --> FM --> SC
    end

    MX -- "Euler açıları / RC girişi" --> OP
    MX -. "Gyro hızları" .-> IP

    SC --> A["Aileron servo"]
    SC --> E["Elevator servo"]
    SC --> R["Rudder servo"]
    SC --> T["Throttle ESC"]

    subgraph FUTURE["Gelecekte eklenecek"]
        direction LR
        TEL["Telemetri\nWiFi / LoRa + MAVLink"]
        CAM["Kamera\nESP32-CAM / RP Zero"]
        GCS["GCS — Rasp 3B+\nProcessing / uygulama + anten"]
    end
```

---

## Project Structure

| Module | Description |
|---|---|
| `src/main.cpp` | Entry point, `setup()` and `loop()` |
| `src/config.h` | Pins, PID gains, RC parameters — start here |
| `src/core/` | Flight manager, Madgwick filter, PID controller, mixer |
| `src/drivers/` | Hardware abstraction: MPU6050, GY-87, PWM output, RC input |
| `src/telemetry/` | MAVLink communication layer (in progress) |
| `src/utils/` | Logger, math helpers |

---

## Performance

| Parameter | Value |
|---|---|
| Control loop frequency | 500 Hz (configurable) |
| Cross-core data sharing | Mutex-protected, no busy-wait |
| Attitude estimation | Madgwick (quaternion, tunable β) |
| PWM output range | 1000–2000 µs |

---

## Roadmap

| Feature | Status |
|---|---|
| Basic flight control loop | ✅ Done |
| PWM RC input | ✅ Done |
| MPU6050 + GY-87 support | ✅ Done |
| Mutex-protected dual-core sharing | ✅ Done |
| MAVLink telemetry | ⏳ In progress |
| Failsafe & signal-loss handling | ⏳ In progress |
| WiFi / LoRa telemetry (Pico W) | 📅 Planned |
| GCS with tracking antenna | 📅 Planned |
| Camera integration (ESP32-CAM) | 📅 Planned |

---

## How to Build

1. Clone this repository.
2. Open the project in **PlatformIO** (VS Code).
3. Verify `platformio.ini` targets the `earlephilhower` RP2040 core.
4. Run **Build**.
5. Copy the generated `firmware.uf2` to your Pico in **BOOTSEL** mode.

---

## Contribute

Issues and pull requests are welcome. If you are new to embedded systems and want to understand how a flight controller works from the ground up, this codebase is a good place to start — open an issue and ask questions freely.

---

*Developed by Muhammed Fatih Emre Özçelik*  
*Copyright © 2026 Muhammed Fatih Emre Özçelik. All rights reserved.*
