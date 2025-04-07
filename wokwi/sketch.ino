// Inclusió de llibreries externes i mòduls propis
#include "config.h"

#include <DallasTemperature.h>
#include <SimpleDHT.h>

// Declaració d'objectes globals
SimpleDHT22 dht22(PIN_DHT22);

OneWire oneWire(PIN_ONE_WIRE);
DallasTemperature sensorsDS18B20(oneWire);


void setup() {
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico W!");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_MQTT_IN, OUTPUT);
  pinMode(LED_MQTT_OUT, OUTPUT);
  pinMode(LED_TEMP_READ, OUTPUT);
  pinMode(LED_CONNECT, OUTPUT);
  pinMode(PIN_DHT22, INPUT);
}

void loop() {
  static int lbValue = HIGH;
  int dhtError = -1;
  float dhtTemperature, dhtHumidity;

  digitalWrite(LED_BUILTIN, lbValue);
  lbValue = (lbValue == HIGH ? LOW : HIGH);

  dhtError = dht22.read2(
               &dhtTemperature,
               &dhtHumidity,
               NULL);

  if (dhtError == SimpleDHT.) {
    Serial1.print("T = ");
    Serial1.print(dhtTemperature);
    Serial1.print(", H = ");
    Serial1.println(dhtHumidity);
  }

  delay(2000);
}
