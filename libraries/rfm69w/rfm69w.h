// rfm69w.h - Header file for rfm69w.cpp
// Library to use the rfm69w transeiver.
// Author: M. Tunstall
// NOTE: This is heavily commented for my own learning/reference.
#include "spi.h"
#include <stdint.h> // Enable fixed width integers.
#include "rfm69w_reg.h" // Register Addresses as a separate file

#ifndef RFM69W_H // Header guards
#define rfm69w_h
#define RFM69W_H

class RFM69W
{
    protected: // accessed by member functions and derived classes.
        Spi& SPI; // Reference to an instance of the SPI interface.

    private: // accessed by member functions but not derived classes.
        void setDefaultReg(); // Load Register Recommended Defaults
        void setCustomReg();  // Load Custom Register Settings
    public: // accessed by anybody.
        RFM69W(Spi& SPIx) : SPI(SPIx) //Default constructor
        {
            SPI.InitMaster(); // Initialise as Master SPI Node
            SPI.SetClock(1); // Change the SPI Clock rate.
            SPI.EnableSPI(); // Enable SPI Communication.
            // DEV NOTE: It doesn't seem possible to load the default
            // register values from within the constructor.
        }
        ~RFM69W(){}  // Destructor
        void setReg(); // Wrapper to load Custom & Recommended Defaults
        uint8_t singleByteRead(uint8_t byteAddr);
        void singleByteWrite(uint8_t byteAddr, uint8_t dataByte);
        void setNodeAdr(uint8_t Adr); // Set the Node Address to be included with data packet.
        void calOsc(); // Calibrate RC Oscillator

        // TODO: Add function to adjust transmit power.

        // TODO: Add function to change to transmit mode.
        void modeTransmit();
        // TODO: Add function to change to receive mode,
        void modeReceive();
        // TODO: Add function to change to sleep mode.
        void modeSleep();
        // TODO: Add function to change to standby mode.
        void modeStandby();

};


#endif //RFM69W_H
