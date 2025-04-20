#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>

extern char SERIAL_NUMBER[];

void setup_clear();
void setup_load();
void setup_save();

void setup_setName(String key, String value);
const char* setup_getName(char* key);
String setup_getName(String key);

#endif // SETUP_H
