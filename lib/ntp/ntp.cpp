#include "ntp.h"

#include <WiFi.h>

void ntp_setup() {
    NTP.begin(NTP_SERVER, "pool.ntp.org");

    setenv("TZ", TZ_DST, 1);
    tzset();
}

void ntp_update() {
    NTP.waitSet(NTP_TIMEOUT);

    time_t now = time(nullptr);
    struct tm timeinfo;

    gmtime_r(&now, &timeinfo);

    if (now < 8 * 3600 * 2) {
        Serial.println("[ntp] Could not get ntp time");
        Serial.print("[ntp] Current time: ");
    }
    else {
        Serial.println("[ntp] Time synced");
        Serial.print("[ntp] Current ntp time: ");
    }
    Serial.printf("%s\r", asctime(&timeinfo));

    localtime_r(&now, &timeinfo);
    Serial.print("[ntp]       Local time: ");
    Serial.printf("%s\r", asctime(&timeinfo));
}
