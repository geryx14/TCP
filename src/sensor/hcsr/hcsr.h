#ifndef HCSR_H
#define HCSR_H

#include <Arduino.h>

class Hcsr {
private:
    uint8_t trigPin;
    uint8_t echoPin;
    unsigned long interval;
    unsigned long previousMillis;
    float distance; // Simpan jarak terbaru

public:
    Hcsr(uint8_t trig, uint8_t echo, unsigned long interval);
    void init();
    float ReadUltrasonic();
    void loop();
    float getDistance(); // Getter untuk mengambil jarak
};

#endif
