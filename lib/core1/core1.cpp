#include "core1.h"
#include "temp.h"
#include "web.h"

#include "../../include/config_radio.h"
#ifdef USE_LORA_RADIO
#include "radio.h"
#endif

#include <Arduino.h>

void core1_setup() {
    delay(5000);
    temp_setup();
    web_setup();

#ifdef USE_LORA_RADIO
    radio_setup();
    radio_sleep();
#endif
}

void core1_loop() {
    temp_read();
    web_handleClient();

#ifdef USE_LORA_RADIO
    if (radio_isAwake()) {
        radio_transmitMessage("TEST Hello from IoT device!");
        radio_receiveMessage();
    }
#endif

    delay(1000);
}
