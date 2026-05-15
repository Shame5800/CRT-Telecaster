#include "global.h"
#include <Arduino.h>
#include "def.h"

volatile uint32_t phaseChannelOne = 0;
volatile uint32_t phaseChannelTwo = 0;
volatile uint32_t phaseChannelThree = 0;
volatile uint32_t phaseChannelFour = 0;
volatile uint8_t indexChannelOne = 0;
volatile uint8_t indexChannelTwo = 0;
volatile uint8_t indexChannelThree = 0;
volatile uint8_t indexChannelFour = 0;



void IRAM_ATTR onTimer() {
    BaseType_t woken = pdFALSE;
    vTaskNotifyGiveFromISR(dacTaskHandle, &woken);
    portYIELD_FROM_ISR(woken);
}
const uint8_t squareTable[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
};
void dacTask(void *pv) {
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // sleeps until ISR fires
        
        
        phaseChannelOne += phaseIncrementChannelOne;
        phaseChannelTwo += phaseIncrementChannelTwo;
        phaseChannelThree += phaseIncrementChannelThree;
        phaseChannelFour += phaseIncrementChannelFour;
        indexChannelOne = (phaseChannelOne >> 24) & 0xFF;
        indexChannelTwo = (phaseChannelTwo >> 24) & 0xFF;
        indexChannelThree = (phaseChannelThree >> 24) & 0xFF;
        indexChannelFour = (phaseChannelFour >> 24) & 0xFF;
        //0x3000 A
        //0xB000 B
        digitalWrite(CS1, LOW);
        spi.transfer16(0x3000|((squareTable[indexChannelOne] * (multiplierChannelOne)) & 0x0FFF));   
        digitalWrite(CS1, HIGH);
        digitalWrite(CS1, LOW);
        spi.transfer16(0xB000|((squareTable[indexChannelTwo] * (multiplierChannelTwo)) & 0x0FFF)); 
        digitalWrite(CS1, HIGH);
        digitalWrite(CS2, LOW);
        spi.transfer16(0x3000|((squareTable[indexChannelThree] * (multiplierChannelThree)) & 0x0FFF));
        digitalWrite(CS2, HIGH); 
        digitalWrite(CS2, LOW);  
        spi.transfer16(0xB000|((squareTable[indexChannelFour] * (multiplierChannelFour)) & 0x0FFF)); 
        digitalWrite(CS2, HIGH);
    }
}