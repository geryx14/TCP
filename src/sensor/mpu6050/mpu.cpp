#include "mpu.h"

// Konstruktor
MPU6050::MPU6050(uint8_t address) : devAddr(address), lastReadTime(0), readInterval(500) {}

// Inisialisasi MPU6050
void MPU6050::begin() {
    Wire.begin();
    delay(100);
    writeRegister(0x6B, 0x00);  // Wake up sensor
    writeRegister(0x1B, 0x00);  // Set gyro full-scale ±250°/s
}

// Fungsi loop untuk membaca GyroX dan GyroY secara periodik
void MPU6050::loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastReadTime >= readInterval) {
        lastReadTime = currentMillis;
        readGyroXY(gx, gy);
    }
}

// Membaca kecepatan sudut Gyro X dan Gyro Y
void MPU6050::readGyroXY(float &gx, float &gy) {
    gx = readRegister16(0x43) / 131.0;  // Gyro X
    gy = readRegister16(0x45) / 131.0;  // Gyro Y
}

// Menulis ke register I2C
void MPU6050::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(devAddr);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

// Membaca data 16-bit dari register
int16_t MPU6050::readRegister16(uint8_t reg) {
    Wire.beginTransmission(devAddr);
    Wire.write(reg);
    Wire.endTransmission(false);
    
    Wire.requestFrom(devAddr, (uint8_t)2);
    if (Wire.available() >= 2) {
        int16_t highByte = Wire.read();
        int16_t lowByte = Wire.read();
        return (highByte << 8) | lowByte;
    }
    return 0;  // Return 0 jika tidak ada data
}
