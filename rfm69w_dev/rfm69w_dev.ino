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
    delay(2000); // Wait before entering loop
    RFM.setReg(); // Setup the registers & initial mode for the RFM69
    RFM.modeReceive();
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

void test_Reg()
{
    // Code used to test the register values on the rfm69w
    // Assumes working SPI connection
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

void test_SPI()
{
    test_spiReg();
    test_singleByteRead(0x2d,0x03);
    test_singleByteWrite(0x2d,0x04);
    test_singleByteRead(0x2d,0x04);
    test_singleByteWrite(0x2d,0x03);
    Serial.println();
    
}

void ping()
{
    // Sends test packet #1 from RFM69W
    
    // Load data into FIFO Register
    RFM.singleByteWrite(RegFifo,'H');
    RFM.singleByteWrite(RegFifo,'E');
    RFM.singleByteWrite(RegFifo,'L');
    RFM.singleByteWrite(RegFifo,'L');
    RFM.singleByteWrite(RegFifo,'O');
    RFM.singleByteWrite(RegFifo,'_');
    
}

void ping2()
{
    // Sends test packet #2 from RFM69W
    
    // Load data into FIFO Register
    RFM.singleByteWrite(RegFifo,'W');
    RFM.singleByteWrite(RegFifo,'O');
    RFM.singleByteWrite(RegFifo,'R');
    RFM.singleByteWrite(RegFifo,'L');
    RFM.singleByteWrite(RegFifo,'D');
    RFM.singleByteWrite(RegFifo,'_');
    
}

void ping3()
{
    // Sends test packet #3 from RFM69W
    
    // Load data into FIFO Register
    RFM.singleByteWrite(RegFifo,'1');
    RFM.singleByteWrite(RegFifo,'2');
    RFM.singleByteWrite(RegFifo,'3');
    RFM.singleByteWrite(RegFifo,'4');
    RFM.singleByteWrite(RegFifo,'5');
    RFM.singleByteWrite(RegFifo,'_');
    
}

void listen()
{
    // Listens for an incomming packet via RFM69W
    // Read the Payload Ready bit from RegIrqFlags2 to see if any data
    
    Serial.println("Start Listening: ");
    
    while (RFM.singleByteRead(RegIrqFlags2) & 0x04)
    {
        Serial.print("Rec: ");
        
        Serial.println(RFM.singleByteRead(RegFifo));
    }
    Serial.println("Stop Listening.");
}

void loop()
{
    /*
    // The SPI communication and registers have been set by setup()
    Serial.println("Start: ");
    // The RFM69W should be in Sleep mode.
    // Load bytes to transmit into the FIFO register.
    ping();
    // The data will be sent once the conditions for transmitting have been met.
    // With packet mode and data already in the FIFO buffer this should be met as soon as transmit mode is enabled.
    RFM.modeTransmit();
    // TODO: Add a check to see if packet sent before continuing. 
    // Only 6 bytes of user data being sent at 4.8kbps
    // Use a brief (1 second) delay, this should be enough time to complete send.
    delay(1000);
    RFM.modeSleep(); // Return to Sleep mode.
    //test_SPI();
    //test_Reg();
    Serial.println("End: ");
    delay(15000); // Pause between loops (5 seconds).
    */
    listen();
    
}
