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

extern SPIClass spi;

extern volatile int waveSelector;

extern TaskHandle_t dacTaskHandle;