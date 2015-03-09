/*
    rfm69w_test.ino - An Arduino script to test SPI comms with RFM69W
*/

#include "spi.h" // Include my spi library.
#include <Wire.h> // Used for serial comms.
#include <stdint.h> // Enable fixed width integers.

Spi SPI;   // Create Global instance of the Spi Class

void setup()
{
    Serial.begin(19200); // Setup Serial Comms
    SPI.InitMaster(); // Initialise as Master SPI Node
    SPI.SetClock(1); // Change the SPI Clock rate.
    SPI.EnableSPI(); // Enable SPI Communication.
}

uint8_t singleByteRead(uint8_t byteAddr)
{
    // Read a single byte from an address over the SPI interface
    uint8_t readByte; // Place to store returned data
    SPI.SelectSlave();// Set Slave Select line low
    SPI.Transiever(byteAddr); // Request data from address
    readByte = SPI.Transiever(0); // Pad the shift register to get data.
    SPI.DeselectSlave();//Set Slave Select line high
    return readByte;
}


void loop()
{
        //DEBUG - Print the SPI Registers to Serial Output
        Serial.print("SPCR: ");
        Serial.println(SPCR,BIN);
        Serial.print("SPSR: ");
        Serial.println(SPSR,BIN);
        
        //End DEBUG
        uint8_t datain = singleByteRead(0x2d);
        Serial.print("ADDR: ");
        Serial.println(0x2d,HEX);
        Serial.print("Data: ");
        Serial.println(datain,HEX);
        Serial.println();
        delay(1000);
}
