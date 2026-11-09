#pragma once
#include <Arduino.h>
#include <SPI.h>

extern portMUX_TYPE mux; //Safely changes voltatile stuff

extern volatile uint32_t phaseIncrementChannelOne;
extern volatile uint32_t phaseIncrementChannelTwo;
extern volatile uint32_t phaseIncrementChannelThree;
extern volatile uint32_t phaseIncrementChannelFour;
extern volatile int multiplierChannelOne;
extern volatile int multiplierChannelTwo;
extern volatile int multiplierChannelThree;
extern volatile int multiplierChannelFour;
extern volatile int pulsePot;
extern SPIClass spi;

extern volatile int waveSelector;

extern TaskHandle_t dacTaskHandle;

extern const uint8_t sineTable[256];

extern const uint8_t triangleTable[256];

extern const uint8_t arbTable[256];

extern const uint8_t steppedSineTable[256];

extern const uint8_t rectifierTable[256];