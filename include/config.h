#ifndef CONFIG_H
#define CONFIG_H

#include "secrets.h"

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#ifndef _VERSION_
#include "version.h"
#endif // _VERSION_

#include <pico/unique_id.h>
#include <Arduino.h>

const char TZ_DST[] = "UTC-2"; // Timezone for Barcelona

// Versió del programa
const char *PROGRAM_NAME    = "SMX1_FIN/IoT";
const char *PROGRAM_VERSION = STR(_VERSION_); // defined in version.h
const bool  LOG_ERRORS      = false;

// Informació de la placa pico-w
const unsigned int SN_LENGTH = 2*PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1;
char SERIAL_NUMBER[SN_LENGTH];

// Reboots
const int REBOOTS_E_ADD = 0x1C;

// Configuració d'NTP
const char *NTP_SERVER  = "es.pool.ntp.org";
const int   NTP_TIMEOUT = 10000;

// Configuració de MQTT
// const char *MQTT_BROKER   = "smx204.local";
const char *MQTT_BROKER   = "smx1.local";
// const char *MQTT_BROKER   = "192.168.18.11";
const int   MQTT_PORT     = 1883;
const char *MQTT_USER     = SECRET_MQTT_USER;
const char *MQTT_PASSWORD = SECRET_MQTT_PASSWORD;

// Configuració de la Wi-Fi
const char *WIFI_SSID      = SECRET_WIFI_SSID;
const char *WIFI_PASSWORD  = SECRET_WIFI_PASSWORD;

// Check WiFi & MQTT connection every CHECK_CONNECTION_PERIOD milliseconds
const int   CHECK_CONNECTION_PERIOD    = 15 * 1000UL;

// Reboot after WIFI_RECONNECT failed reconnection attemps
const int   WIFI_RECONNECT = 5;

// At each reconnection attempt, retry WIFI_RETRIES times
const int   WIFI_RETRIES   = 25;

// LED de la placa pico-w
const bool LED_BLINK  = true;
const int  LED_PERIOD = 2 * 1000UL;

// Altres LEDs
const unsigned int LED_MQTT_IN   =  9; // Green
const unsigned int LED_MQTT_OUT  = 10; // Blue
const unsigned int LED_TEMP_READ = 11; // Yellow
const unsigned int LED_CONNECT   = 12; // Red

// Sensors de temperatura
const int PIN_DHT22    = 28;
const int PIN_DHT11    = 27;
const int ONE_WIRE_BUS =  8; // DS18B20

// Topics
const char *TOPIC_CMD          = "/cmd";
const int   TOPIC_CMD_QoS      = 0;

const char *TOPIC_STATUS       = "/status";
const int   TOPIC_STATUS_QoS   = 0;

const char *TOPIC_RET_TEMP_INT = "/ret/temp/int";
const char *TOPIC_RET_TEMP_EXT = "/ret/temp/ext";
const char *TOPIC_RET_HUMIDITY = "/ret/humidity";
const int   TOPIC_RET_QoS      = 2;

// Will topic
const char  *WILL_TOPIC       = "/will";
const String WILL_PAYLOAD_FMT = "{ \"pid\": \"%s\", \"msg\": \"Lost Mqtt Connection\" }";
const bool   WILL_RETAIN      = true;
const int    WILL_QoS         = 1;

// Commands
const char *CMD_TEMP_INT    = "temp_int";
const char *CMD_TEMP_EXT    = "temp_ext";
const char *CMD_HUMIDITY    = "humidity";
const char *CMD_REBOOT      = "reboot";
const char *CMD_REBOOT_LOAD = "reboot_to_loader";

#endif // CONFIG_H
