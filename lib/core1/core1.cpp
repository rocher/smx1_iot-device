#include "core1.h"
#include "radio.h"
#include "temp.h"
#include "web.h"

#include <Arduino.h>

void core1_setup() {
    delay(5000);
    temp_setup();
    web_setup();

    radio_setup();
    // radio_sleep();
}

void core1_loop() {
    temp_read();
    web_handleClient();

    // radio_transmitMessage("Hello from IoT device!");
    radio_receiveMessage();

    delay(1000);
}
