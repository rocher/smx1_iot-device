#include "mqtt.h"
#include "boot.h"
#include "setup.h"

#include <ArduinoJson.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>

WiFiClient wClient;
MqttClient mqttClient(wClient);

bool mustReturnTopic             = false;
bool receivedInternalTemperature = false;
bool receivedExternalTemperature = false;
bool receivedHumidity            = false;

unsigned int mqttReceived = 0;
unsigned int mqttSent     = 0;

void onMqttMessage(int messageSize);

void mqtt_setup() {
    int  willLength = WILL_PAYLOAD_FMT.length() - 2 + SN_LENGTH + 1;
    char willPayload[willLength];

    pinMode(LED_MQTT_IN, OUTPUT);
    pinMode(LED_MQTT_OUT, OUTPUT);

    mqttClient.stop();
    delay(20);

    mqttClient.beginWill(WILL_TOPIC, willLength, WILL_RETAIN, WILL_QoS);
    sprintf(willPayload, WILL_PAYLOAD_FMT.c_str(), setup_getName(SERIAL_NUMBER));
    mqttClient.print(willPayload);
    mqttClient.endWill();

    mqttClient.setUsernamePassword(MQTT_USER, MQTT_PASSWORD);

    // Set the message receive callback
    mqttClient.onMessage(onMqttMessage);
}

bool mqtt_isConnected() {
    return mqttClient.connected();
}

void mqtt_reconnect() {
    if (mqttClient.connected()) {
        return;
    }

    Serial.print("[mqtt] Attempting to connect to the MQTT broker: ");
    Serial.println(MQTT_BROKER);

    if (!mqttClient.connect(MQTT_BROKER, MQTT_PORT)) {
        Serial.print("[mqtt] Connection failed! Error code = ");
        Serial.println(mqttClient.connectError());
    } else {
        Serial.println("[mqtt] Connected to the MQTT broker");
    }
}

void mqtt_poll() {
    mqttClient.poll();
}

void mqtt_subscribe(const char* topic, int qos) {
    Serial.print("[mqtt] Subscribing to topic: ");
    Serial.println(topic);
    mqttClient.subscribe(topic, qos);
}

void mqtt_startReturnTopic() {
    if (mustReturnTopic) {
        digitalWrite(LED_MQTT_OUT, HIGH);
    }
}

void mqtt_endReturnTopic() {
    if (mustReturnTopic) {
        delay(10);
        mustReturnTopic             = false;
        receivedInternalTemperature = false;
        receivedExternalTemperature = false;
        receivedHumidity            = false;
        digitalWrite(LED_MQTT_OUT, LOW);
    }
}

bool mqtt_receivedInternalTemperature() {
    return receivedInternalTemperature;
}

bool mqtt_receivedExternalTemperature() {
    return receivedExternalTemperature;
}

bool mqtt_receivedHumidity() {
    return receivedHumidity;
}

void mqtt_publish(const char* topic,
    const char*               payload,
    int                       qos,
    bool                      retained) {
    mqttClient.beginMessage(topic, retained, qos, false);
    mqttClient.print(payload);
    mqttClient.endMessage();
    mqttSent++;
}

unsigned int mqtt_sentMessages() {
    return mqttSent;
}

unsigned int mqtt_receivedMessages() {
    return mqttReceived;
}

void onMqttMessage(int messageSize) {
    digitalWrite(LED_MQTT_IN, HIGH);
    mqttReceived++;

    String topic = mqttClient.messageTopic();
    // Serial.print("mqtt rcv: ");
    // Serial.println(topic);

    StaticJsonDocument<256> doc;
    deserializeJson(doc, mqttClient);

    // Serial.print("mqtt cmd: ");
    // Serial.println(cmd);

    if (topic == TOPIC_CMD) {
        // Serial.print("[mqtt] Command: ");
        String cmd = doc["cmd"];
        if (cmd == CMD_REBOOT) {
            boot_reboot();
        } else if (cmd == CMD_REBOOT_LOAD) {
            boot_rebootToBootloader();
        } else if (cmd == CMD_TEMP_INT) {
            mustReturnTopic             = true;
            receivedInternalTemperature = true;
        } else if (cmd == CMD_TEMP_EXT) {
            mustReturnTopic             = true;
            receivedExternalTemperature = true;
        } else if (cmd == CMD_HUMIDITY) {
            mustReturnTopic  = true;
            receivedHumidity = true;
        } else {
            Serial.print("[mqtt] Unknown command: ");
            Serial.println(cmd);
        }
    } else if (topic == TOPIC_SETUP) {
        String cmd = doc["cmd"];
        // Serial.print("[mqtt] Setup ");
        // Serial.println(cmd);
        if (cmd == "set") {
            String pid = doc["pid"];
            if (pid = SERIAL_NUMBER) {
                String key   = doc["key"];
                String value = doc["value"];
                Serial.print("[mqtt] Setup set ");
                Serial.print(key.c_str());
                Serial.print(": ");
                Serial.println(value.c_str());
                setup_setName(key, value);
            }
        } else if (cmd == "get") {
            String pid = doc["pid"];
            if (pid = SERIAL_NUMBER) {
                String key   = doc["key"];
                String value = setup_getName(key);
                Serial.print("[mqtt] Setup get ");
                Serial.print(key);
                Serial.print(" = ");
                Serial.println(value);
                mqtt_publish(TOPIC_SETUP, value.c_str(), TOPIC_SETUP_QoS);
            }
        } else if (cmd == "clear") {
            String pid = doc["pid"];
            Serial.println("[mqtt] Setup clear");
            if (pid = SERIAL_NUMBER) {
                setup_clear();
            }
        } else if (cmd == "save") {
            String pid = doc["pid"];
            if (pid = SERIAL_NUMBER) {
                Serial.println("[mqtt] Setup save");
                setup_save();
            }
        } else {
            Serial.print("[mqtt] Unknown setup command: ");
            Serial.println(cmd);
        }
    } else {
        Serial.print("[mqtt] Unknown topic: ");
        Serial.println(topic);
    }

    delay(10);
    digitalWrite(LED_MQTT_IN, LOW);
}
