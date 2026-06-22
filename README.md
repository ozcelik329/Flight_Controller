
# AeroPico FC: Raspberry Pi Pico Fixed-Wing Flight Controller

This project is a **fixed-wing aircraft flight controller** running on the Raspberry Pi Pico. Currently, the project provides flight control software at a hobby/prototype level and should not be considered an industrially certified flight computer.

## 📂 File Structure and Architecture

The following modules are currently operational in the project:

* `src/main.cpp`
* Initializes the flight control loop with `setup()` and `loop()`.
* Configures the `FlightManager` and `SystemManager` objects.


* `src/config.h`
* Contains hardware pins, PWM constants, RC channel assignments, and PID/flight parameters.


* `src/def.h`
* Main loop timing constant (`LOOP_TIME`).


* `src/core/FlightManager.*`
* Reads sensors, executes fusion, and provides RC inputs.
* Forwards roll/pitch/yaw status data to the upper layer.


* `src/core/SensorFusion.*`
* Performs attitude estimation using Madgwick-style sensor fusion.
* Can operate in IMU-only mode or full GY-87 magnetometer-assisted mode.


* `src/core/SystemTimer.*`
* Manages the PID loop running on Core 1.
* Implements outer-loop attitude PID and inner-loop rate PID control.


* `src/core/FixedWingMixer.*`
* Performs roll/pitch/yaw mixing for fixed-wing aircraft.
* Coordinates motor ESC and servo signals.


* `src/core/PID.*`
* Provides a generalized PID controller.


* `src/drivers/Sensors.*`
* Reads MPU6050 and GY-87 sensors.
* Provides gyro/accel data and magnetometer/barometer data if available.


* `src/drivers/RX.*`
* Reads channels from an SBUS receiver and normalizes them to a 1000-2000 range.
* Switches to safe mode if the receiver signal is invalid.


* `src/drivers/Output.*`
* Generates ESC and servo PWM outputs.
* This is the final layer that writes to servos and the motor.


* `src/telemetry/`
* Contains files for the MAVLink-based telemetry interface.


* `src/utils/`
* Contains the logger and auxiliary mathematical functions.



## 🚀 Current Features

* Basic flight control loop for fixed-wing aircraft
* SBUS RC input
* MPU6050 + optional GY-87 sensor support
* Madgwick-style sensor fusion
* Outer-loop angle PID + inner-loop rate PID control structure
* Servo and ESC PWM output
* Fixed-wing mixer structure

## ⚠️ What is Missing?

This project is still in the prototype phase. The following critical features are either missing or not fully integrated:

* Industrial-grade safety and certification
* Full flight mode management (Stabilize, Manual, RTH, etc.)
* HIL/SIL verification and comprehensive testing infrastructure
* Fail-safe, watchdog, and power/EMI management
* Dynamic PID calibration and in-flight tuning
* GCS telemetry integration is not fully completed
* Redundant sensor control and error tolerance

## 🛠 How to Build

1. Ensure the correct environment is selected in `platformio.ini`. This project requires `framework = arduino` and `board_build.core = earlephilhower`.
2. Open the project in PlatformIO.
3. Run the "Build" command in VS Code.
4. Once the `firmware.uf2` file is generated, put the Pico in `BOOTSEL` mode and copy the file to the drive.

> Note: You must have `pio` or `platformio` installed on your local machine.
> `Wire.begin()` is used in `Sensors.cpp`; the `Wire.begin(SDA, SCL)` call is not supported in the Arduino-mbed based core.

## 🛠 Hardware Requirements

* Raspberry Pi Pico
* MPU6050 IMU (accelerometer + gyroscope)
* GY-87 module (optional): MPU6050 + HMC5883L magnetometer + BMP180 barometer
* SBUS-supported receiver
* ESC and brushless motor / servo setup

## Hardware Capabilities and Sensors

This flight controller features a modular design and can be easily configured via `config.h`:

* If `USE_GY87` is defined, the system expands sensor fusion to use magnetometer and barometer data.
* If `USE_GY87` is disabled, the system operates with a lower processor load using only basic IMU data (accelerometer + gyroscope).
* This structure is designed to support both basic MPU6050-based configurations and more advanced GY-87-equipped setups.

## 📋 Future Enhancements

* Activating MAVLink support in the `telemetry/` folder
* Black box recording and flight data logging
* Error tolerance and fail-safe mechanisms
* Pilot-selectable flight modes
* Advanced sensor filtering (EKF / Mahony / Kalman)

---

### 💡 Note to the User

This project requires comprehensive hardware testing, calibration, and safety checks before any actual flight attempts. The content in this README has been updated based on the current code, and the project remains at a prototype level.

### Acknowledgments / References

This project utilizes architectural concepts and mathematical implementations inspired by the following open-source works:

* **PicoFlight** by JoshuaPeddle: Referenced for its modular software architecture, IMU fusion implementation, and low-level hardware abstraction logic.

[[https://github.com/JoshuaPeddle/PicoFlight](https://github.com/JoshuaPeddle/PicoFlight)]

Developed by Muhammed Fatih Emre Özçelik/ozcelik329
Copyright © 2026 Muhammed Fatih Emre Özçelik. All rights reserved.
