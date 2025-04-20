#include "setup.h"

#include <ArduinoJson.h>
#include <LittleFS.h>
#include <map>

std::map<String, String> nameOf;

void setup_clear() {
    File setupFile = LittleFS.open("/setup.json", "w");
    nameOf.clear();

    if (setupFile) {
        setupFile.close();
        Serial.println("[setup] File & setup cleared");
    } else {
        Serial.println("[setup] Error clearing setup");
    }
}

void setup_load() {
    File setupFile = LittleFS.open("/setup.json", "r");

    if (setupFile) {
        String json = setupFile.readString();
        setupFile.close();

        // Parse the JSON string and populate the name map
        DynamicJsonDocument  doc(1024);
        DeserializationError error = deserializeJson(doc, json);
        if (!error) {
            for (JsonPair kv : doc.as<JsonObject>()) {
                nameOf[kv.key().c_str()] = kv.value().as<String>();
                Serial.printf("[setup] Load %s = %s\n\r",
                    kv.key().c_str(), kv.value().as<String>().c_str());
            }
        } else {
            Serial.println("[setup] Error parsing setup.json");
        }
    } else {
        Serial.println("[setup] File setup.json not found, using default values");
    }
}

void setup_save() {
    File setupFile = LittleFS.open("/setup.json", "w");

    if (setupFile) {
        // Create a JSON object and populate it with the name map
        DynamicJsonDocument doc(1024);
        for (const auto& kv : nameOf) {
            doc[kv.first.c_str()] = kv.second.c_str();
        }

        // Serialize the JSON object to the file
        serializeJson(doc, setupFile);
        setupFile.close();
        Serial.println("[setup] File setup.json saved");
    } else {
        Serial.println("[setup] Error opening setup.json for writing");
    }
}

void setup_setName(String key, String value) {
    nameOf[key] = value;
    Serial.printf("[setup] Set %s: %s\n\r", key.c_str(), value.c_str());
}

const char *setup_getName(char *key) {
    String key_str(key);
    if (nameOf.find(key_str) != nameOf.end()) {
        return nameOf[key_str].c_str();
    }
    return key;
}

String setup_getName(String key) {
    if (nameOf.find(key) != nameOf.end()) {
        return nameOf[key];
    }
    return key;
}
