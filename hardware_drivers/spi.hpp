/*
 * spi.hpp:
 ***********************************************************************
 * Wrapper for the SPI Driver created by WiringPi.
 *      https://github.com/WiringPi/WiringPi
 *      Has additional functionality to support project specific SPI devices.
 ***********************************************************************
 */


#ifndef SPIDRIVER
#define SPIDRIVER

// DEBUG -- CAN BE USED TO PRINT INDIVIDUAL BITS OF A BYTE, with printf
#define BIN "%c%c%c%c%c%c%c%c"
#define BINS(byte) \
    ((byte) & 0x80 ? '1' : '0'), \
    ((byte) & 0x40 ? '1' : '0'), \
    ((byte) & 0x20 ? '1' : '0'), \
    ((byte) & 0x10 ? '1' : '0'), \
    ((byte) & 0x08 ? '1' : '0'), \
    ((byte) & 0x04 ? '1' : '0'), \
    ((byte) & 0x02 ? '1' : '0'), \
    ((byte) & 0x01 ? '1' : '0') 

class SPIDriver {
    private:
        /** SPI baudrate defines speed of data over the bus. Set to 8 Mhz. */
        const int SPI_BAUDRATE = 8000000;

        /** Enabling SPI channel also starts automatic control of pin 10 as CS. */
        const int SPI_CHANNEL_0 = 0;

        /** Defines pins used as CS on the RaspberryPi. */
        const int SPI_AUTO_CHIP_SELECTS[1] = {10}; // DO NOT USE
        const int SPI_CHIP_SELECTS[5] = {21,22,23,24,25};

    public:
        /**
         * Ensures proper initialization of WiringPi SPI driver.
         * Changes all SPI CS pins to HIGH to deselect SPI devices on initialization. 
         * @returns -1 if initialization failed.
         */
        int initSPI();

        /**
         * SPI command to read and write over the SPI bus.
         * @param data holds data being trasmitted and data being recieved.
         * @param len indicates the length of data.
         * @returns -1 if read/write failed.
         */
        int readWrite(unsigned char* data, int len); 
};

#endif