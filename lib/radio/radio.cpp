#include <Arduino.h>
#include <RadioLib.h>

// Global variables

// radio module object, using Waveshare Pico-LoRa-SX1262 pinout
SX1262 radio = new Module(3, 20, 15, 2, SPI1, RADIOLIB_DEFAULT_SPI_SETTINGS);

// flag to indicate that a packet was sent or received
volatile bool operationDone = true;

// save transmission states between loops
int transmissionState = RADIOLIB_ERR_NONE;

// flag to indicate transmission or reception state
bool transmitFlag = false;

// flag to indicate whether the radio is currently playing ping-pong
bool pingPongMode = false;

// flag to indicate whether the radio is currently in sleep mode
bool sleepMode = false;

// this function is called when a complete packet
// is transmitted or received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void radio_setFlag(void) {
    // we sent or received a packet, set the flag
    operationDone = true;
}

bool radio_isAwake() {
    return !sleepMode;
}

void radio_setup() {

    // Radio setup according to Waveshare Pico-LoRa-SX1262:
    // https://github.com/BNNorman/Waveshare-Pico-LoRa-SX1262-868M

    // initialize SPI1
    Serial.println(F("[radio] Initializing SPI1 ... "));
    SPI1.setSCK(10);
    SPI1.setTX(11);
    SPI1.setRX(12);
    pinMode(3, OUTPUT);
    digitalWrite(3, HIGH);
    SPI1.begin(false);
    Serial.println(F("[radio] Initializing SPI1 ... done"));

    // initialize SX1262 with default settings
    Serial.println(F("[radio] Initializing ... "));
    // Waveshare Pico-LoRa-SX1262 has the following connections:
    // NSS pin:   3
    // DIO1 pin: 20
    // NRST pin: 15
    // BUSY pin:  2
    // radio     = new Module(3, 20, 15, 2, SPI1, RADIOLIB_DEFAULT_SPI_SETTINGS);
    int state = radio.begin();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("[radio] Initializing ... done"));
    } else {
        Serial.print(F("[radio] Initializing ... failed with code "));
        Serial.println(state);
    }

    // set the function that will be called
    // when new packet is received
    radio.setDio1Action(radio_setFlag);
}

void radio_sleep() {
    if (sleepMode) {
        Serial.println(F("[radio] Already in sleep mode"));
        return;
    }

    Serial.println(F("[radio] Going to sleep ... "));
    int state = radio.sleep(/* retainConfig = */ true);
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("[radio] Going to sleep ... done"));
        sleepMode = true;
    } else {
        Serial.print(F("[radio] Going to sleep ... failed with code "));
        Serial.println(state);
    }
}

void radio_wakeUp() {
    if (!sleepMode) {
        Serial.println(F("[radio] Already awake"));
        return;
    }

    Serial.println(F("[radio] Waking up ... "));
    int state = radio.standby();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("[radio] Waking up ... done"));
        sleepMode = false;
    } else {
        Serial.print(F("[radio] Waking up ... failed with code "));
        Serial.println(state);
    }
}

void radio_transmitMessage(char* message) {
    if (sleepMode) {
        Serial.println(F("[radio] Cannot transmit while in sleep mode"));
        return;
    }

    if (!operationDone) {
        Serial.println(F("[radio] Cannot transmit while previous operation is not done"));
        return;
    }
    operationDone = false; // reset the flag

    Serial.print(F("[radio] Transmitting: "));
    Serial.println(message);
    transmissionState = radio.transmit(message);
    if (transmissionState == RADIOLIB_ERR_NONE) {
        Serial.println(F("[radio] Transmission successful"));
    } else {
        Serial.print(F("[radio] Transmission failed with code "));
        Serial.println(transmissionState);
    }
}

void radio_receiveMessage() {
    if (sleepMode) {
        // Serial.println(F("[radio] Cannot receive while in sleep mode"));
        return;
    }

    if (!operationDone) {
        Serial.println(F("[radio] Cannot receive messages while previous operation is not done"));
        return;
    }
    operationDone = false; // reset the flag

    Serial.println(F("[radio] Receiving ... "));
    transmissionState = radio.startReceive();
    if (transmissionState == RADIOLIB_ERR_NONE) {
        Serial.println(F("[radio] Reception successful"));
        String msg;
        radio.readData(msg);
        Serial.print("[radio] Message: ");
        Serial.println(msg);
    } else {
        Serial.print(F("[radio] Reception failed with code "));
        Serial.println(transmissionState);
    }
}
