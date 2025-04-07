#include "temp.h"

#include <DallasTemperature.h>
#include <SimpleDHT.h>
#include <OneWire.h>

// Gestió de la temperatura amb DHT11 i DHT22
int dhtXError[2] = {-1};
float dhtXTemperature[2] = {0};
float dhtXHumidity[2] = {0};
SimpleDHT22 dht22(PIN_DHT22);
SimpleDHT11 dht11(PIN_DHT11);

// Gestió de la temperatura amb DS18B20
int dsXError[3] = {-1};
float dsXTemperature[3] = { DEVICE_DISCONNECTED_C };
DeviceAddress dsXAddress[3];
char dsXAddressStr[3][17] = {0};

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void temp_setup() {
    pinMode(LED_TEMP_READ, OUTPUT);

    sensors.begin();
    sensors.setResolution(12);
    Serial.printf("[temp] sensors count = %d\n\r", sensors.getDeviceCount());
    for (int i=0; i<sensors.getDeviceCount(); i++) {
        sensors.getAddress(&dsXAddress[i][0], i);
    }

    // Configuració del sensor DHT11 i DHT22
    // dht11.setPinInputMode(INPUT_PULLUP);
    // dht22.setPinInputMode(INPUT_PULLUP);

    // Configuració del sensor DS18B20
    sensors.begin();

    for (int i=0; i<sensors.getDeviceCount(); i++) {
        sensors.getAddress(dsXAddress[i], i);
        for (int j=0; j<8; j++) {
            sprintf(&dsXAddressStr[i][j*2], "%02X", dsXAddress[i][j]);
        }
        Serial.printf("[temp] DS18B20 num %d: address %s\n\r",
                      i, dsXAddressStr[i]);
    }
}

void temp_read() {
    digitalWrite(LED_TEMP_READ, HIGH);

    // Llegeix la temperatura i humitat del DHT11
    dhtXError[0] = dht11.read2(&dhtXTemperature[0], &dhtXHumidity[0], NULL);
    if (LOG_ERRORS && dhtXError[0] != SimpleDHTErrSuccess) {
        Serial.print("[temp] Read DHT11 failed, err=");
        Serial.print(SimpleDHTErrCode(dhtXError[0]));
        Serial.print(",");
        Serial.println(SimpleDHTErrDuration(dhtXError[0]));
    }

    delay(500);
    digitalWrite(LED_TEMP_READ, LOW);
    delay(500);
    digitalWrite(LED_TEMP_READ, HIGH);

    // Llegeix la temperatura i humitat del DHT22
    dhtXError[1] = dht22.read2(&dhtXTemperature[1], &dhtXHumidity[1], NULL);
    if (LOG_ERRORS && dhtXError[1] != SimpleDHTErrSuccess) {
        Serial.print("[temp] Read DHT22 failed, err=");
        Serial.print(SimpleDHTErrCode(dhtXError[1]));
        Serial.print(",");
        Serial.println(SimpleDHTErrDuration(dhtXError[1]));
    }

    delay(500);
    digitalWrite(LED_TEMP_READ, LOW);
    delay(500);
    digitalWrite(LED_TEMP_READ, HIGH);

    // Llegeix la temperatura del(s) DS18B20
    sensors.requestTemperatures();
    delay(2000);
    for (int i=0; i<sensors.getDeviceCount(); i++) {
        dsXTemperature[i] = sensors.getTempCByIndex(i);
    }

    // Serial.printf("[temp] sensors count = %d\n\r", sensors.getDeviceCount());
    // for (int i=0; i<sensors.getDeviceCount(); i++) {
    //     sensors.getAddress(&dsXAddress[i][0], i);
    //     Serial.print("[temp] dsTemp 0x");
    //     Serial.printf("%s", dsXAddressStr[i]);
    //     Serial.printf(": %f\n\r", dsXTemperature[i]);
    // }
    digitalWrite(LED_TEMP_READ, LOW);
}

bool temp_hasDHT11() {
    return dhtXError[0] == SimpleDHTErrSuccess;
}

float temp_getDHT11Temperature() {
    return dhtXTemperature[0];
}

float temp_getDHT11Humidity() {
    return dhtXHumidity[0];
}

bool temp_hasDHT22() {
    return dhtXError[1] == SimpleDHTErrSuccess;
}

float temp_getDHT22Temperature() {
    return dhtXTemperature[1];
}

float temp_getDHT22Humidity() {
    return dhtXHumidity[1];
}

int temp_getDS18B20Count() {
    return sensors.getDeviceCount();
}

bool temp_hasDS18B20(int index) {
    if (index < 0 || index >= sensors.getDeviceCount()) {
        return false;
    }
    return dsXTemperature[index] != DEVICE_DISCONNECTED_C;
}

float temp_getDS18B20Temperature(int index) {
    if (index < 0 || index >= sensors.getDeviceCount()) {
        return DEVICE_DISCONNECTED_C;
    }
    return dsXTemperature[index];
}

char *temp_getDS18B20Address(int index) {
    if (index < 0 || index >= sensors.getDeviceCount()) {
        return NULL;
    }
    return dsXAddressStr[index];
}
