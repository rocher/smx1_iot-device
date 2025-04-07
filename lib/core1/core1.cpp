#include "core1.h"
#include "temp.h"
#include "web.h"

#include <Arduino.h>

void core1_setup() {
    delay(5000);
    temp_setup();
    web_setup();
}

void core1_loop() {
    temp_read();
    web_handleClient();
    delay(1000);
}
