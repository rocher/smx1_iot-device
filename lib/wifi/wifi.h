#ifndef WIFI_H
#define WIFI_H

extern char        SERIAL_NUMBER[];
extern const char* WIFI_SSID;
extern const char* WIFI_PASSWORD;
extern const int   CHECK_CONNECTION_PERIOD;
extern const int   WIFI_RECONNECT;
extern const int   WIFI_RETRIES;

extern const unsigned int LED_CONNECT;

void wifi_setup();
bool wifi_isConnected();
bool wifi_status();
void wifi_reconnect();

#endif // WIFI_H
