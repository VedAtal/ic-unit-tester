/*
 * AD8802.hpp:
 ***********************************************************************
 * Interfaces with:
 *      AD8802 SPI based 12-channel DAC chip. 
 *      https://www.analog.com/media/en/technical-documentation/data-sheets/AD8802_8804.pdf
 ***********************************************************************
 */


#include <cstdint>
#include <cmath>

#include "../hardware_drivers/spi.hpp"
#include "../hardware_drivers/gpio.hpp"

#ifndef AD8802CONTROLLER
#define AD8802CONTROLLER

class AD8802Controller {
    private:
        /** List of all DAC outputs. */
        typedef enum {
            DAC_OUTPUT_1,
            DAC_OUTPUT_2,
            DAC_OUTPUT_3,
            DAC_OUTPUT_4,
            DAC_OUTPUT_5,
            DAC_OUTPUT_6,
            DAC_OUTPUT_7,
            DAC_OUTPUT_8,
            DAC_OUTPUT_9,
            DAC_OUTPUT_10,
            DAC_OUTPUT_11,
            DAC_OUTPUT_12,
        } DAC_OUTPUTS;

        /** Pin number on the RaspebrryPi of the chip select for the DAC. */
        const int DAC_1_CS = 23;
        const int DAC_2_CS = 24;

        /** Addresses used in the SPI command for the 12 different DAC outputs. */
        const int DAC_OUTPUT_ADDRESSES[12] = {
            [DAC_OUTPUT_1] = 0x00,
            [DAC_OUTPUT_2] = 0x01,
            [DAC_OUTPUT_3] = 0x02,
            [DAC_OUTPUT_4] = 0x03,
            [DAC_OUTPUT_5] = 0x04,
            [DAC_OUTPUT_6] = 0x05,
            [DAC_OUTPUT_7] = 0x06,
            [DAC_OUTPUT_8] = 0x07,
            [DAC_OUTPUT_9] = 0x08,
            [DAC_OUTPUT_10] = 0x09,
            [DAC_OUTPUT_11] = 0x0A,
            [DAC_OUTPUT_12] = 0x0B
        };

        /** Max voltage output. */
        const double MAX_OUTPUT_VOLTAGE = 5.0; // IMPORTANT VALUE, ENSURE CORRECTNESS

        /** Max value that can be sent over SPI for DAC control. 8-bit. */
        const int MAX_INPUT_VALUE = 255;

        /** Value that the DAC divides input value by to get voltage output. */
        const int DAC_DIVISON_FACTOR = 256;

        /** 
         * Calculates the value that needs to be sent to the DAC for a desired voltage.
         * @param voltage the desired voltage output from the DAC.
         * @returns a value from 0-255 to be sent as input to the DAC.
         */
        uint16_t dacInputData(double voltage);
    
    public:
        /** CS used for ZIF pins. */
        static const int DAC_CS = 23;

        /**
         * Completes proper intialization procedure to ensure AD8802 board is in ready state.
         * Ensures DAC is reset.
         * @param spi a SPI diver.
         * @param gpio a GPIO driver.
         * @returns -1 if initialization failed.
         */
        int initAD8802(SPIDriver& spi, GPIODriver& gpio);

        /**
         * Applies a given voltage to the DAC output specified.
         * @param spi a SPI diver.
         * @param gpio a GPIO driver.
         * @param dacOutput the DAC output channel, 0-11.
         * @param voltage the desired voltage to be applied, 0-5V.
         * @param cs the cs of the desired DAC.
         */
        void applyVoltage(SPIDriver& spi, GPIODriver& gpio, int dacOutput, double voltage, int cs);
};

#endif