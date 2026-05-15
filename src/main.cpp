#include <Arduino.h>
#include "def.h"
#include "esp_wifi.h"
#include "esp_bt.h"
#include "global.h"
#include "wavegen.h"
#include "notes.h"


uint32_t tempPhaseIncrementChannelOne=0;
uint32_t tempPhaseIncrementChannelTwo=0;
uint32_t tempPhaseIncrementChannelThree=0;
uint32_t tempPhaseIncrementChannelFour=0;
int tempMultiplierChannelOne=0;
int tempMultiplierChannelTwo=0;
int tempMultiplierChannelThree=0;
int tempMultiplierChannelFour=0;
int tempWaveSelector=0;

hw_timer_t *timer = NULL;




void setFrequencies(float freq1,float freq2,float freq3,float freq4) {
  // phaseIncrement = (freq * 2^32) / SAMPLE_RATE
  tempPhaseIncrementChannelOne = (uint32_t)((freq1 * 4294967296.0) / 20000);
  tempPhaseIncrementChannelTwo = (uint32_t)((freq2 * 4294967296.0) / 20000);
  tempPhaseIncrementChannelThree = (uint32_t)((freq3 * 4294967296.0) / 20000);
  tempPhaseIncrementChannelFour = (uint32_t)((freq4 * 4294967296.0) / 20000);
}
void setMultiplier(int input1,int input2,int input3,int input4){
  tempMultiplierChannelOne=input1;
  tempMultiplierChannelTwo=input2;
  tempMultiplierChannelThree=input3;
  tempMultiplierChannelFour=input4;
}
void sendToWaveGenerator(){
  portENTER_CRITICAL(&mux);
  phaseIncrementChannelOne = tempPhaseIncrementChannelOne;
  phaseIncrementChannelTwo = tempPhaseIncrementChannelTwo;
  phaseIncrementChannelThree = tempPhaseIncrementChannelThree;
  phaseIncrementChannelFour = tempPhaseIncrementChannelFour;
  multiplierChannelOne = tempMultiplierChannelOne;
  multiplierChannelTwo = tempMultiplierChannelTwo;
  multiplierChannelThree = tempMultiplierChannelThree;
  multiplierChannelFour = tempMultiplierChannelFour;
  waveSelector = tempWaveSelector;
  portEXIT_CRITICAL(&mux);
}
void playTuningChorale(){
  //COPYRIGHT//
  //PROPERTY OF THE LANGLEY HIGH SCHOOL BAND PROGRAM
  //AUTHORED BY DOUG MARTIN AND KAI HASHIMOTO SMITH
  //USAGE OF THIS CHORALE OUTSIDE OF THIS ORGANIZATION WILL LEAD TO IMMEDIATE EXECUTION
  setMultiplier(0,0,0,0);
  sendToWaveGenerator();
  delay(5000);
  setMultiplier(16,16,16,0);
  setFrequencies(A2,A3,A4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); (50); setMultiplier(16,16,16,0);
  setFrequencies(G2,G3,G4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); (50); setMultiplier(16,16,16,0);
  setFrequencies(G2,G3,G4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); (50); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); (50); setMultiplier(16,16,16,0);
  setFrequencies(E2,E3,E4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); (50); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(2000);
  setMultiplier(0,0,0,0);  sendToWaveGenerator(); delay(1000); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); (50); setMultiplier(16,16,16,0);
  setFrequencies(G2,G3,G4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(G2,G3,G4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); (50); setMultiplier(16,16,16,0);
  setFrequencies(A2,A3,A4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); (50); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); (50); setMultiplier(16,16,16,0);
  setFrequencies(Bb2,Bb3,Bb4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(Bb2,Bb3,Bb4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); (50); setMultiplier(16,16,16,0);
  setFrequencies(A2,A3,A4,B5); sendToWaveGenerator(); delay(2000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); (50); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(4000);
  setMultiplier(0,0,0,0); sendToWaveGenerator();
}


