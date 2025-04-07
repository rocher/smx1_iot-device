#ifndef BOOT_H
#define BOOT_H

extern const int REBOOTS_E_ADD;

void boot_setup();
void boot_reboot();
void boot_rebootToBootloader();

#endif // BOOT_H
