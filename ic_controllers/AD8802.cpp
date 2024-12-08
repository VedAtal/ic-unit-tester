/*
 * AD8802.cpp:
 ***********************************************************************
 * Interfaces with:
 *      AD8802 SPI based 12-channel DAC chip. 
 *      https://www.analog.com/media/en/technical-documentation/data-sheets/AD8802_8804.pdf
 ***********************************************************************
 */


#include <wiringPi.h>

#include "AD8802.hpp"
#include "../hardware_drivers/spi.hpp"
#include "../hardware_drivers/gpio.hpp"

int AD8802Controller::initAD8802(SPIDriver& spi, GPIODriver& gpio) {
    // Sets all voltages to 0 initially.
    for (int dacOut = 0; dacOut < 11; dacOut++) {
        applyVoltage(spi, gpio, dacOut, 0.0, DAC_1_CS);
        applyVoltage(spi, gpio, dacOut, 0.0, DAC_2_CS);
    }
    return 0;
};

void AD8802Controller::applyVoltage(SPIDriver& spi, GPIODriver& gpio, int dacOutput, double voltage, int cs) {
    // Get voltage input data for the DAC and combine it with the output address.
    uint16_t dataRaw = dacInputData(voltage);
    dataRaw = dataRaw << 8;
    dataRaw = dataRaw | DAC_OUTPUT_ADDRESSES[dacOutput];

    uint8_t data[2];
    // Sends 4 address bits. 
    data[0] = dataRaw & 0x0F;
    // The last 8 value bits read.
    data[1] = (dataRaw >> 8) & 0xFF;

    // Sends SPI data to apply voltage.
    gpio.low(cs);
    if (spi.readWrite(data, 2) == -1) {
        return;
    }
    gpio.high(cs);
};

uint16_t AD8802Controller::dacInputData(double voltage) {
    // Calculates closest matching value in the increment from 0-255 for given voltage.
    uint16_t inputValue = (uint16_t)(std::round((voltage/MAX_OUTPUT_VOLTAGE)*DAC_DIVISON_FACTOR));
    // Clamps output to max value.
    inputValue = inputValue > MAX_INPUT_VALUE ? MAX_INPUT_VALUE : inputValue;

    return inputValue;
};