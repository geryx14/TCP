#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
#include <WiFi.h>

class TCPClient {
private:
    WiFiClient client;
    const char* serverIP;
    uint16_t serverPort;
    const String delimiter = "\r\n";
    unsigned long lastReconnectAttempt = 0;
    const unsigned long reconnectInterval = 5000; 

public:
    TCPClient(const char* ssid, const char* password, const char* ip, uint16_t port);
    void connectWiFi();
    bool connect();
    void reconnect();
    void loop();
    void sendInitMessage();
    void sendData(const String& jarak, const String& accelX, const String& gyroY);
    void checkResponse();
    void sendLoop(const String& jarak, const String& accelX, const String& gyroY, unsigned long sendInterval, unsigned long &previousMillis);
};

#endif