void setup(){
  Serial.begin(9600);
  esp_wifi_stop();
  esp_bt_controller_disable();

  pinMode(CS1,OUTPUT);
  pinMode(CS2,OUTPUT);
 
  pinMode(S1,INPUT);
  pinMode(S2,INPUT);
  pinMode(S3,INPUT);
  pinMode(S4,INPUT);

  pinMode(WARBLEPOT,INPUT);
  pinMode(PULSEPOT,INPUT);
  pinMode(INTENSITYPOT,INPUT);
  pinMode(SHIFTPOT,INPUT);

  pinMode(GLISSANDOPIN,INPUT_PULLUP);
  pinMode(WARBLEPIN,INPUT_PULLUP);

  pinMode(WAVEDIP1PIN,INPUT_PULLUP);
  pinMode(WAVEDIP2PIN,INPUT_PULLUP);
  pinMode(WAVEDIP3PIN,INPUT_PULLUP);
  pinMode(WAVEDIP4PIN,INPUT_PULLUP);

  pinMode(MODEDIP1PIN,INPUT_PULLUP);
  pinMode(MODEDIP2PIN,INPUT_PULLUP);
  pinMode(MODEDIP3PIN,INPUT_PULLUP);

  spi.begin(SCK_PIN, -1, MOSI_PIN, -1);
  spi.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));

  digitalWrite(CS1, LOW);
  spi.transfer16(0x3000);   
  spi.transfer16(0xB000); 
  digitalWrite(CS1, HIGH);
  digitalWrite(CS2, LOW);
  spi.transfer16(0x3000);   
  spi.transfer16(0xB000); 
  digitalWrite(CS2, HIGH);

  //May not workVVV

  xTaskCreatePinnedToCore(
    dacTask,        // function
    "DAC",          // name
    4096,           // stack size
    NULL,           // parameters
    2,              // priority
    &dacTaskHandle, // handle
    0               // core 0
  );
  
  
  tempMultiplierChannelOne=16;
  tempMultiplierChannelTwo=16;
  tempMultiplierChannelThree=16;
  tempMultiplierChannelFour=0;

  //Gflat Dflat and A

 
  //setFrequencies(Gb4,A4,Db5,Db2);
  //sendToWaveGenerator();


 

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 50, true);
  timerAlarmEnable(timer);
  delay(2000);

  playTuningChorale();

}

void loop(){


  delay(10000); // just chillax


  playTuningChorale();

}
















/*
SPIClass spi(VSPI);

const uint16_t HIGH_VAL = 4094;
const uint16_t LOW_VAL  = 0;
const int delay_us = 1050;

// Active output command — works for both A and B
uint16_t cmdA(uint16_t val) { return 0x3000 | (val & 0x0FFF); }  // Ch A, active
uint16_t cmdB(uint16_t val) { return 0xB000 | (val & 0x0FFF); }  // Ch B, active

int processInput(int input){
  input = input-1020;
  int multiple = input/200;
  if (input-multiple*200>100){
    multiple=multiple+1;
  }
  return multiple;
}//end process

void writeDAC(uint8_t csPin, uint16_t data) {
  digitalWrite(csPin, LOW);
  spi.transfer16(data);
  digitalWrite(csPin, HIGH);
}

void setup() {
  Serial.begin(9600);
  pinMode(19,INPUT_PULLUP);
  pinMode(18,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(17,INPUT_PULLUP);
  pinMode(16,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  
  pinMode(12,INPUT);
  pinMode(14,INPUT);
  pinMode(27,INPUT);
  pinMode(26,INPUT);

  pinMode(CS1, OUTPUT); digitalWrite(CS1, HIGH);
  pinMode(CS2, OUTPUT); digitalWrite(CS2, HIGH);

  spi.begin(SCK_PIN, -1, MOSI_PIN, -1);
  spi.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));

  // Drive all unused channels to 0V (active, not shutdown)
  // Floating inputs into the MCP6002 buffers will rail/oscillate otherwise
  writeDAC(CS1, cmdB(0));  // Chip 1 DAC B → 0V
  writeDAC(CS2, cmdA(0));  // Chip 2 DAC A → 0V
  writeDAC(CS2, cmdB(0));  // Chip 2 DAC B → 0V
  writeDAC(CS1, cmdA(0));
}
int reading = 0;
void loop() {

  /*
  writeDAC(CS1, cmdA(HIGH_VAL));
  //writeDAC(CS1, cmdB(HIGH_VAL));
  delayMicroseconds(delay_us);

  writeDAC(CS1, cmdA(LOW_VAL));
  //writeDAC(CS1, cmdB(LOW_VAL));
  delayMicroseconds(delay_us);
  
  reading = analogRead(12);
  Serial.print("ADC - ");
  Serial.print(reading);
  Serial.print(" Fret - ");
  Serial.println(processInput(reading));
  delay(10);

}
*/


