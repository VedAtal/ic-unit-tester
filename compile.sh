#!/bin/bash

g++ main.cpp hardware_drivers/gpio.cpp hardware_drivers/spi.cpp ic_controllers/MCP23S17.cpp ic_controllers/AD8802.cpp ic_controllers/LTC2380.cpp -o test -lwiringPi

echo Program Compiled!