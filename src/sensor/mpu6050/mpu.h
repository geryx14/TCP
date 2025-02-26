#ifndef MPU6050_H
#define MPU6050_H

#include <Wire.h>
#include <Arduino.h>

class MPU6050 {
public:
    MPU6050(uint8_t address = 0x68);
    void begin();
    void loop();  // Tambahkan loop untuk membaca secara periodik
    void readGyroXY(float &gx, float &gy);
    
private:
    uint8_t devAddr;
    float gx, gy;
    unsigned long lastReadTime;
    const unsigned long readInterval;

    void writeRegister(uint8_t reg, uint8_t value);
    int16_t readRegister16(uint8_t reg);
};

#endif
