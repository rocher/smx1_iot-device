#include "boot.h"
#include "wifi.h"

#include <WiFi.h>

int wifiReconnection = 0 ;

void wifi_setup() {
    pinMode(LED_CONNECT, OUTPUT);
    digitalWrite(LED_CONNECT, HIGH);

    WiFi.mode(WIFI_STA);
    WiFi.setHostname(SERIAL_NUMBER);
}

bool wifi_isConnected() {
    return (WiFi.status() == WL_CONNECTED);
}

bool wifi_status() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("[wifi] Connected to '%s'\n\r", WIFI_SSID);
        Serial.print("[wifi] IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.print("[wifi] Not connected, status: ");
        Serial.println(WiFi.status());
    }
    return false;
}

void wifi_reconnect() {
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("[wifi] IP address: ");
        Serial.println(WiFi.localIP());
        return;
    }

    digitalWrite(LED_CONNECT, HIGH);
    Serial.print("[wifi] Not connected, status: ");
    Serial.println(WiFi.status());

    if(wifiReconnection++ > WIFI_RECONNECT){
        Serial.println("[wifi] Too many reconnection attempts, rebooting...");
        boot_reboot();
    }

    Serial.printf("[wifi] Connecting to '%s' (%d)\n\r",
                  WIFI_SSID, wifiReconnection);

    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    for(int i = 0; i < WIFI_RETRIES; i++){
        if (WiFi.status() != WL_CONNECTED) {
            digitalWrite(LED_CONNECT, HIGH);
            delay(100);
            digitalWrite(LED_CONNECT, LOW);
            delay(500);
        }
        else {
            break;
        }
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("[wifi] Connected to '%s'\n\r", WIFI_SSID);
        Serial.print("[wifi] IP address: ");
        Serial.println(WiFi.localIP());
        digitalWrite(LED_CONNECT, LOW);
        wifiReconnection = 0;
    }
    else {
        Serial.println("[wifi] Failed to connect");
    }
}
