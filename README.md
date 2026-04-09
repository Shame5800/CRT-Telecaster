Members

Shane Pomajambo, Electrical Engineering Student (2026)
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

One primary educational aspect of the project will be the development of the PCB. Through other personal projects using copper plate PCBS and working on debugging boards on design teams, I have become very familiar with KiCAD. This project will allow me to continue developing this skill while also introducing me to the proper PCB design principles and standards in the industry. The first educational aspect of the redesign of the telecasters' driver board is that I’m aiming to include an onboard ESP32 WROOM chip rather than a dev board, as it would give me exposure to integrating microprocessors directly on the board with programmer chips and USB ports for a more finished product. In addition, since previous designs had relied on copper plate milling and therefore had to follow different design considerations, I can focus on making the redesign compact, easy to interface with , and following proper PCB design principles.

There is also lots of educational value in programming as well. The inclusion of SPI DAC modules or other devices will expose me to embedded systems. In addition, I’ll be able to practice using hardware interrupts and optimizing code when controlling the wave generator.

The project’s development as a whole will give me lots of experience in planning, cost management/optimization, and debugging. Additionally, since very little information exists on how ELECTRONICOS FANTASTICOS’s instruments work, I'm also planning on creating tutorials and documentation on the project’s progress, giving me experience with open source documentation and technical writing.

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
