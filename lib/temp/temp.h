#ifndef TEMP_H
#define TEMP_H

extern const bool LOG_ERRORS;

extern const int PIN_DHT11;
extern const int PIN_DHT22;
extern const int ONE_WIRE_BUS;

extern const unsigned int LED_TEMP_READ;

void temp_setup();
void temp_read();

bool  temp_hasDHT11();
float temp_getDHT11Temperature();
float temp_getDHT11Humidity();

bool  temp_hasDHT22();
float temp_getDHT22Temperature();
float temp_getDHT22Humidity();

int   temp_getDS18B20Count();
bool  temp_hasDS18B20(int index);
float temp_getDS18B20Temperature(int index);
char *temp_getDS18B20Address(int index);

#endif // TEMP_H
