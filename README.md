# Anvil
## Overview
Anvil is a sumo robot. This repo containts the code of our first iteration
## Hardware
The system was designet to incorporate 2 AVRs: one that would control the motor driver (atmega326) and one that is the main logical controller, responsible for reading the sensors, taking decisions and sending commands to the motor driver.
## Firmware
The system was designed with speed beeing the main feature, so this is the main argument why no arduino code was used. We developed our own customised HAL that would best fit our needs.
## Links
###### [Facebook page](https://www.facebook.com/anvilsumobot/)
You can reach us for more info.
###### Youtube
* [A match of or first contest](https://www.youtube.com/watch?v=rlrTxM9EhVk)
* [Testing gone wrong](https://www.youtube.com/watch?v=shfqgkyOkAU)
## Disclaimer 
This code was abandoned while beeing unde development as we started developing the firware on another platform, another approach as this did not suit our needs (flexibility, rather that execution speed).
Even if this code worked well, it should be used just as inspiration.
## TODO
* Organise strategies folders
* Comment more code
* Upload state diagram
