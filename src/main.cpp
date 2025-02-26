#include "tcp/tcp.h"
#include "sensor/hcsr/hcsr.h"
#include "sensor/mpu6050/mpu.h"

/**
 * Inisiasi MPU6050
 */
MPU6050 mpu;

/**
 * Define ultrasonic
 */
#define TRIG_PIN 19
#define ECHO_PIN 23
Hcsr ultrasonic(TRIG_PIN, ECHO_PIN, 500);

/**
 * Koneksi TCP
 */
const char* SSID = "MatadorDev";
const char* PASSWORD = "bayardulu";
const char* SERVER_IP = "ec2-3-26-243-146.ap-southeast-2.compute.amazonaws.com";
const uint16_t SERVER_PORT = 5000;
TCPClient tcpClient(SSID, PASSWORD, SERVER_IP, SERVER_PORT);

/**
 * Interval pengiriman TCP
 */
const unsigned long sendInterval = 5000;
unsigned long previousMillis = 0;

/**
 * Variabel data sensor
 */
String distance = "";
String gx_str = "";
String gy_str = "";

void setup() {
    Serial.begin(115200);

    /**
     * Connect WiFi dan TCP
     */
    tcpClient.connectWiFi();
    tcpClient.connect();

    /**
     * Inisiasi sensor
     */
    ultrasonic.init();
    mpu.begin();
}

void loop() {
    /**
     * Pembacaan sensor MPU6050
     */
    mpu.loop();
    float gx, gy;
    mpu.readGyroXY(gx, gy);

    /**
     * Konversi nilai menjadi String
     */
    gx_str = String(gx, 2);
    gy_str = String(gy, 2);

    /**
     * Pembacaan sensor Ultrasonic
     */
    ultrasonic.loop();
    distance = String(ultrasonic.getDistance());

    /**
     * Panggil sendLoop untuk mengirim data jika sudah waktunya
     */
    tcpClient.sendLoop(distance, gx_str, gy_str, sendInterval, previousMillis);

    /**
     * Reconnect TCP jika terputus
     */
    tcpClient.loop();
}
