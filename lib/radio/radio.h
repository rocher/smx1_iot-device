#ifndef RADIO_H
#define RADIO_H

void radio_setup();

void radio_sleep();
void radio_wakeUp();
void radio_transmitMessage(char* message);
void radio_receiveMessage();

#endif // RADIO_H
