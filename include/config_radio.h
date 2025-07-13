//  Define the LoRa board type.
//
//  Uncomment the following line to use Waveshare Pico-LoRa-SX1262 board If
//  you are using a different board, comment this line out and define the
//  correct board For example, if you are using a Feather RP2040 with RFM95,
//  you would define RADIO_BOARD_FEATHER_RP2040 instead. If you are using a
//  different board, you can define it in RadioBoards.h or RadioBoards.cpp, or
//  you can define it in your own code.
//
// Used in conjunction with RadioBoards library, with modifications from
// https://github.com/radiolib-org/RadioBoards/blob/main/src/contributed/Waveshare/RP2040_LoRa.h
//

// Comment this line to disable the usage of the LoRa radio
#define USE_LORA_RADIO

// Define the LoRa board type
#ifdef USE_LORA_RADIO
#define WAVESHARE_PICO_LORA_SX1262_433
#endif // USE_LORA_RADIO
