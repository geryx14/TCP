#include "hcsr.h"

Hcsr::Hcsr(uint8_t trig, uint8_t echo, unsigned long interval)
    : trigPin(trig), echoPin(echo), interval(interval), previousMillis(0), distance(0) {}

void Hcsr::init() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

float Hcsr::ReadUltrasonic() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2; 

    return distance;
}

void Hcsr::loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        distance = ReadUltrasonic(); // Simpan hasil pembacaan ke atribut distance
    }
}

float Hcsr::getDistance() {
    return distance; // Return jarak terbaru
}
