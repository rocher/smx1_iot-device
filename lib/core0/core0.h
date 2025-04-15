#ifndef CORE0_H
#define CORE0_H

extern const char* PROGRAM_NAME;
extern const char* PROGRAM_VERSION;
extern const bool  LOG_ERRORS;

extern const int REBOOTS_E_ADD;

extern const unsigned int SN_LENGTH;
extern char               SERIAL_NUMBER[];

extern const unsigned int LED_MQTT_IN;
extern const unsigned int LED_MQTT_OUT;
extern const unsigned int LED_TEMP_READ;
extern const unsigned int LED_CONNECT;

extern const bool LED_BLINK;
extern const int  LED_PERIOD;

extern const char* TOPIC_RET_TEMP_INT;
extern const char* TOPIC_RET_TEMP_EXT;
extern const char* TOPIC_RET_HUMIDITY;
extern const int   TOPIC_RET_QoS;

extern const int CHECK_CONNECTION_PERIOD;

void core0_setup();
void core0_loop();

#endif // CORE0_H
