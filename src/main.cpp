#include <Arduino.h>
#include "def.h"
#include "esp_wifi.h"
#include "esp_bt.h"
#include "global.h"
#include "wavegen.h"
#include "notes.h"
//================================================================================
//     ______           __                   _
//    /_  __/______  __/ /____  ______ ___  (_)___  __  __
//     / / / ___/ / / / //_/ / / / __ `__ \/ /_  / / / / /
//    / / (__  ) /_/ / ,< / /_/ / / / / / / / / /_/ /_/ /
//   /_/ /____/\__,_/_/|_|\__,_/_/ /_/ /_/_/ /___/\__,_/
//
//                          ______     __                     __
//                         /_  __/__  / /__  _________ ______/ /____  _____
//                          / / / _ \/ / _ \/ ___/ __ `/ ___/ __/ _ \/ ___/
//                         / / /  __/ /  __/ /__/ /_/ /__  / /_/  __/ /
//                        /_/  \___/_/\___/\___/\__,_/____/\__/\___/_/
//
//================================================================================
// 
//Tsukumizu Progamable 4 string hybrid bass guitar
//
//    Designed by Shane Sebastian Pomajambo
//
//    Originial idea by ELECTRONICOS FANTASTICOS
//
//    Special Thanks to Elias Charron, Shane Wyman, Professor Arthur Ball, and the AMP Lab at Virginia Tech
//
// Ver 2.1
// - We're in VSCode Now!
// - Created V2 PCB
// - This code is designed for the V2 PCB, and is much diffrent
// - 4 channel DACs based on SPI
// - More potential wave options plus mode selector
// - combined interupts logic with dual core architecture of ESP32
//
//
//
//
//
//Tuning
int manualModifiers[] = {0,0,0,0};
//Set true for chorus, set false for octaver
bool chorus = false;
//
//
//Note Array
int notes[] = {
  Bb0,B0note,C1,Db1,D1,Eb1, E1, F1, Gb1, G1, Ab1, A1, Bb1, B1note,
  C2, Db2, D2, Eb2, E2, F2, Gb2, G2, Ab2, A2, Bb2, B2,
  C3, Db3, D3, Eb3, E3, F3, Gb3, G3, Ab3, A3, Bb3, B3,
  C4, Db4, D4, Eb4, E4, F4, Gb4, G4, Ab4, A4, Bb4, B4,
  C5, Db5, D5, Eb5, E5, F5, Gb5, G5, Ab5, A5, Bb5, B5,
  C6, Db6, D6, Eb6, E6, F6
};

//sendToWavegen() Variables
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

//Potentiometer and user input
bool warble = false;
bool glissando = false;
int warblePot = 0;
int intensityPot=0;
int shiftPot=0;
int tempPulsePot = 0;
int modeSelector=0;

//Warble mode
int warbleDivisor;
int warbleCounter=0;
bool upwardsWarble=false;

//Fretboard input processing
int lastPlayedFret[] = {-1,-1,-1,-1};
int requestedNote[] = {0,0,0,0};
int requestedFret[] = {-1,-1,-1,-1};
int numNotesPlaying = 0;
int totalMultiplier=0;
int noteBases[] = {6,6,6,6};

//Methods ---------------------------------------------------------------
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
  pulsePot = tempPulsePot;
  portEXIT_CRITICAL(&mux);
}

int processInput(int input) {
  input = input - 1020;
  int multiple = input / 200;
  if (input - multiple * 200 > 100) {
    multiple = multiple + 1;
  }
  if(multiple<0){
    return -1;
  }
  numNotesPlaying++;
  return multiple;
}  //end process

