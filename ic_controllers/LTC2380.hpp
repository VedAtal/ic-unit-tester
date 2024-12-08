/*
 * LTC2380.hpp:
 ***********************************************************************
 * Interfaces with:
 *      LTC2380-24, 24-bit SPI-based differential input ADC.
 *      https://www.analog.com/media/en/technical-documentation/data-sheets/238024fa.pdf
 ***********************************************************************
 */


#include "../hardware_drivers/gpio.hpp"
#include "../hardware_drivers/spi.hpp"

#ifndef LTC2380CONTROLLER
#define LTC2380CONTROLLER

class LTC2380Controller {
    private:
        /** The SDI pin on the ADC which can act as a CS. */
        const int LTC2380_CS = 25;

        /** The conversion start pin, which triggers a new conversion. */
        const int LTC2380_CNV = 29;

        /** The multiplier for the voltage measurement. */
        const double VOLTAGE_MULTIPLY = 20.0;

        /** 2.3% consistent error offset. */
        const double VOLTAGE_ERROR_OFFSET = 1.023;

        /** The divisor for the current measurement. */
        const double CURRENT_DIVIDE = 7.995;

        /** The multiplier for the current measurement. */
        const double CURRENT_MULTIPLY = 10.0;

        /** An estimated nanosecond delay function. */
        void delayNanoseconds(int howLong);

    public:
        /**
         * Completes proper intialization procedure to ensure LTC2380 board is in ready state.
         * Ensures DAC is reset.
         * @param spi a SPI diver.
         * @param gpio a GPIO driver.
         * @returns -1 if initialization failed.
         */
        int initLTC2380(SPIDriver& spi, GPIODriver& gpio);

        /** 
         * Read the current input on the ADC.
         * @param spi a SPI diver.
         * @param gpio a GPIO driver.
         * @param voltage indicates whether the reading is for voltage or current.
         * @returns the value read from the ADC.
         */
        int read(SPIDriver& spi, GPIODriver& gpio, bool voltage);
        
};

#endif