/*
 * MCP23S17.cpp:
 ***********************************************************************
 * Interfaces with:
 *      MCP23S17 SPI based I/O expander chip. 
 *      https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP23017-Data-Sheet-DS20001952.pdf
 ***********************************************************************
 */


#include <wiringPi.h>

#include "MCP23S17.hpp"
#include "../hardware_drivers/spi.hpp"
#include "../hardware_drivers/gpio.hpp"

int MCP23S17Controller::initMCP23S17(SPIDriver& spi, GPIODriver& gpio) {
    // Enables the IOCON.HAEN bit which enables hardware addressing.
    //   All expanders are addressed here since there is no distinintction between primaries and secondaries 
    //   before HAEN is turned on. Secondaries have CS low by deafult, this init handles that.
    primaryWrite(spi, gpio, IOCON, 0x08);

    // Sets the I/O direction of the primary expanders to output.
    primaryWrite(spi, gpio, IODIRA, 0x00);
    primaryWrite(spi, gpio, IODIRB, 0x00);
    
    primaryWrite(spi, gpio, OLATA, 0xFF);
    primaryWrite(spi, gpio, OLATB, 0xFF);

    // Sets the I/O direction of the secondary expanders to output.
    primaryWrite(spi, gpio, OLATA, 0x00);
    primaryWrite(spi, gpio, OLATB, 0x00);
    secondaryWrite(spi, gpio, IODIRA, 0x00);
    primaryWrite(spi, gpio, OLATA, 0xFF);
    primaryWrite(spi, gpio, OLATB, 0xFF);

    primaryWrite(spi, gpio, OLATA, 0x00);
    primaryWrite(spi, gpio, OLATB, 0x00);
    secondaryWrite(spi, gpio, IODIRB, 0xFF);
    primaryWrite(spi, gpio, OLATA, 0xFF);
    primaryWrite(spi, gpio, OLATB, 0xFF);

    return 0;
};

void MCP23S17Controller::enablePin(SPIDriver& spi, GPIODriver& gpio, DIOPinInfo pin) {
    // Finds CS location of the secondary on the primary.
    int primaryRegAddress = pin.primaryPin < 8 ? OLATA : OLATB;
    int primaryValue = ~(0b00000001 << (pin.primaryPin % 8));
   
    // Reads current state of secondary, so other pins are maintained.
    primaryWrite(spi, gpio, primaryRegAddress, primaryValue, PRIMARY_EXPANDERS_CS[pin.primaryExpander]);
    int secondaryRegAddress = pin.secondaryPin < 8 ? OLATA : OLATB;
    uint8_t currentState = secondaryRead(spi, gpio, secondaryRegAddress);
    primaryWrite(spi, gpio, primaryRegAddress, 0xFF, PRIMARY_EXPANDERS_CS[pin.primaryExpander]);
    
    // Turns on selected pin.
    primaryWrite(spi, gpio, primaryRegAddress, primaryValue, PRIMARY_EXPANDERS_CS[pin.primaryExpander]);
    int secondaryValue = currentState | (0b00000001 << (pin.secondaryPin % 8));
    secondaryWrite(spi, gpio, secondaryRegAddress, secondaryValue);
    primaryWrite(spi, gpio, primaryRegAddress, 0xFF, PRIMARY_EXPANDERS_CS[pin.primaryExpander]);
}

void MCP23S17Controller::disablePin(SPIDriver& spi, GPIODriver& gpio, DIOPinInfo pin) {
    // Finds CS location of the secondary on the primary.
    int primaryRegAddress = pin.primaryPin < 8 ? OLATA : OLATB;
    int primaryValue = ~(0b00000001 << (pin.primaryPin % 8));

    // Reads current state of secondary, so other pins are maintained.
    primaryWrite(spi, gpio, primaryRegAddress, primaryValue, PRIMARY_EXPANDERS_CS[pin.primaryExpander]);
    int secondaryRegAddress = pin.secondaryPin < 8 ? OLATA : OLATB;
    uint8_t currentState = secondaryRead(spi, gpio, secondaryRegAddress);
    primaryWrite(spi, gpio, primaryRegAddress, 0xFF, PRIMARY_EXPANDERS_CS[pin.primaryExpander]);

    // Turns off selected pin.
    primaryWrite(spi, gpio, primaryRegAddress, primaryValue, PRIMARY_EXPANDERS_CS[pin.primaryExpander]);
    int secondaryValue = currentState & ~(0b00000001 << (pin.secondaryPin % 8));
    secondaryWrite(spi, gpio, secondaryRegAddress, secondaryValue);
    primaryWrite(spi, gpio, primaryRegAddress, 0xFF, PRIMARY_EXPANDERS_CS[pin.primaryExpander]);
}



void MCP23S17Controller::primaryWrite(SPIDriver& spi, GPIODriver& gpio, uint8_t regAddress, uint8_t value) {
    uint8_t data[4];
    data[0] = PRIMARY_WRITE_OPCODE;
    data[1] = regAddress;
    data[2] = value;

    for (int CS : PRIMARY_EXPANDERS_CS) {
        gpio.low(CS);
    }
    delayMicroseconds(100);
    if (spi.readWrite(data, 3) == -1) {
        return;
    }
    delayMicroseconds(100);
    for (int CS : PRIMARY_EXPANDERS_CS) {
        gpio.high(CS);
    }
};

void MCP23S17Controller::primaryWrite(SPIDriver& spi, GPIODriver& gpio, uint8_t regAddress, uint8_t value, int CS) {
    uint8_t data[4];
    data[0] = PRIMARY_WRITE_OPCODE;
    data[1] = regAddress;
    data[2] = value;

    gpio.low(CS);
    delayMicroseconds(100);
    if (spi.readWrite(data, 3) == -1) {
        return;
    }
    delayMicroseconds(100);
    gpio.high(CS);
};

uint8_t MCP23S17Controller::primaryRead(SPIDriver& spi, GPIODriver& gpio, uint8_t regAddress, int CS) {
    uint8_t data[4];
    data[0] = PRIMARY_READ_OPCODE;
    data[1] = regAddress;
    data[2] = 0x00;

    gpio.low(CS);
    delayMicroseconds(100);
    if (spi.readWrite(data, 3) == -1) {
        return -1;
    }
    delayMicroseconds(100);
    gpio.high(CS);

    return data[2];
};

void MCP23S17Controller::secondaryWrite(SPIDriver& spi, GPIODriver& gpio, uint8_t regAddress, uint8_t value) {
    uint8_t data[4];
    data[0] = SECONDARY_WRITE_OPCODE;
    data[1] = regAddress;
    data[2] = value;

    if (spi.readWrite(data, 3) == -1) {
        return;
    }
};

uint8_t MCP23S17Controller::secondaryRead(SPIDriver& spi, GPIODriver& gpio, uint8_t regAddress) {
    uint8_t data[4];
    data[0] = SECONDARY_READ_OPCODE;
    data[1] = regAddress;
    data[2] = 0x00;

    if (spi.readWrite(data, 3) == -1) {
        return -1;
    }

    return data[2];
};