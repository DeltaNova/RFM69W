// spi.h - Header file for spi.cpp
// Serial Periferal Interface for Atmega328 using the default portB.
// Author: M. Tunstall
// NOTE: This is heavily commented for my own learning/reference.

#ifndef SPI_H //If spi.h hasn't been included elsewhere, define it now.
#define spi_h 

#include <avr/io.h> // Needed for register references
//#include <cstdint> // Fixed width integer support (C++11)
#include <stdint.h> // Fixed width integer support (Arduino IDE 1.0.5 Compatable)
/*----------------------------------------------------------------------

SPCR - SPI Control Register
SPCR = SPIE,SPE,DORD,MSTR,CPOL,CPHA,SPR1,SPR0

SPSR - SPI Status Register
SPSR = SPIF,WCOL,-,-,-,-,-,SPI2X

SPDR - SPI Data Register
SPDR = MSB,-,-,-,-,-,-,LSB

DDRB - Data Direction Register for PortB
DDRB = DDB7,DDB6,DDB5(SCK),DDB4(MISO),DDB3(MOSI),DDB2(SS),DDB1,DDB0
----------------------------------------------------------------------*/
namespace Spi // Namespace definition
{
    class Spi
    {
        // private members listed first as class members are private by 
        // default (without the access specifier).
        private: // Can only be accessed by functions in this class
            
            
        public: // Can be accessed by any function in the program
            void InitMaster() 
            { 
                SPCR |= (1<<MSTR);
                // Set SCK,MOSI,SS as outputs
                DDRB |= (1<<DDB5)|(1<<DDB3)|(1<<DDB2);
                // Set MISO as input
                DDRB &= ~(1<<DDB4);
                // Set SS line HIGH (slave not enabled)
                PORTB |= (1<<PORTB2);
            } 
            void InitSlave() 
            { 
                SPCR &= ~(1<<MSTR);
                //Set SCK,MOSI,SS as inputs
                DDRB &= ~((1<<DDB5)|(1<<DDB3)|(1<<DDB2));
                // Set MISO as output
                DDRB |= (1<<DDB4);
            } 
            void InterruptEnable() { SPCR |= (1<<SPIE); }
            void InterruptDisable() { SPCR &= ~(1<<SPIE); }
            void EnableSPI() { SPCR |= (1<<SPE); }
            void DisableSPI() { SPCR &= ~(1<<SPE); }
            void MSBFirst() { SPCR |= (1<<DORD); }
            void LSBFirst() { SPCR &= ~(1<<DORD); }
            void SelectSlave() { PORTB &= ~(1<<PORTB2); }
            void DeselectSlave() { PORTB |= (1<<PORTB2); }
            int8_t SetMode(int8_t Mode)
            {
                /* 
                   Sets one of the SPI Modes below.
                   Returns 1 if error. 
                   
                   SPI Modes
                    0 - CPOL=0, CPHA=0
                    1 - CPOL=0, CPHA=1
                    2 - CPOL=1, CPHA=0
                    3 - CPOL=1, CPHA=1
                */
                int8_t return_code = 0;
                
                switch(Mode)
                {
                    case 0:
                        SPCR &= ~((1<<CPOL) | (1<<CPHA));
                        break;
                    case 1:
                        SPCR &= ~(1<<CPOL);
                        SPCR |= (1<<CPHA);
                        break;
                    case 2:
                        SPCR |= (1<<CPOL);
                        SPCR &= ~(1<<CPHA);
                        break;
                    case 3:
                        SPCR |= (1<<CPOL)|(1<<CPHA);
                        break;
                    default:
                        return_code = 1;
                }
                return return_code;
            }
            int8_t SetClock(int8_t ClockMode)
            {
                /*
                 fosc - frequency of crystal/resonator of uC. 
                 
                 Mode - Clock Frequency
                 0          fosc/4 (default)
                 1          fosc/16
                 2          fosc/64
                 3          fosc/128
                 4          fosc/2
                 5          fosc/8
                 6          fosc/32
                 7          fosc/64
                  
                */
                int8_t return_code = 0;
                switch(ClockMode)
                {
                    case 0: // fosc/4
                        SPSR &= ~(1<<SPI2X);
                        SPCR &= ~((1<<SPR1)|(1<<SPR0));
                    case 1: // fosc/16
                        SPSR &= ~(1<<SPI2X);
                        SPCR &= ~(1<<SPR1);
                        SPCR |= (1<<SPR0);
                    case 2: // fosc/64
                        SPSR &= ~(1<<SPI2X);
                        SPCR |= (1<<SPR1);
                        SPCR &= ~(1<<SPR0);
                    case 3: // fosc/128
                        SPSR &= ~(1<<SPI2X);
                        SPCR |= (1<<SPR1)|(1<<SPR0);
                    case 4: // fosc/2
                        SPSR |= (1<<SPI2X);
                        SPCR &= ~((1<<SPR1)|(1<<SPR0));
                    case 5: // fosc/8
                        SPSR |= (1<<SPI2X);
                        SPCR &= ~(1<<SPR1);
                        SPCR |= (1<<SPR0);
                    case 6: // fosc/32
                        SPSR |= (1<<SPI2X);
                        SPCR |= (1<<SPR1);
                        SPCR &= ~(1<<SPR0);
                    case 7: // fosc/64
                        SPSR |= (1<<SPI2X);
                        SPCR |= (1<<SPR1)|(1<<SPR0);
                    default:
                        return_code = 1;
                }
                return return_code;
            }
        
    };
}
#endif // End of ifndef SPI_H
