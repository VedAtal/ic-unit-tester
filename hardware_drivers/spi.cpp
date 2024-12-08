/*
 * spi.cpp:
 ***********************************************************************
 * Wrapper for the SPI Driver created by WiringPi.
 *      https://github.com/WiringPi/WiringPi
 *      Has additional functionality to support project specific SPI devices.
 ***********************************************************************
 */


#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "spi.hpp"


int SPIDriver::initSPI() {
    // Enables SPI functionality.
    if (wiringPiSPISetup(SPI_CHANNEL_0, SPI_BAUDRATE) < 0) {
        return -1;
    }   

    // Sets all CS pins to HIGH to deselect SPI devices on initialization.
    for (int CS : SPI_CHIP_SELECTS) {
        digitalWrite(CS, HIGH);
    }

    return 0;
};

int SPIDriver::readWrite(unsigned char* data, int len) {
    return wiringPiSPIDataRW(SPI_CHANNEL_0, data, len);
};
