Example Code For Using Analog To Digital Conversion on STM32F4DISCOVERY (Cortex M4-F) Using FreeRTOS
========================

This is contains example code and project for compiling and porting to STM32F4DISCOVERY.

The project was tested on a STM32F4 Discovery board.

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

License
-------

MIT licensed

Copyright (C) 2013 Chetan Patil, http://chetanpatil.info
