// rfm69w_dev.ino - An Arduino script to develop with RFM69W
// ---------------------------------------------------------------------
// Author: M. Tunstall
// NOTE: This is heavily commented for my own learning/reference.

#include "spi.h" // Include my spi library.
#include "rfm69w.h" // Include my rfm69w library
#include <Wire.h> // Used for serial comms.
#include <stdint.h> // Enable fixed width integers.
#include "rfm69w_reg.h" // Register reference for rfm69w

//Spi SPI;   // Create Global instance of the Spi Class
RFM69W RFM; // Create Global instance of RFM69W Class
void setup()
{
    Serial.begin(19200); // Setup Serial Comms
//    SPI.InitMaster(); // Initialise as Master SPI Node
//    SPI.SetClock(1); // Change the SPI Clock rate.
//    SPI.EnableSPI(); // Enable SPI Communication.
}



void test_singleByteRead(uint8_t byteAddr, uint8_t byteExpect)
{
    // SPI - singleByteRead
    Serial.println("SPI - singleByteRead");
    uint8_t datain = RFM.singleByteRead(byteAddr);
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
    RFM.singleByteWrite(byteAddr,dataByte);
    Serial.print("ADDR: ");
    Serial.println(byteAddr,HEX);
    Serial.print("Sent Data: ");
    Serial.println(dataByte);
    Serial.println();
    delay(1000);
}

void loop()
{
    delay(2000);
    RFM.setReg();
    test_spiReg();
    test_singleByteRead(0x2d,0x03);
    test_singleByteWrite(0x2d,0x04);
    test_singleByteRead(0x2d,0x04);
    test_singleByteWrite(0x2d,0x03);
    Serial.println();
    Serial.println("Check Reg Init");
    test_singleByteRead(RegLna,0x88);
    test_singleByteRead(RegRxBw,0x55);
    
    test_singleByteRead(RegAfcBw,0x8b);
    test_singleByteRead(RegDioMapping2,0x07);
    test_singleByteRead(RegRssiThresh,0xe4);
    test_singleByteRead(RegSyncValue1,0x01);
    test_singleByteRead(RegSyncValue2,0x01);
    test_singleByteRead(RegSyncValue3,0x01);
    test_singleByteRead(RegSyncValue4,0x01);
    test_singleByteRead(RegSyncValue5,0x01);
    test_singleByteRead(RegSyncValue6,0x01);
    test_singleByteRead(RegSyncValue7,0x01);
    test_singleByteRead(RegSyncValue8,0x01);
    test_singleByteRead(RegFifoThresh,0x8f);
    test_singleByteRead(RegTestDagc,0x30);
    Serial.println("Check Custom Reg Init");
    test_singleByteRead(RegDataModul,0x08);
    
}
