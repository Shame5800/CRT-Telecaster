Members

Shane Pomajambo, Electrical Engineering Student (2026)
Shanepomajambo16@vt.edu

Mentor

Arthur Ball

Current Status

IN PROGRESS

Project Overview

The purpose of this project is to create the CRT Telecaster guitars used by the Band ELECTRONICOS FANTASTICOS (https://www.youtube.com/@ELECTRONICOSFANTASTICOS_YT). The instrument sends 0V to 1V audio signals such as square, sine, and triangle waves to the video jack of a CRT television. This visualizes the waveform in the form of white and black bars, where white is high and black is low. Since CRT televisions are significant sources of electric fields, the audio signal fed into the video jack will become part of these fields, and the field’s rate of change will be the audio’s frequency with the CRT's internal frequencies. 

The note playing is caused by the concept of capacitive coupling. When the user touches the screen of the CRT, the screen and the user's hand form a parallel plate capacitor. On the board, there's an op-amp buffer that takes its input from a small trace and sends the output to an external speaker. When the instrument is in playing position, the trace is in proximity to the user's torso and is far away from any ground fills. This forms a series capacitor network formed by the hand-to-screen capacitance, the torso-to-trace capacitance, and the parasitic capacitance of the op-amp buffer trace. The changing E-field of the CRT creates displacement current on the network, which means the audio signal input to the CRT can be picked up by the capacitor network and sent to the speaker. Since the smallest capacitor in a series capacitor network sees the largest amount of voltage, the user can then change the volume of the note by bringing their hand closer or farther to the screen, which changes the capacitance ratios of the network that determine how much the op-amp buffer sees. 

The design of this instrument uses a driver PCB equipped with an ESP32 and a button array PCB meant to resemble a guitar fretboard. The board is equipped with 4 DAC channels and a non-inverting summing amplifier that leads to the CRT video input. In order to determine the fret being played, the fretboard PCB connects resistor dividers that are read by the ESP32’s ADCs. Each fret is designed to be 0.15V away from eachother, so that the ESP32’s 12-bit resolution sees it as being 200 counts away from eachother and has enough space to provide accurate readings. Once the user inputs a note on the fretboard, the ESP32 uses interrupt-style code to communicate to the DAC’s and have them output the musical frequencies. The driver board also comes equipped with a variety of switches and potentiometers to handle different musical effects and synth waveform structures. The user can alternate between 8 wave types, octaver, and warble effects, as well as bass, 5-string bass, and guitar options.

As of Summer 2026, the V2 of this project has finished development, and the V3 platform is currently in development. The next variation aims to improve mistakes from the previous iteration, including easier-to-access switches and a more ergonomic fretboard, and fixing SPI bus issues encountered in V2’s integration. It also aims to introduce new features, such as a touchscreen LCD allowing the user to draw the waveform that will be generated, more audio effects, and an internal programmable drum machine connected to the CRT’s speakers.

