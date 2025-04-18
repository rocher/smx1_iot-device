#include "boot.h"
#include "core0.h"
#include "mqtt.h"
#include "ntp.h"
#include "temp.h"
#include "wifi.h"

#include <LittleFS.h>

void reconnect(char *status = (char *)"reconnect") {
    wifi_reconnect();
    if (wifi_isConnected()) {
        ntp_setup();
        ntp_update();
        mqtt_reconnect();
        if (mqtt_isConnected()) {
            mqtt_subscribe(TOPIC_CMD, TOPIC_CMD_QoS);

            char payload[128];
            struct tm timeinfo;
            time_t now = time(nullptr);
            gmtime_r(&now, &timeinfo);
            sprintf(payload,
                "{\"time\": \"%s\", \"pid\": \"%s\", \"status\": \"reconnected\"}",
                asctime(&timeinfo), SERIAL_NUMBER);
                mqtt_publish(TOPIC_STATUS, payload, TOPIC_STATUS_QoS);
        }
    }
}

void core0_setup() {
    pico_get_unique_board_id_string(SERIAL_NUMBER, SN_LENGTH);

    // Configura els GPIOs
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    LittleFS.begin();
    Serial.begin(115200);

    // Espera que el port sèrie estigui disponible
    // while (! Serial.available()) {}
    delay(5000);

    Serial.println("[setup] -----------------------------------------------");
    Serial.printf("[setup] %s %s\n\r", PROGRAM_NAME, PROGRAM_VERSION);
    Serial.printf("[setup] S/N: 0x%s\n\r", SERIAL_NUMBER);

    boot_setup();
    wifi_setup();
    mqtt_setup();

    reconnect((char *)"boot");

    delay(10);
}

void core0_loop() {
    unsigned long now = millis();

    static unsigned long lastWifiCheck = now;
    static unsigned long lastLed = 0;
    static PinStatus ledState = LOW;

    // Run this loop every 100ms
    delay(100);

    // struct tm timeinfo;
    // time_t nowTime = time(nullptr);
    // unsigned long ntpTime = nowTime;
    // gmtime_r(&nowTime, &timeinfo);

    // call poll() regularly to allow the library to receive MQTT messages and
    // send MQTT keep alives which avoids being disconnected by the broker
    mqtt_poll();

    if (!wifi_isConnected() || !mqtt_isConnected()) {
        digitalWrite(LED_CONNECT, ledState);
    }
    else {
        digitalWrite(LED_CONNECT, LOW);
    }

    if (now - lastWifiCheck > WIFI_PERIOD) {
        lastWifiCheck = now;
        if (!wifi_isConnected()) {
            reconnect();
        }
    }

    mqtt_startReturnTopic();

    if (mqtt_receivedInternalTemperature()) {
        char payload[128];
        sprintf(payload,
                "{\"pid\": \"%s\", \"value\": %4.2f}",
                SERIAL_NUMBER, analogReadTemp());
        mqtt_publish(TOPIC_RET_TEMP_INT, payload, TOPIC_RET_QoS);
    }

    if (mqtt_receivedExternalTemperature()) {
        // DHT11
        if (temp_hasDHT11()) {
            char payload[128];
            sprintf(payload,
                    "{\"pid\": \"%s\", \"tid\": \"dht11\", \"value\": %f}",
                    SERIAL_NUMBER, temp_getDHT11Temperature());
            mqtt_publish(TOPIC_RET_TEMP_EXT, payload, TOPIC_RET_QoS);
        }

        // DHT22
        if (temp_hasDHT22()) {
            char payload[128];
            sprintf(payload,
                    "{\"pid\": \"%s\", \"tid\": \"dht22\", \"value\": %f}",
                    SERIAL_NUMBER, temp_getDHT22Temperature());
            mqtt_publish(TOPIC_RET_TEMP_EXT, payload, TOPIC_RET_QoS);
        }

        // DS18B20
        for (int i=0; i<temp_getDS18B20Count(); i++) {
            if (temp_hasDS18B20(i)) {
                char payload[128];
                sprintf(payload,
                        "{\"pid\": \"%s\", \"tid\": \"%s\", \"value\": %f}",
                        SERIAL_NUMBER, temp_getDS18B20Address(i), temp_getDS18B20Temperature(i));
                mqtt_publish(TOPIC_RET_TEMP_EXT, payload, TOPIC_RET_QoS);
            }
        }
    }

    if (mqtt_receivedHumidity()) {
        // DHT11
        if (temp_hasDHT11()) {
            char payload[128];
            sprintf(payload,
                    "{\"pid\": \"%s\", \"tid\": \"dht11\", \"value\": %f}",
                    SERIAL_NUMBER, temp_getDHT11Humidity());
            mqtt_publish(TOPIC_RET_HUMIDITY, payload, TOPIC_RET_QoS);
        }

        // DHT22
        if (temp_hasDHT22()) {
            char payload[128];
            sprintf(payload,
                    "{\"pid\": \"%s\", \"tid\": \"dht22\", \"value\": %f}",
                    SERIAL_NUMBER, temp_getDHT22Humidity());
            mqtt_publish(TOPIC_RET_HUMIDITY, payload, TOPIC_RET_QoS);
        }
    }

    mqtt_endReturnTopic();

    if (LED_BLINK) {
        if (now - lastLed > LED_PERIOD) {
            lastLed = now;
            ledState = (ledState == LOW ? HIGH : LOW);
            digitalWrite(LED_BUILTIN, ledState);
        }
    }
}
