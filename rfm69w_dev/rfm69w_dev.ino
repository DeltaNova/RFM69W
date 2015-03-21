// rfm69w_dev.ino - An Arduino script to develop with RFM69W
// ---------------------------------------------------------------------
// Author: M. Tunstall
// NOTE: This is heavily commented for my own learning/reference.

#include "spi.h" // Include my spi library.
#include "rfm69w.h" // Include my rfm69w library
#include <Wire.h> // Used for serial comms.
#include <stdint.h> // Enable fixed width integers.
#include "rfm69w_reg.h" // Register reference for rfm69w
#include <avr/interrupt.h> // Required for interrupts.

// Function Declarations
void setup_int();
void listen();
void setup_mode();

//Spi SPI;   // Create Global instance of the Spi Class
RFM69W RFM; // Create Global instance of RFM69W Class
uint8_t intFlag = 0x00; // Setup a flag for monitoring the interrupt.
uint8_t mode = 0x00; // Node startup mode. Rx Default.
void setup()
{
    //Set PB0 as Tx/Rx Mode select input
    DDRB &= ~(1<<DDB0);
    // No internal pullup on PB0, hardwired to VCC (Tx) or GND (Rx).
    PORTB &= ~(1<<PORTB0);
    Serial.begin(19200); // Setup Serial Comms
    delay(2000); // Wait before entering loop
    RFM.setReg(); // Setup the registers & initial mode for the RFM69
    setup_mode(); // Determine the startup mode from status of PB0.
    setup_int(); // Setup Interrupts
}
void setup_mode()
{
    // Configure the node startup mode as a Tx or Rx.

    if (PINB & (1<<PINB0))
    {
        // Tx Mode Selected
        mode = 0xff; // Change node mode
        Serial.println("Tx Mode"); //DEBUG: Print "Tx Mode"
        // RFM69W configured to startup in sleep mode and will wake to
        // transmit as required.
        // TODO: Check interrupt settings / DIO0 map for sleep mode

    }
    else
    {
        // Rx Mode Selected
        Serial.println("Rx Mode"); //DEBUG: Print "Rx Mode"
        RFM.modeReceive();
    }
    return;
}

void setup_int()
{
    // Set PB1 as interrupt input.
    DDRB &= ~(1<<DDB1);

    // Not using internal pullup on PB1 as we want to trigger from a logic 1
    // RFM69W DIO0 is logic 0 until set. Pull down resistor not used.
    PCICR |= (1<<PCIE0); // Enable PCMSK0 covering PCINT[7:0]
    PCMSK0 |= (1<<PCINT1); // Set the mask to only interrupt on PCINT1
    sei(); // Enable interrupts
}

ISR (PCINT0_vect) // PCINT0 is vector for PCINT[7:0]
{
    // Dev Note: Serial.println() commands cannot be used within an interrupt vector.
    /*
        The ISR will set a flag that can be tested by the main loop.
        The interrupt is triggered by DIO0 on RFM69W.
        Setting a local flag via this interrupt allows the monitoring of
        RFM69W without the need to constantly read the register statuses
        over the SPI bus.
    */
    intFlag = 0xff; // Set interrupt flag.
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
    intFlag = 0x00; // Reset interrupt flag
}

void transmit()
{
    // The SPI communication and registers have been set by setup()
    Serial.println("Start: "); // DEBUG: Print "Start: " Start of Tx.
    // The RFM69W should be in Sleep mode.
    // Load bytes to transmit into the FIFO register.
    ping();  // TODO: Add code to send a different packet.
    // The data will be sent once the conditions for transmitting have been met.
    // With packet mode and data already in the FIFO buffer this should be met as soon as transmit mode is enabled.
    RFM.modeTransmit();
    // TODO: Add a check to see if packet sent before continuing.
    // Only 6 bytes of user data being sent at 4.8kbps
    // Use a brief (1 second) delay, this should be enough time to complete send.
    delay(1000); // TODO: Need to add a check for end of transmit, will be quicker than needing a delay.
    RFM.modeSleep(); // Return to Sleep mode.
    Serial.println("End: ");  // DEBUG: Print "End: " End of Tx.

}
void transmitter()
{
    // Transmitter Node Loop
    while (1)
    {
        transmit();
        delay(15000); // Transmit a packet every 15 seconds.
    }
}

void receiver()
{
    // Receiver Node Loop
    while (1)
    {
        // If the interrupt flag has been set then listen for incomming data.
        while (intFlag == 0xff)
        {
            listen();
        }
        //Serial.println("Loop Wait"); // DEBUG: Print "Loop Wait"
    }
}

void loop()
{
    //test_SPI(); // DEBUG: Test SPI Comms
    //test_Reg(); // DEBUG: Test RFM69W Register Values

    if (mode == 0xff) // If node configured as a Transmitter.
    {
        // Run transmitter node loop
        transmitter();
    }
    else // If node configured as a Receiver.
    {
        // Run receiver node loop
        receiver();
    }
}
