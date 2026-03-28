Members

Shane Pomajambo, Electrical Engineering Student (2024)
Shanepomajambo16@vt.edu

Mentor

Arthur Ball

Current Status

IN PROGRESS

Project Overview

The purpose of this project is to create the CRT Telecaster guitars used by the Band ELECTRONICOS FANTASTICOS (https://www.youtube.com/@ELECTRONICOSFANTASTICOS_YT). The way it works is that audio signals such as square, sine, and triangle waves are sent to the video jack of a CRT television, which leads it to display the wave in the form of white and black bars. Since CRTs are a significant source of EM radiation, the EM waves can be picked up with either an inductor or by connecting the + end of an active speaker system and inducing voltage through the capacitive effect of touching the screen. Notes can be played, and the sound can be changed by moving your hand closer or farther to the screen, similar to a therevin type insturment.

The design of this instrument uses a Driver PCB equipped with an ESP32. The DAC modules built into the dev board act as a wave generator, which generates sound waves to feed into the CRT input. In order to play a certain note, 4 ADC pins with 1K resistors to ground are connected to a button and resistor array that form a 3.3V voltage divider. Values are chosen in order to be 0.15V, and each button represented a fret, similar to how a guitar would be played. Several buttons and knobs are also included to have programable affects, such as changing the type of wave, holding inputs, or having sound effects.

Through other Virginia Tech maker spaces, a working prototype has been created. However, issues still exist with grounding, playing different notes, reliability, and compactness. The main goal of the project will be to further improve the design.

Educational Value Added

One primary educational aspect of the project will be the development of the PCB. I am currently working on a redesign of the PCB Driver that utlizes a onboard WROOM ESP32 Chip rather than a dev board to save on space. I believe this aspect will give valuable experience in PCB development using onboard microcontrollers, which is much more common in industry. The oppurtunity to improve on an existing PCB design will allow me to practice proper PCB design standards and rules. 



There is also a large amount of educational value in programming as well. The inclusion of SPI DAC modules will expose me to embedded systems programing hardware interrupts.

Tasks

<!-- Your Text Here. You may work with your mentor on this later when they are assigned -->

Design Decisions

<!-- Your Text Here. You may work with your mentor on this later when they are assigned -->

Design Misc

<!-- Your Text Here. You may work with your mentor on this later when they are assigned -->

Steps for Documenting Your Design Process

<!-- Your Text Here. You may work with your mentor on this later when they are assigned -->

BOM + Component Cost

<!-- Your Text Here. You may work with your mentor on this later when they are assigned -->

Timeline

<!-- Your Text Here. You may work with your mentor on this later when they are assigned -->

Useful Links

<!-- Your Text Here. You may work with your mentor on this later when they are assigned -->

Log

<!-- Your Text Here. You may work with your mentor on this later when they are assigned -->
