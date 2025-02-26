#include "tcp.h"

const char* ssid;
const char* password;

TCPClient::TCPClient(const char* ssid, const char* password, const char* ip, uint16_t port) : serverIP(ip), serverPort(port) {
    ::ssid = ssid;
    ::password = password;
}

void TCPClient::connectWiFi() {
    Serial.print("[WiFi] Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[WiFi] Connected");
    Serial.print("[WiFi] IP Address: ");
    Serial.println(WiFi.localIP());
}

bool TCPClient::connect() {
    if (client.connect(serverIP, serverPort)) {
        Serial.println("[TCP] Connected to Server A");
        sendInitMessage();
        return true;
    }
    Serial.println("[TCP] Connection failed!");
    return false;
}

void TCPClient::reconnect() {
    Serial.println("[TCP] Attempting to reconnect...");
    if (connect()) {
        Serial.println("[TCP] Reconnected successfully");
    } else {
        Serial.println("[TCP] Reconnect failed, will retry...");
    }
}

void TCPClient::loop() {
    static unsigned long lastReconnectAttempt = 0;
    const unsigned long reconnectInterval = 5000;

    if (!client.connected()) {
        unsigned long now = millis();
        if (now - lastReconnectAttempt > reconnectInterval) {
            lastReconnectAttempt = now;
            reconnect();
        }
    }
    checkResponse();
}

void TCPClient::sendInitMessage() {
    String message = "sensor1 init_data" + delimiter;
    client.print(message);
    Serial.println("[TCP] Sent: " + message);
}

void TCPClient::sendData(const String& jarak, const String& accelX, const String& gyroY) {
    if (!client.connected()) {
        Serial.println("[TCP] Disconnected, trying to reconnect...");
        return; // Jangan kirim data jika belum reconnect
    }
    
    String data = "{";
    data += "\"jarak\": \"" + jarak + "\",";
    data += "\"accelX\": \"" + accelX + "\",";
    data += "\"gyroY\": \"" + gyroY + "\"";
    data += "}";
    
    String message = "POST " + data + delimiter;
    client.print(message);
    Serial.println("[TCP] Sent: " + message);
}

void TCPClient::checkResponse() {
    while (client.available()) {
        String response = client.readStringUntil('\n');
        Serial.println("[TCP] Received: " + response);
    }
}

void TCPClient::sendLoop(const String& jarak, const String& accelX, const String& gyroY, unsigned long sendInterval, unsigned long &previousMillis) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= sendInterval) {
        previousMillis = currentMillis;

        if (!jarak.isEmpty() && !accelX.isEmpty() && !gyroY.isEmpty()) {
            sendData(jarak, accelX, gyroY);
        }
    }
}