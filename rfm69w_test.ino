// rfm69w_test.ino - An Arduino script to test SPI comms with RFM69W
// ---------------------------------------------------------------------
// Author: M. Tunstall
// NOTE: This is heavily commented for my own learning/reference.

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

void singleByteWrite(uint8_t byteAddr, uint8_t dataByte)
{
    // Write a single byte to an address over the SPI interface.
    SPI.SelectSlave();// Slave Select line low
    // In this instance we are not handling any data the slave returns.
    // To write, the MSB of the address must be 1; MSB 0 to read.
    SPI.Transiever(byteAddr|0x80); // Send the address byte.
    SPI.Transiever(dataByte);   // Send the data byte.
    SPI.DeselectSlave(); // Slave Select line high
    return;
}

void loop()
{
        //Print the SPI Registers to Serial Output
        Serial.println("SPI Registers");
        Serial.print("SPCR: ");
        Serial.println(SPCR,BIN);
        Serial.print("SPSR: ");
        Serial.println(SPSR,BIN);
        delay(1000);

        // SPI - singleByteRead
        Serial.println("SPI - singleByteRead");
        uint8_t datain = singleByteRead(0x2d);
        Serial.print("ADDR: ");
        Serial.println(0x2d,HEX);
        Serial.println("Expected Data 0x03");
        Serial.print("Actual Data: ");
        Serial.println(datain,HEX);
        Serial.println();
        delay(1000);
        
        // SPI - singleByteWrite
        Serial.println("SPI - singleByteWrite");
        singleByteWrite(0x2d,0x04);
        Serial.print("ADDR: ");
        Serial.println(0x2d,HEX);
        Serial.print("Sent Data: ");
        Serial.println(0x04);
        Serial.println();
        delay(1000);
        
        // SPI - singleByteRead
        Serial.println("SPI - singleByteRead");
        uint8_t datain2 = singleByteRead(0x2d);
        Serial.print("ADDR: ");
        Serial.println(0x2d,HEX);
        Serial.println("Expected Data 0x04");
        Serial.print("Actual Data: ");
        Serial.println(datain2,HEX);
        Serial.println();
        delay(1000);
        // SPI - singleByteWrite
        Serial.println("SPI - singleByteWrite");
        singleByteWrite(0x2d,0x03);
        Serial.print("ADDR: ");
        Serial.println(0x2d,HEX);
        Serial.print("Sent Data: ");
        Serial.println(0x03);
        Serial.println();
        delay(1000);








}
