#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>

extern const unsigned int SN_LENGTH;
extern char SERIAL_NUMBER[];

extern const unsigned int LED_MQTT_IN;
extern const unsigned int LED_MQTT_OUT;

extern const char *MQTT_BROKER;
extern const int   MQTT_PORT;
extern const char *MQTT_USER;
extern const char *MQTT_PASSWORD;

extern const char *TOPIC_CMD;
extern const int   TOPIC_CMD_QoS;

extern const char *TOPIC_STATUS;
extern const int   TOPIC_STATUS_QoS;

extern const char  *WILL_TOPIC;
extern const String WILL_PAYLOAD_FMT;
extern const bool   WILL_RETAIN;
extern const int    WILL_QoS;

extern const char *CMD_TEMP_INT;
extern const char *CMD_TEMP_EXT;
extern const char *CMD_HUMIDITY;
extern const char *CMD_REBOOT;
extern const char *CMD_REBOOT_LOAD;

void mqtt_setup();
bool mqtt_isConnected();
void mqtt_reconnect();

void mqtt_poll();
void mqtt_subscribe(const char *topic, int qos = 0);

void mqtt_startReturnTopic();
void mqtt_endReturnTopic();

bool mqtt_receivedInternalTemperature();
bool mqtt_receivedExternalTemperature();
bool mqtt_receivedHumidity();

void mqtt_publish(const char *topic,
                  const char *message,
                  int         qos = 0,
                  bool        retained = false);

unsigned int mqtt_sentMessages();
unsigned int mqtt_receivedMessages();

#endif // MQTT_H