void playTuningChorale(){
  
  //In Honor of Mr Martin.
    //
    // I truly wouldn't be the person I am today without this man, and I still can't belive that hes just gone now.
    // He was someone I really looked up to, both over his jokes and mannerisim that made rehersal fun, and his pure commitment to the band program. 
    // He pushed us to be the best we could be, he gave so much to the band, and you could tell that enjoyed every single second of it.
    //
    // I still remember from of his philisophies too, well at the very least I called them that. I think in my junior of sophmore year at the
    // end of the marching band season, he gathered everyone together and made a anouncement/speech. He said that he never aimed to win comepitions
    // If he wanted to win comeptitions, he would of invested in adding props or out of the box themes that could really wow the judges, but it would
    // come of the cost of students having to man or use them, and taking away the time they have to actually play their insturment. Instead of
    // being dead focused on prestige or "winning", he wanted to give each student the chance to do what they were there for, and that was to play music
    // He made it harder for us to win so we could walk away being proud in the work we put in and what we acomplished, rather than being the guy
    // who moves the letter H accross the field. He truly cared about each and every one of us. I didnt fully see that in high school, but I do now.
    //
    // The experience that I had with the langley high school bands would of never been the same without him. He garnnered a amazing community of 
    // people, he gave us memories that I still remember today, and pushed us to be better and improve without us even realizing it.
    //
    // I didnt have a chance to say goodbyte to him. At the end of year bandquet I tried to say goodbye, but he seemed stressed with his recent complications
    // so I decided to wait for a better time in class or after school. That time never came, and I graduated. I told myself I would comeback
    // next year for the spring or winter concerts. People allways comeback to visit. So I did. He wasn't there. and he died 4 days later. From Cancer.
    // My last words to him was probably a rushed get well soon message I signed when emma asked me to sign a booklet durring the spring concert.
    // It had my name purposely misspelled as Shane Pombajambo. I allways assumed he would turn out fine. He talked about having a tumor on his head
    // durring macrhing band and how the chemotherapy worked to remove it. When I heard he had cancer again I assumed the same, but it wasn't
    // I never had a chance to tell him how eternally greatful I am for his mere fucking existence. Just a get well soon message. And now he 
    // dosent exist anymore. Because of Cancer.
    //
    // I will miss you. You truly were the best.
    //
    // The following is 3 octaves worth of the bass section tuning chorale used by the langley high school band program
    // It plays before every preformance, at the start of every preformance, and after every tuning break.
    // It reminds me of him. It reminds me of my time in Band, It reminds me of everything he has done to make that time in band unforgettable
    // It has been ingrained in my memory, and It will play in his honor whenever all 3 DIP Mode selectors are switched on durring Startup.
    //
    // Thank you for everything you have done. I dont have to words to say how greatful I am for you.
    // I hope you have infinite aeropress coffee in heaven. 
    //

  setMultiplier(0,0,0,0);
  sendToWaveGenerator();
  delay(5000);
  setMultiplier(16,16,16,0);
  setFrequencies(A2,A3,A4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(G2,G3,G4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(G2,G3,G4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(E2,E3,E4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(2000);
  setMultiplier(0,0,0,0);  sendToWaveGenerator(); delay(1000); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(G2,G3,G4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(G2,G3,G4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(A2,A3,A4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(Bb2,Bb3,Bb4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(Bb2,Bb3,Bb4,B5); sendToWaveGenerator(); delay(1000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(A2,A3,A4,B5); sendToWaveGenerator(); delay(2000);
  setMultiplier(0,0,0,0); sendToWaveGenerator(); delay(50); setMultiplier(16,16,16,0);
  setFrequencies(F2,F3,F4,B5); sendToWaveGenerator(); delay(4000);
  setMultiplier(0,0,0,0); sendToWaveGenerator();
}

void sendNotes(){
  if(requestedNote[0]!=0){tempMultiplierChannelOne=totalMultiplier;} else{tempMultiplierChannelOne=0;}
  if(requestedNote[1]!=0){tempMultiplierChannelTwo=totalMultiplier;} else{tempMultiplierChannelTwo=0;}
  if(requestedNote[2]!=0){tempMultiplierChannelThree=totalMultiplier;} else{tempMultiplierChannelThree=0;}
  if(requestedNote[3]!=0){tempMultiplierChannelFour=totalMultiplier;} else{tempMultiplierChannelFour=0;}
  setFrequencies(requestedNote[0],requestedNote[1],requestedNote[2],requestedNote[3]);
  sendToWaveGenerator();
  lastPlayedFret[0]=requestedFret[0];
  lastPlayedFret[1]=requestedFret[1];
  lastPlayedFret[2]=requestedFret[2];
  lastPlayedFret[3]=requestedFret[3];
}

//Startup
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

  modeSelector=0;
  if(digitalRead(MODEDIP1PIN)==LOW){modeSelector+=1;}
  if(digitalRead(MODEDIP2PIN)==LOW){modeSelector+=2;}
  if(digitalRead(MODEDIP3PIN)==LOW){modeSelector+=4;}
  Serial.println("Startup Msg");

  //May not workVVV   //It works

  xTaskCreatePinnedToCore(
    dacTask,        // function
    "DAC",          // name
    4096,           // stack size
    NULL,           // parameters
    2,              // priority
    &dacTaskHandle, // handle
    0               // core 0
  );
  


  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 50, true);
  timerAlarmEnable(timer);

  modeSelector=0;
  if(digitalRead(MODEDIP1PIN)==LOW){modeSelector+=1;}
  if(digitalRead(MODEDIP2PIN)==LOW){modeSelector+=2;}
  if(digitalRead(MODEDIP3PIN)==LOW){modeSelector+=4;}

  if(modeSelector==7){
    playTuningChorale();
  }


}//end setup


//Application Loop
void loop(){

  //Fret Debugging
  // int reading = analogRead(S1);
  // Serial.println(reading);
  // Serial.println(processInput(reading));
  // delay(100);

  
  delay(25);
  
  if(glissando){
    delay(75+(intensityPot));
  }


  //Take UI measurements
  if(digitalRead(GLISSANDOPIN)==LOW){glissando=true;}
  else{glissando=false;}
  if(digitalRead(WARBLEPIN)==LOW){warble=true;}
  else{warble=false;}

  modeSelector=0;
  if(digitalRead(MODEDIP1PIN)==LOW){modeSelector+=1;}
  if(digitalRead(MODEDIP2PIN)==LOW){modeSelector+=2;}
  if(digitalRead(MODEDIP3PIN)==LOW){modeSelector+=4;}

  tempWaveSelector=0;
  if(digitalRead(WAVEDIP1PIN)==LOW){tempWaveSelector+=1;}
  if(digitalRead(WAVEDIP2PIN)==LOW){tempWaveSelector+=2;}
  if(digitalRead(WAVEDIP3PIN)==LOW){tempWaveSelector+=4;}
  if(digitalRead(WAVEDIP4PIN)==LOW){tempWaveSelector+=8;}
  
  //Base Mode Logic
  //4 string bass
  noteBases[0]=6;
  noteBases[1]=6;
  noteBases[2]=6;
  noteBases[3]=6;
  if(modeSelector==1){ //guitar lower
    noteBases[0]=18;
    noteBases[1]=18;
    noteBases[2]=18;
    noteBases[3]=18;
  }
  if(modeSelector==2){ //guitar middle
    noteBases[0]=23;
    noteBases[1]=23;
    noteBases[2]=23;
    noteBases[3]=22;
  }
  if(modeSelector==3){ //guitar higher
    noteBases[0]=28;
    noteBases[1]=28;
    noteBases[2]=27;
    noteBases[3]=27;
  }
  if(modeSelector==4){ //5 string bass lower, upper is 4 string bass
    noteBases[0]=1;
    noteBases[1]=1;
    noteBases[2]=1;
    noteBases[3]=1;
  }
  if(modeSelector==5){ //Overdrive Guitar
   noteBases[0]=38;
    noteBases[1]=38;
    noteBases[2]=37;
    noteBases[3]=37;

  }
  if(modeSelector==6){ //overdrive bass
    noteBases[0]=16;
    noteBases[1]=16;
    noteBases[2]=16;
    noteBases[3]=16;
  }
  if(modeSelector==7){
    //TBD 
    //preset modes for autoplay eventually
  }
  
  //Process Potentiometers
  tempPulsePot=analogRead(PULSEPOT);
  tempPulsePot=10+(tempPulsePot/19);

  intensityPot = analogRead(INTENSITYPOT)/41;

  shiftPot = analogRead(SHIFTPOT);

  warblePot = analogRead(WARBLEPOT);
  warbleDivisor = 60-(warblePot/136);

  //Take fret readings
  numNotesPlaying=0;
  requestedFret[0] = processInput(analogRead(S1));
  requestedFret[1] = processInput(analogRead(S2));
  requestedFret[2] = processInput(analogRead(S3));
  requestedFret[3] = processInput(analogRead(S4));

  //Process frets into notes. -1 means not pressed for fret. note of = 0 means silence 
  requestedNote[0] = 0; requestedNote[1] = 0; requestedNote[2] = 0; requestedNote[3] = 0;
  if(requestedFret[0]!=-1){requestedNote[0] = notes[noteBases[0] + manualModifiers[0] + requestedFret[0]];}
  if(requestedFret[1]!=-1){requestedNote[1] = notes[noteBases[1] + 5 + manualModifiers[1] + requestedFret[1]];}
  if(requestedFret[2]!=-1){requestedNote[2] = notes[noteBases[2] + 10 + manualModifiers[2] + requestedFret[2]];}
  if(requestedFret[3]!=-1){requestedNote[3] = notes[noteBases[3] + 15 + manualModifiers[3] + requestedFret[3]];}
  
  if(tempWaveSelector>=8){
    for(int i=0;i<4;i++){
      if(requestedNote[i]!=0){
        int currentNoteIndex = requestedFret[i] + (i*5) + manualModifiers[i] + noteBases[i];
        requestedNote[0]=0; requestedNote[1]=0; requestedNote[2]=0; requestedNote[3]=0;
        numNotesPlaying=0;
        if(chorus){
          requestedNote[0] = notes[currentNoteIndex];
          requestedNote[1] = notes[currentNoteIndex];
          requestedNote[2] = notes[currentNoteIndex];
          numNotesPlaying=3;
          
        }
        else{
          requestedNote[0] = notes[currentNoteIndex];
          numNotesPlaying++;
          if(currentNoteIndex-12>=0){
            requestedNote[1] = notes[currentNoteIndex-12];
            numNotesPlaying++;
          }
          if(currentNoteIndex+12<68){
            requestedNote[2] = notes[currentNoteIndex+12];
            numNotesPlaying++;
          }
        }
        break;
      }
    }
    tempWaveSelector=tempWaveSelector-8;
  }
  if(warble==true&&glissando==false){
    if(upwardsWarble){
      warbleCounter++;
    }
    else{
      warbleCounter--;
    }
    if(warbleCounter>1){
      warbleCounter=0;
      upwardsWarble=false;
    }
    if(warbleCounter<-1){
      warbleCounter=0;
      upwardsWarble=true;
    }
    for(int i=0;i<4;i++){
      requestedNote[i]=requestedNote[i]+(warbleCounter*(requestedNote[i]/warbleDivisor));
    }
  }

  //determined needed multipliers depending on amount of notes pressed 
  if(numNotesPlaying==4){totalMultiplier=12;}
  else if(numNotesPlaying==3){totalMultiplier=16;}
  else if(numNotesPlaying==2){totalMultiplier=24;}
  else if(numNotesPlaying==1){totalMultiplier=40;}
  else{totalMultiplier=0;}


  
  if(glissando==false){
    sendNotes();
  }

  if(glissando==true){
    for(int i=0;i<4;i++){
      if(requestedFret[i]!=-1&&requestedFret[i]!=lastPlayedFret[i]){
        sendNotes(); 
        break;
      }

    }
    
  } 
  

  
  
}//end application loop




