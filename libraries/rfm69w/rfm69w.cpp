// rfm69w.cpp - Header file for rfm69w.h
// Library to use the rfm69w transeiver.
// Author: M. Tunstall
// NOTE: This is heavily commented for my own learning/reference.
#include "rfm69w_reg.h"
#include "rfm69w.h"

void RFM69W::setDefaultReg()
{
    // Write the recommended default values into device registers

    /*
    Recommended Defaults

    RegLna         = 0x88
    RegRxBw        = 0x55
    RegAfcBw       = 0x8b
    RegDioMapping2 = 0x07
    RegRssiThresh  = 0xe4

    RegSyncValue1  = 0x01
    RegSyncValue2  = 0x01
    RegSyncValue3  = 0x01
    RegSyncValue4  = 0x01
    RegSyncValue5  = 0x01
    RegSyncValue6  = 0x01
    RegSyncValue7  = 0x01
    RegSyncValue8  = 0x01

    RegFifoThresh  = 0x8f
    RegTestDagc    = 0x30

    */
    singleByteWrite(RegLna,0x88);
    singleByteWrite(RegRxBw,0x55);
    singleByteWrite(RegAfcBw,0x8b);
    singleByteWrite(RegDioMapping2,0x07);
    singleByteWrite(RegRssiThresh,0xe4);
    singleByteWrite(RegSyncValue1,0x01);
    singleByteWrite(RegSyncValue2,0x01);
    singleByteWrite(RegSyncValue3,0x01);
    singleByteWrite(RegSyncValue4,0x01);
    singleByteWrite(RegSyncValue5,0x01);
    singleByteWrite(RegSyncValue6,0x01);
    singleByteWrite(RegSyncValue7,0x01);
    singleByteWrite(RegSyncValue8,0x01);
    singleByteWrite(RegFifoThresh,0x8f);
    singleByteWrite(RegTestDagc,0x30);

};
void RFM69W::setNodeAdr(uint8_t Adr)
{
    singleByteWrite(RegNodeAdrs, Adr);
};

void RFM69W::setCustomReg()
{
    // Write Custom Setup Values to registers

    // TODO: Move this out of library into application code.

    // Data Modulation
    // - Packet Mode, OOK, No Shaping
    singleByteWrite(RegDataModul,0x08);

    // DIO0 Mapping - Starup value, want to change during operation
    //              - depending on mode
    singleByteWrite(RegDioMapping1,0x00); // TODO: Confirm best initial state.


    // Packet Config - Fixed Length 8 bytes
    //singleByteWrite(RegDataModul,0x10); // Defines Fixed Packet (Default)
    singleByteWrite(RegPayloadLength,0x08); // Set Fixed Packet Length to 8 bytes.

    // TODO: Enable Carrier Frequency to be adjusted, currently hardcoded.

    // Set Carrier Frequency to 867,999,975.2 Hz
    singleByteWrite(RegFrfMsb,0xd9);
    singleByteWrite(RegFrfMid,0x00);
    singleByteWrite(RegFrfLsb,0x24);

    // Set DIO4/5, Disable Clk Out - None of these currently used/connected
    singleByteWrite(RegDioMapping2,0x07);

};

void RFM69W::calOsc()
{
    // TODO: Calibrate RC Oscillator
    singleByteWrite(RegOsc1,0x80); // Trigger cal operation
    // Reg will read 0x01 whilst cal in progress.
    // Reg will read 0x41 when cal complete.
};

void RFM69W::setReg()
{
    // Wrapper for register setup
    setDefaultReg();
    setCustomReg();
    modeSleep(); // Set to sleep mode
};

uint8_t RFM69W::singleByteRead(uint8_t byteAddr)
{
    // Read a single byte from an address over the SPI interface
    uint8_t readByte; // Place to store returned data
    SPI.SelectSlave();// Set Slave Select line low
    SPI.Transiever(byteAddr); // Request data from address
    readByte = SPI.Transiever(0); // Pad the shift register to get data.
    SPI.DeselectSlave();//Set Slave Select line high
    return readByte;
};

void RFM69W::singleByteWrite(uint8_t byteAddr, uint8_t dataByte)
{
    // Write a single byte to an address over the SPI interface.
    SPI.SelectSlave();// Slave Select line low
    // In this instance we are not handling any data the slave returns.
    // To write, the MSB of the address must be 1; MSB 0 to read.
    SPI.Transiever(byteAddr|0x80); // Send the address byte.
    SPI.Transiever(dataByte);   // Send the data byte.
    SPI.DeselectSlave(); // Slave Select line high
    return;
};

void RFM69W::modeTransmit()
{   // Set Op Mode
    singleByteWrite(RegOpMode,0x0C);
    // Set DIO Map
    // DIO0 will indicate PacketSent in Tx Mode
    singleByteWrite(RegDioMapping1,0x00);
};

void RFM69W::modeReceive()
{
    // Set Op Mode
    singleByteWrite(RegOpMode,0x10);
    // Set DIO Map
    // DIO0 will indicate PayloadReady in Rx Mode
    singleByteWrite(RegDioMapping1,0x40);
};

void RFM69W::modeSleep()
{
    singleByteWrite(RegOpMode,0x00);
};

void RFM69W::modeStandby()
{
    singleByteWrite(RegOpMode,0x04);
};
