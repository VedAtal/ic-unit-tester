/*
 * LTC2380.cpp:
 ***********************************************************************
 * Interfaces with:
 *      LTC2380-24, 24-bit SPI-based differential input ADC.
 *      https://www.analog.com/media/en/technical-documentation/data-sheets/238024fa.pdf
 ***********************************************************************
 */


#include <wiringPi.h>
#include <cstdint>

#include "LTC2380.hpp"
#include "../hardware_drivers/spi.hpp"
#include "../hardware_drivers/gpio.hpp"

int LTC2380Controller::initLTC2380(SPIDriver& spi, GPIODriver& gpio) {
    return 0;
};

int LTC2380Controller::read(SPIDriver& spi, GPIODriver& gpio, bool voltage) {
    // Trigger conversion and ensure the trigger is on for adequate time.
    gpio.high(LTC2380_CNV);
    delayNanoseconds(30);
    gpio.low(LTC2380_CNV);

    uint8_t data[5];

    // Enable SDO and read the output from ADC.
    //    First 24 bits are the result, the last 16 are the number of samples averaged.
    //    Might have to tie the SPI read to wait on the BUSY pin on the ADC going low, otherwise it may be misaligned.
    gpio.low(LTC2380_CS);
    if (spi.readWrite(data, 5) == -1) {
        return -1;
    }
    gpio.high(LTC2380_CS);

    // Concatenate the result back into 1 integer.
    int value = data[0] << 24 | data[1] << 16 | data[2] << 8;
    // Takes care of sign extension because output is in two's complement (signed).
    value = value >> 8;

    if (voltage) {
        value = value*VOLTAGE_MULTIPLY;
        value = value*VOLTAGE_ERROR_OFFSET;
    } else {
        value = value/CURRENT_DIVIDE;
        value = value*CURRENT_MULTIPLY;
    }

    return value;
}

// Estimated nanosecond delay.
//    Each instruction takes 1/2.4 GHz (CPU processor speed for RPi5) or ~0.417 ns, so x2.4 = 1 ns.
//    nop is a no operation assembly instruction that should take ~1 cycle.
void LTC2380Controller::delayNanoseconds(int howLong) {
    for (int i = 0; i < howLong*2.4; i++) {
        asm volatile("nop");
    }
}