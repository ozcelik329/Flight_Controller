#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <Wire.h>
#include "../config.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"

#ifdef USE_GY87
  #include <Adafruit_HMC5883_U.h>
  #include <Adafruit_BMP085_U.h>
#endif

// MPU6050 I2C adresi ve register'ları
#define MPU6050_ADDR        0x68
#define MPU6050_REG_PWR     0x6B
#define MPU6050_REG_ACCEL   0x3B  // 6 byte ivme
#define MPU6050_REG_GYRO    0x43  // 6 byte jiroskop
#define MPU6050_REG_GYRO_CFG  0x1B
#define MPU6050_REG_ACCEL_CFG 0x1C
#define MPU6050_REG_DLPF    0x1A

// Ham 14 byte: accel(6) + temp(2) + gyro(6)
#define MPU6050_RAW_LEN     14

// Ölçek faktörleri
#define ACCEL_SCALE  (1.0f / 4096.0f)  // ±8g → 4096 LSB/g
#define GYRO_SCALE   (1.0f / 65.5f)    // ±500°/s → 65.5 LSB/°/s

struct SensorBuffer {
    float ax, ay, az;
    float gx, gy, gz;
    #ifdef USE_GY87
        float mx, my, mz;
        float pressure;
    #endif
    uint32_t timestamp;
    bool valid;
};

class SensorManager {
  public:
    void init();
    void update();
    SensorBuffer getLatest();

    #ifdef USE_GY87
        bool hasMag  = false;
        bool hasBaro = false;
    #endif

  private:
    // DMA
    int _dma_chan = -1;
    uint8_t _dma_buf[MPU6050_RAW_LEN];
    uint8_t _reg_addr = MPU6050_REG_ACCEL;

    void _mpu_write_reg(uint8_t reg, uint8_t val);
    void _mpu_start_dma_read();
    bool _mpu_dma_ready();
    void _mpu_parse(SensorBuffer& buf);

    #ifdef USE_GY87
        Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
        Adafruit_BMP085_Unified  bmp = Adafruit_BMP085_Unified(10085);
    #endif

    SensorBuffer _buf[2];
    volatile uint8_t _writeIdx = 0;
    mutex_t _mutex;
};

#endif