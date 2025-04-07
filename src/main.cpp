#include "core0.h"
#include "core1.h"
#include "mqtt.h"
#include "ntp.h"
#include "temp.h"
#include "web.h"
#include "wifi.h"

#include "config.h"

void setup() {
    core0_setup();
}

void loop() {
    core0_loop();
}

void setup1() {
    delay(5000);
    core1_setup();
}

void loop1() {
    core1_loop();
}
