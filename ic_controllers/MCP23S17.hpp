/*
 * dio.hpp:
 ***********************************************************************
 * Digital Input Output (DIO). Interfaces with:
 *      MCP23S17 SPI based I/O expander chip. 
 *      https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP23017-Data-Sheet-DS20001952.pdf
 ***********************************************************************
 */


#include <string>
#include <cstdint>

#include "../hardware_drivers/spi.hpp"
#include "../hardware_drivers/gpio.hpp"

#ifndef MCP23S17CONTROLLER
#define MCP23S17CONTROLLER

/** Addresses of the registers on the MCP23S17 I/O expander chip. From datasheet. */
#define IODIRA 0x00
#define IODIRB 0x01
#define IPOLA 0x02
#define IPOLB 0x03
#define GPINTENA 0x04
#define GPINTENB 0x05
#define DEFVALA 0x06
#define DEFVALB 0x07
#define INTCONA 0x08
#define INTCONB 0x09
#define IOCON 0x0A
#define IOCONAUX 0x0B
#define GPPUA 0x0C
#define GPPUB 0x0D
#define INTFA 0x0E
#define INTFB 0x0F
#define INTCAPA 0x10
#define INTCAPB 0x11
#define GPIOA 0x12
#define GPIOB 0x13
#define OLATA 0x14
#define OLATB 0x15

class MCP23S17Controller {
    private:
        /** List of all expanders. */
        typedef enum {
            PRIMARY_EXPANDER_1, // Secondary Expanders 3-18
            PRIMARY_EXPANDER_2, // Secondary Expanders 19-34
            SECONDARY_EXPANDER_3,
            SECONDARY_EXPANDER_4,
            SECONDARY_EXPANDER_5,
            SECONDARY_EXPANDER_6,
            SECONDARY_EXPANDER_7,
            SECONDARY_EXPANDER_8,
            SECONDARY_EXPANDER_9,
            SECONDARY_EXPANDER_10,
            SECONDARY_EXPANDER_11,
            SECONDARY_EXPANDER_12,
            SECONDARY_EXPANDER_13,
            SECONDARY_EXPANDER_14,
            SECONDARY_EXPANDER_15,
            SECONDARY_EXPANDER_16,
            SECONDARY_EXPANDER_17,
            SECONDARY_EXPANDER_18,
            SECONDARY_EXPANDER_19,
            SECONDARY_EXPANDER_20,
            SECONDARY_EXPANDER_21,
            SECONDARY_EXPANDER_22,
            SECONDARY_EXPANDER_23,
            SECONDARY_EXPANDER_24,
            SECONDARY_EXPANDER_25,
            SECONDARY_EXPANDER_26,
            SECONDARY_EXPANDER_27,
            SECONDARY_EXPANDER_28,
            SECONDARY_EXPANDER_29,
            SECONDARY_EXPANDER_30,
            SECONDARY_EXPANDER_31,
            SECONDARY_EXPANDER_32,
            SECONDARY_EXPANDER_33,
            SECONDARY_EXPANDER_34,
        } Expanders;

        /** Pin numbers on the RaspebrryPi of the chip selects for the primary expanders.*/
        const int PRIMARY_EXPANDERS_CS[2] = {
            [PRIMARY_EXPANDER_1] = 21,
            [PRIMARY_EXPANDER_2] = 22
        };

        /** Opcodes necessary to communicate with the primary and secondary expanders. */
        const int PRIMARY_WRITE_OPCODE = 0x42;
        const int PRIMARY_READ_OPCODE = 0x43;
        const int SECONDARY_WRITE_OPCODE = 0x40;
        const int SECONDARY_READ_OPCODE = 0x41;

        /** 
         * Writes a 1 byte value to all present primary expanders.
         * @param spi a SPI driver.
         * @param gpio a GPIO driver.
         * @param regAdress the MCP register address that needs to be written to.
         * @param value the 1 byte data being written. 
         */
        void primaryWrite(SPIDriver& spi, GPIODriver& gpio, uint8_t regAdress, uint8_t value);

        /** 
         * Writes a 1 byte value to the selected primary expander.
         * @param spi a SPI driver.
         * @param gpio a GPIO driver.
         * @param regAdress the MCP register address that needs to be written to.
         * @param value the 1 byte data being written.
         * @param CS chip select of the primary expander being written to.
         */
        void primaryWrite(SPIDriver& spi, GPIODriver& gpio, uint8_t regAdress, uint8_t value, int CS);

        /** 
         * Reads a 1 byte value to the selected primary expander.
         * @param spi a SPI driver.
         * @param gpio a GPIO driver.
         * @param regAdress the MCP register address that needs to be read from.
         * @param CS chip select of the primary expander being read from.
         * @return the 1 byte data read from the device.
         */
        uint8_t primaryRead(SPIDriver& spi, GPIODriver& gpio, uint8_t regAdress, int CS);

        /**
         * Writes a 1 byte message to all secondary expanders with CS low. 
         * IMPORTANT - that only one secondary expander has CS low at a time.
         * Otherwise devices may recieve unwanted commands.
         * @param spi a SPI driver.
         * @param gpio a GPIO driver.
         * @param regAdress the MCP register address that needs to be written to.
         * @param value the 1 byte data being written.
         */
        void secondaryWrite(SPIDriver& spi, GPIODriver& gpio, uint8_t regAdress, uint8_t value);

         /**
         * Reads a 1 byte message from a secondary expander with CS low.
         * IMPORTANT - that only one secondary expander has CS low at a time.
         * Otherwise incorrect data could be recieved.
         * @param spi a SPI driver.
         * @param gpio a GPIO driver.
         * @param regAdress the MCP register address that needs to be read from.
         * @return the 1 byte data read from the device.
         */
        uint8_t secondaryRead(SPIDriver& spi, GPIODriver& gpio, uint8_t regAdress);

    public:
        /**
         * Struct containing the vital information regarding each pin on the DIO.
         * @param primaryExpander either 0 or 1 indicating one of the two primary expanders.
         * @param primaryPin indicates the pin with the CS of the seconday expander.
         * @param secondaryExpander in the range of 2-33.
         * @param seondaryPin indicates the pin on the secondary expander which the DIO pin resides on.
        */
        struct DIOPinInfo {
            int primaryExpander;
            int primaryPin;
            int secondaryExpander;
            int secondaryPin;
        };

        /**
         * Completes proper intialization procedure to ensure MCP23S17 board is in ready state.
         * Ensures expanders are reset and not communication over the SPI bus on bootup.
         * @param spi a SPI diver.
         * @param gpio a GPIO driver.
         * @returns -1 if initialization failed.
         */
        int initMCP23S17(SPIDriver& spi, GPIODriver& gpio);

        /**
         * Enables the provided DIO pin.
         * @param spi a SPI driver.
         * @param gpio a GPIO driver.
         * @param DIOPin the DIO pin that needs to be enabled.
         */
        void enablePin(SPIDriver& spi, GPIODriver& gpio, DIOPinInfo DIOPin);

        /**
         * Disables the provided DIO pin.
         * @param spi a SPI driver.
         * @param gpio a GPIO driver.
         * @param DIOPin the DIO pin that needs to be disabled.
         */
        void disablePin(SPIDriver& spi, GPIODriver& gpio, DIOPinInfo DIOPin);

};

#endif