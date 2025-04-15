#include "web.h"
#include "mqtt.h"

#include <Arduino.h>
#include <LittleFS.h>
#include <WebServer.h>

WebServer server(80);

void handleIndex();

void web_setup() {
    // OLD: server.on("/", handleIndex);
    server.on("/", HTTP_GET, handleIndex);
    server.onNotFound([]() {
        Serial.println("[web] 404: Not found");
        server.send(404, "text/plain", "404: Not found");
    });
    server.begin();
}

void web_handleClient() {
    server.handleClient();
}

void handleIndex() {
    const int bufferSize = 640;
    const int indexSize  = bufferSize + 8 + 64 + 128;

    static bool init = false;
    static char buffer[bufferSize + 1];
    static char index[indexSize + 1];

    if (!init) {
        bzero(buffer, bufferSize + 1);
        File index_fmt = LittleFS.open("index.html", "r");
        if (index_fmt) {
            index_fmt.read((uint8_t*)buffer, bufferSize);
            index_fmt.close();
        }
    }
    sprintf(index, buffer, SERIAL_NUMBER,
        mqtt_receivedMessages(), mqtt_sentMessages());
    Serial.println("[web] 200: Sending index");
    server.send(200, "text/html", index);
}
