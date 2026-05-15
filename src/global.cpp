#include "global.h"

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
volatile uint32_t phaseIncrementChannelOne = 0;
volatile uint32_t phaseIncrementChannelTwo = 0;
volatile uint32_t phaseIncrementChannelThree = 0;
volatile uint32_t phaseIncrementChannelFour = 0;
volatile int multiplierChannelOne = 0;
volatile int multiplierChannelTwo = 0;
volatile int multiplierChannelThree = 0;
volatile int multiplierChannelFour = 0;

SPIClass spi(VSPI);

volatile int waveSelector=0;

TaskHandle_t dacTaskHandle = NULL;