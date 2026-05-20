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
int amplitudes[] = {0,0,0,0};



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
void sendToSPIBus(int val1, int val2,int val3,int val4){
    digitalWrite(CS1, LOW);
    spi.transfer16(0x3000|((val1 * (multiplierChannelOne)) & 0x0FFF));   
    digitalWrite(CS1, HIGH);
    digitalWrite(CS1, LOW);
    spi.transfer16(0xB000|((val2 * (multiplierChannelTwo)) & 0x0FFF)); 
    digitalWrite(CS1, HIGH);
    digitalWrite(CS2, LOW);
    spi.transfer16(0x3000|((val3 * (multiplierChannelThree)) & 0x0FFF));
    digitalWrite(CS2, HIGH); 
    digitalWrite(CS2, LOW);  
    spi.transfer16(0xB000|((val4 * (multiplierChannelFour)) & 0x0FFF)); 
    digitalWrite(CS2, HIGH);
}


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
        if(waveSelector==0){ //Square
            amplitudes[0]=0;
            amplitudes[1]=0;
            amplitudes[2]=0;
            amplitudes[3]=0;
            if(indexChannelOne>128){amplitudes[0]=100;}
            if(indexChannelTwo>128){amplitudes[1]=100;}
            if(indexChannelThree>128){amplitudes[2]=100;}
            if(indexChannelFour>128){amplitudes[3]=100;}
            sendToSPIBus(amplitudes[0],amplitudes[1],amplitudes[2],amplitudes[3]);
        }
        else if(waveSelector==1){//Triangle
            sendToSPIBus(triangleTable[indexChannelOne],triangleTable[indexChannelTwo],triangleTable[indexChannelThree],triangleTable[indexChannelFour]);
        }
        else if(waveSelector==2){//Sine
            sendToSPIBus(sineTable[indexChannelOne],sineTable[indexChannelTwo],sineTable[indexChannelThree],sineTable[indexChannelFour]);
        }
        else if(waveSelector==3){//Sawtooth
            sendToSPIBus(indexChannelOne/3,indexChannelTwo/3,indexChannelThree/3,indexChannelFour/3);
        }
        else if(waveSelector==4){//Arbitary
            sendToSPIBus(arbTable[indexChannelOne],arbTable[indexChannelTwo],arbTable[indexChannelThree],arbTable[indexChannelFour]);
        }
        else if(waveSelector==5){//Pulse
            amplitudes[0]=0;
            amplitudes[1]=0;
            amplitudes[2]=0;
            amplitudes[3]=0;
            if(indexChannelOne>=pulsePot){amplitudes[0]=100;}
            if(indexChannelTwo>=pulsePot){amplitudes[1]=100;}
            if(indexChannelThree>=pulsePot){amplitudes[2]=100;}
            if(indexChannelFour>=pulsePot){amplitudes[3]=100;}
            sendToSPIBus(amplitudes[0],amplitudes[1],amplitudes[2],amplitudes[3]);
        }
        else{
            amplitudes[0]=0;
            amplitudes[1]=0;
            amplitudes[2]=0;
            amplitudes[3]=0;
            if(indexChannelOne>128){amplitudes[0]=128;}
            if(indexChannelTwo>128){amplitudes[1]=128;}
            if(indexChannelThree>128){amplitudes[2]=128;}
            if(indexChannelFour>128){amplitudes[3]=128;}
            sendToSPIBus(amplitudes[0],amplitudes[1],amplitudes[2],amplitudes[3]);
        }

        
    }
}










/*
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
*/