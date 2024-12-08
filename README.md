****************************************************
Ved Atal - 2024
****************************************************

Software is divided into 2 layers.<br />
Hardware Drivers
 * Control the underlying hardware of the RPi.
 * GPIO - deals with general purpose I/O.
 * SPI - deals with the SPI pins on the RPi, used for communication.
 * Drivers are made by WiringPi library and this program abtracts over that in a wrapper class for some additioanl functionality.

Board Controllers
 * MCP23S17 - i/o expander ic
 * AD8802 - DAC ic
 * LTC2380 - ADC ic

General Notes.<br />
WiringPi
 * Drivers for the RPi.
 * Necessary to go to WiringPi github and follow installation steps to install their library for the drivers to work.

compile.sh
 * Run this bash script to compile the program, it's stored in here becuase it a long command and this makes it easy to run.

****************************************************
