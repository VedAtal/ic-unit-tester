/*
 * gpio.cpp:
 ***********************************************************************
 * Wrapper for the GPIO Driver created by WiringPi.
 *      https://github.com/WiringPi/WiringPi
 *      Has additional functionality to support project specific GPIO functions.
 ***********************************************************************
 */


#include <wiringPi.h>
#include <iostream>

#include "gpio.hpp"


int GPIODriver::initGPIO() {
    // Sets all GPIO pins to OUTPUT mode and turns them LOW.
    for (int pin : GPIO_OUTPUT_PINS) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    // Checks if all GPIO pins are correctly set to LOW.
    bool pinsIntialized = true;
    for (int pin : GPIO_OUTPUT_PINS) {
        if (digitalRead(pin) != LOW) {
            std::cout << "GPIO pin: " << pin << " failed to initialize correctly.\n";
            pinsIntialized = false;
        }
    }
    if (pinsIntialized == false) {
        return -1;
    }

    return 0;
};

void GPIODriver::high(int pin) {
    digitalWrite(pin, HIGH);
};

void GPIODriver::low(int pin) {
    digitalWrite(pin, LOW);
}