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

void test_singleByteRead(uint8_t byteAddr, uint8_t byteExpect)
{
    // SPI - singleByteRead
    Serial.println("SPI - singleByteRead");
    uint8_t datain = singleByteRead(byteAddr);
    Serial.print("ADDR: ");
    Serial.println(byteAddr,HEX);
    Serial.print("Expected Data: ");
    Serial.println(byteExpect,HEX);
    Serial.print("Actual Data: ");
    Serial.println(datain,HEX);
    Serial.println();
    delay(1000);
}

void test_spiReg()
{
    //Print the SPI Registers to Serial Output
    Serial.println("SPI Registers");
    Serial.print("SPCR: ");
    Serial.println(SPCR,BIN);
    Serial.print("SPSR: ");
    Serial.println(SPSR,BIN);
    Serial.println();
    delay(1000);
}

void test_singleByteWrite(uint8_t byteAddr, uint8_t dataByte)
{
    // SPI - singleByteWrite
    Serial.println("SPI - singleByteWrite");
    singleByteWrite(byteAddr,dataByte);
    Serial.print("ADDR: ");
    Serial.println(byteAddr,HEX);
    Serial.print("Sent Data: ");
    Serial.println(dataByte);
    Serial.println();
    delay(1000);
}

void loop()
{

    test_spiReg();
    test_singleByteRead(0x2d,0x03);
    test_singleByteWrite(0x2d,0x04);
    test_singleByteRead(0x2d,0x04);
    test_singleByteWrite(0x2d,0x03);
}
