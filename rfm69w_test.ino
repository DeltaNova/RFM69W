/*
    rfm69w_test.ino - An Arduino script to test SPI comms with RFM69W
*/

#include "spi.h" // Include my spi library
// Include Arduino Wire library. Used for serial comms.
#include <Wire.h>

void setup()
{
    Serial.begin(9600); // Setup Serial Comms
    Spi::Spi SPI;   // Create an instance of the Spi Class
    SPI.InitMaster(); // Initialise the Class instance
    SPI.SetClock(1); // Change the SPI Clock rate.
}

uint8_t singleByteRead(uint8_t byteAddr)
{
    // Read a single byte from an address over the SPI interface
    uint8_t readByte; // Place to store returned data
    SPI.SelectSlave();// Set Slave Select line low
    SPI.transeiver(byteAddr); // Request data from address
    readByte = SPI.transeiver(0); // Pad the shift register to get data.
    SPI.DeselectSlave();//Set Slave Select line high
    return readByte;
}


void loop()
{
    
}
