#include <stdlib.h>					// Standard C library
#include <avr/io.h>					// Input-output ports, special registers
#include <util/delay.h>
#include <stdio.h>
#include <string.h>


// USART Stuff
void usart_init(uint16_t baudin, uint32_t clk_speedin);
void usart_send(uint8_t data);
uint8_t usart_recv(void);
uint8_t usart_istheredata(void);

void printString(char *string);
void printLine(char *string);

void outputPulses(uint16_t pulses[100][2], uint8_t pulseCount);

#define RESOLUTION 20 // us
#define MAXPULSE 65000 // ms

int main (void)
{
	DDRD &= ~_BV(PD2);

	uint16_t pulses[100][2];
	uint8_t currentpulse = 0;
	uint8_t didTimeout;
	uint16_t highpulse = 0, lowpulse = 0;
	usart_init(9600, 16000000);


	for (;;) {
		didTimeout = 0;
		highpulse = 0;
		lowpulse = 0;

		while (PIND & _BV(PD2)) {
			highpulse++;
			_delay_us(RESOLUTION);

			if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
				outputPulses(pulses, currentpulse);
				currentpulse = 0;
				didTimeout = 1;
				break;
			}
		}
		// reset
		if (didTimeout) continue;

		pulses[currentpulse][0] = highpulse;

		while (! (PIND & _BV(PD2)) ) {
			lowpulse++;
			_delay_us(RESOLUTION);

			if ((lowpulse >= MAXPULSE) && (currentpulse != 0)) {
				outputPulses(pulses, currentpulse);
				currentpulse = 0;
				didTimeout = 1;
				break;
			}
		}
		if (didTimeout) continue;
		pulses[currentpulse][1] = lowpulse;
		
		currentpulse++;
	}

	return 0;
}


void printString(char *string)
{
	int i = 0;
	size_t len = strlen(string);
	for (i = 0; i < len; i++) {
		usart_send(string[i]);
	}
}

void printLine(char *string)
{
	printString(string);
	usart_send('\r');
	usart_send('\n');
}

void outputPulses(uint16_t pulses[100][2], uint8_t pulseCount)
{
	uint8_t i;
	char number_string[10];
	printLine("{");
	for (i = 0; i < pulseCount; i++) {
		usart_send('\t');
		// print the high value
		sprintf(number_string, "%6u", pulses[i][0]);
	    printString(number_string);
		usart_send(',');
		// print the low value
		sprintf(number_string, "%6u", pulses[i][1]);
	    printLine(number_string);
	}
	printLine("}");
}

//======================================================================================================
// UART
//======================================================================================================

void usart_init(uint16_t baudin, uint32_t clk_speedin)
{
	uint32_t ubrr = (clk_speedin/16UL)/baudin-1;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*UBRR0H = (BAUD_PRESCALE>>8);
	UBRR0L = BAUD_PRESCALE;*/
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	// UCSR0C = (0<<USBS0)|(3<<UCSZ00);
	/* Set frame format: 8data, 2stop bits */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	UCSR0A &= ~(1<<U2X0);
}

/*the send function will put 8bits on the trans line. */
void usart_send( uint8_t data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

/* the receive data function. Note that this a blocking call
Therefore you may not get control back after this is called 
until a much later time. It may be helpfull to use the 
istheredata() function to check before calling this function
	@return 8bit data packet from sender
*/
uint8_t usart_recv(void)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

/* function check to see if there is data to be received
	@return true is there is data ready to be read */
uint8_t  usart_istheredata(void)
{
	 return (UCSR0A & (1<<RXC0));
}
