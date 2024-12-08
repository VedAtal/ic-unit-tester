/*
 * main.cpp:
 *	Created in 2024 by Ved Atal
 ***********************************************************************
 */


#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>

#include "hardware_drivers/gpio.hpp"
#include "hardware_drivers/spi.hpp"
#include "ic_controllers/MCP23S17.hpp"
#include "ic_controllers/AD8802.hpp"
#include "ic_controllers/LTC2380.hpp"

class TestProgram {
    private:
        GPIODriver gpio;
        SPIDriver spi;
        MCP23S17Controller MCP23S17;
        AD8802Controller AD8802;
        LTC2380Controller LTC2380;
    public:
        /**
         * Main program--executes all logic.
         */
        void run() {
            
            }
        };


        /** 
         * Ensures all bootups are successful, ITR is loaded, and user input is provided.
         */
        bool preExecutionChecks() {
            // Some safety and configuration steps prior to running tests.
            if (systemBootupChecks() == false) {
                std::cout << "Program failed to bootup properly. Exiting Program.\n";
                return false;
            } else {
                std::cout << "Program bootup successful.\n";
            }

            return true;
        };


        /**
         * Ensures all necessary systems properly initialize before running program.
         * @returns false if validation failed.
         */
        bool systemBootupChecks() {
            bool passedChecks = true;

            // Stage 1: Setup hardware library.
            if (wiringPiSetup() == -1) {
                std::cout << "RaspberryPi hardware library--WiringPi setup failed.\n";
                passedChecks = false;
            } else {
                std::cout << "RaspberryPi hardware library--WiringPi setup successful.\n";
            }

            // Stage 2: Setup drivers.
            if (passedChecks) {
                if (gpio.initGPIO() == -1) {
                    std::cout << "RaspberryPi GPIO Driver setup failed.\n";
                    passedChecks = false;
                } else {
                    std::cout << "RaspberryPi GPIO Driver setup successful.\n";
                }

                if (spi.initSPI() == -1) {
                    std::cout << "RaspberryPi SPI Driver setup failed.\n";
                    passedChecks = false;
                } else {
                    std::cout << "RaspberryPi SPI Driver setup successful.\n";
                }
            }

            // Stage 3: Setup boards.
            if (passedChecks) {
                if (dio.initMCP23S17(spi, gpio) == -1) {
                    std::cout << "MCP23S17 Board setup failed.\n";
                    passedChecks = false;
                } else {
                    std::cout << "MCP23S17 Board setup successful.\n";
                }

                if (psu.initAD8802(spi, gpio) == -1) {
                    std::cout << "AD8802 Board setup failed.\n";
                    passedChecks = false;
                } else {
                    std::cout << "PAD8802SU Board setup sucessful.\n";
                }

                if (dmm.initLTC2380(spi, gpio) == -1) {
                    std::cout << "LTC2380 Board setup failed.\n";
                    passedChecks = false;
                } else {
                    std::cout << "LTC2380 Board setup sucessful.\n";
                }
            }

            return passedChecks;
        }

};


int main(void) {
    TestProgram test;
    test.run();

    return 0;
}