![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![C++](https://img.shields.io/badge/Language-C++-blue)
# AeroPico FC: Raspberry Pi Pico Fixed-Wing Flight Controller

**AeroPico FC** is a modular, lightweight flight controller software designed for fixed-wing aircraft, leveraging the power of the **Raspberry Pi Pico**. It provides a customizable, low-cost flight control solution for hobbyists and researchers.

> **Note:** This project is currently in the **prototype phase**. It is intended for educational and experimental purposes and should not be used in critical industrial or commercial applications.

---

### 🏗 System Architecture

The software follows a modular design to separate low-level hardware abstraction from high-level flight logic.

### 📂 Project Structure

| Module | Description |
| --- | --- |
| `src/main.cpp` | Entry point managing the `setup()` and `loop()` control cycles. |
| `src/config.h` | Central configuration for pins, PID constants, and RC parameters. |
| `src/core/` | Flight management, Madgwick sensor fusion, and PID controller loops. |
| `src/drivers/` | Hardware abstraction layers (MPU6050, GY-87, SBUS, PWM). |
| `src/telemetry/` | MAVLink-based communication protocols for GCS integration. |
| `src/utils/` | Logger and mathematical helper functions. |
```mermaid
graph TD
    %% Core 0 - Data Acquisition
    subgraph C0 ["CORE 0: DATA ACQUISITION"]
        IMU["MPU6050 IMU"] --> SP["SENSOR PROCESSING"]
        SP --> MF["MADGWICK FILTER"]
        RC["RC RECEIVER"] --> MX["🔒 MUTEX"]
        GPS["GPS MODULE"] --> MX
        MF --> MX
    end

    %% Core 1 - Flight Control
    subgraph C1 ["CORE 1: FLIGHT CONTROL"]
        OP["ANGLE PID (OUTER)"] --> IP["RATE PID (INNER)"]
        IP --> FM["MIXER"]
        FM --> FSM{"FAILSAFE"}
        FSM --> SC["SAFETY LIMITS"]
        NAV["NAV & MAVLINK"]
    end

    %% Cross-Core
    MX -- "Euler & RC" --> OP
    MX -. "Gyro Rates" .-> IP
    MX -- "Pos & Heading" --> NAV
    NAV <==> MAV["MAVLINK INTERFACE"]

    %% Actuation
    SC --> A["AILERON"]
    SC --> E["ELEVATOR"]
    SC --> R["RUDDER"]
    SC --> T["THROTTLE"]

    %% Monochrome Styling
    classDef default fill:#fff,stroke:#000,stroke-width:1px,color:#000;
    classDef core fill:#f0f0f0,stroke:#000,stroke-width:2px,stroke-dasharray: 5 5;
    classDef mutex fill:#000,stroke:#000,color:#fff;
    
    class C0,C1 core;
    class MX mutex;

```
---

### 🚀 Roadmap

| Feature | Status |
| --- | --- |
| Basic Flight Control Loop | ✅ Completed |
| SBUS RC Input | ✅ Completed |
| MPU6050 + GY-87 Support | ✅ Completed |
| MAVLink Telemetry | ⏳ In Progress |
| Failsafe & Error Tolerance | ⏳ In Progress |
| GCS Integration | 📅 Planned |

---

### 🛠 How to Build

1. **Clone** this repository.
2. Open the project in **PlatformIO** (VS Code).
3. Ensure your `platformio.ini` is configured for the `earlephilhower` core.
4. Run the **Build** command.
5. Copy the generated `firmware.uf2` file to your Raspberry Pi Pico in **BOOTSEL** mode.

---

### 💡 Contribute

I welcome contributions! If you have suggestions, bug reports, or feature requests, feel free to **open an Issue** or submit a **Pull Request**.

*Developed by Muhammed Fatih Emre Özçelik*
*Copyright © 2026 Muhammed Fatih Emre Özçelik. All rights reserved.*

---

