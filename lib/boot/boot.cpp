#include "boot.h"

#include <Arduino.h>
#include <EEPROM.h>

unsigned long reboots = 0;

void boot_setup() {
    EEPROM.begin(256);
    EEPROM.get(REBOOTS_E_ADD, reboots);
    Serial.print("[boot] reboots: ");
    Serial.println(reboots);
}

void boot_add() {
    // Guarda el valor de reboots a la EEPROM
    reboots++;
    EEPROM.put(REBOOTS_E_ADD, reboots);
    if (EEPROM.commit()) {
        Serial.print("[boot] EEPROM successfully committed, reboots: ");
        Serial.println(reboots);
    } else {
        Serial.println("[boot] EEPROM commit failed");
    }
}

void boot_reboot() {
    boot_add();
    Serial.println("[boot] Rebooting...");
    delay(1000);
    rp2040.reboot();
}

void boot_rebootToBootloader() {
    boot_add();
    Serial.println("[boot] Rebooting to bootloader...");
    delay(1000);
    rp2040.rebootToBootloader();
}
