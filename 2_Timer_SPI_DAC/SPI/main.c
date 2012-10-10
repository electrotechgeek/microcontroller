// Assignment 1 (Timers and Interrupts)
// Jason Dreisbach
// Jason Descenzo

#include <arduino.h>

#define MOSI 3  // PB pin 3
#define SCK  5  // PB pin 1
#define SS   2  // PB pin 2

#include <avr/io.h>
#include <util/delay.h>

void Initialize_SPI_Master(void);
void Transmit_SPI_Master(char Data);


int main(void)
{

    DDRB = 1<<MOSI | 1<<SCK | 1<<SS;	// make MOSI, SCK and SS outputs

    Initialize_SPI_Master();

    while(1)
    {
     	Transmit_SPI_Master(0xF0);	
   		_delay_us(100);
			
        Transmit_SPI_Master(0x0F); 	
    	_delay_us(100);

    }  // end while
    return 0;
}  // end main


void Initialize_SPI_Master(void)
{      
    SPCR = (0<<SPIE) | 		//No interrupts
    (1<<SPE) | 				//SPI enabled
    (0<<DORD) | 			//shifted out LSB
    (1<<MSTR) | 			//master
    (0<<CPOL) | 			//rising leading edge   
    (1<<CPHA) | 			//sample leading edge   
    (0<<SPR1) | (0<<SPR0) ; //clock speed   
    SPSR = (0<<SPIF) | 		//SPI interrupt flag
    (0<<WCOL) | 			//Write collision flag
    (0<<SPI2X) ; 			//Doubles SPI clock
    PORTB = 1 << SS;  		// make sure SS is high


}

void Transmit_SPI_Master(char Data)
{   
	PORTB &= ~(1 << SS);			// assert the slave select
	SPDR = Data;     			// Start transmission, send high byte first
 	while (!(SPSR & (1<<SPIF)));// Wait (poll) for transmission complete
	PORTB |= (1 << SS);			// deassert the slave select
}