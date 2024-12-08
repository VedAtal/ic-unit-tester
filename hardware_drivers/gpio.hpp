/*
 * gpio.hpp:
 ***********************************************************************
 * Wrapper for the GPIO Driver created by WiringPi.
 *      https://github.com/WiringPi/WiringPi
 *      Has additional functionality to support project specific GPIO functions.
 ***********************************************************************
 */


#ifndef GPIODRIVER
#define GPIODRIVER

class GPIODriver {
    private:
        /** All GPIO pins that have OUTPUT pin mode function. */
        const int GPIO_OUTPUT_PINS[22] = {0,1,2,3,4,5,6,7,8,9,11,15,16,21,22,23,24,25,26,27,28,29};

        /** All GPIO pins that have ALTERNATE pin mode function. */
        const int GPIO_ALT_PINS[4] = {10,12,13,14};
        
    public:
        /**
         * Ensures proper initialization of WiringPi GPIO driver.
         * Changes pin mode of all GPIO pins to ensure they are in correct operation. 
         * @returns -1 if initialization failed.
         */
        int initGPIO();

        /**
         * Turns a GPIO pin high.
         * @param pin indicates pin to be turned high.
         */
        void high(int pin);

        /**
         * Turns a GPIO pin low.
         * @param pin indicates pin to be turned low.
         */
        void low(int pin);
};

#endif