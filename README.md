Example Code For Using Analog To Digital Conversion on STM32F4DISCOVERY (Cortex M4-F) Using FreeRTOS
========================

This is contains example code (check : src/main.c) and project for compiling and porting to STM32F4DISCOVERY. Tested on a STM32F4 Discovery board.

Example code which captures Analog data on ADC1 Channel 16 and sends it to USART2

Toolchain
---------

The project is known to work with following toolchains :

* Sourcery G++ Lite 2011.03-42
* summon-arm-toolchain : <https://github.com/esden/summon-arm-toolchain>

Should work fine with other toolchains for Cortex. 

Compiling
---------

Clone the repo and enter the directory. Use following to get .elf & .bin in obj directory.

    $ make

To clean : 

    $ make clean

Running & Debugging
-------

This code was ported to STM32F4DISCOVERY using st-link : <https://github.com/texane/stlink>
Configure and follow instruction at above link on how to flash and debug it.

Credit
------

Credit to Thomas Kindler on sharing FreeRTOS port demo : <https://github.com/thomask77/STM32F4_demo>

License
-------

MIT licensed

Copyright (C) 2013 Chetan Patil, http://chetanpatil.info
