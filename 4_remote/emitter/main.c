#include <stdlib.h>					// Standard C library
#include <avr/io.h>					// Input-output ports, special registers
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define freq_to_timerval(x) ((F_CPU / x - 1 )/ 2)

typedef struct codeElement
{
  unsigned int onTime;   // duration of "On" time
  unsigned int offTime;  // duration of "Off" time
} codestruct;

struct powercode
{
  uint8_t timer_val; // not the actual frequency, but the timer value to generate the frequency
  struct codeElement codes[];  // flexible number of on/off codes
};

#define pulseCount  150
unsigned int code[] = 
{
	 28723,  2276,
	  1107,   149,
	   131,   149,
	   405,   151,
	   404,   150,
	   406,   149,
	   131,   149,
	   405,   150,
	   405,   150,
	   405,   149,
	   406,   150,
	   405,   149,
	   405,   149,
	   131,   149,
	   130,   151,
	   130,   150,
	   130,   149,
	   405,   150,
	   406,   149,
	   405,   151,
	   405,   149,
	   406,   150,
	   404,   150,
	   130,   150,
	   406,   149,
	   131,   149,
	   406,   149,
	   131,   149,
	   130,   150,
	   405,   149,
	   131,   149,
	   406,   149,
	   131,   149,
	   135,   150,
	  9026,  2268,
	  1107,   150,
	   130,   150,
	   405,   149,
	   406,   150,
	   405,   149,
	   130,   150,
	   405,   150,
	   405,   150,
	   405,   149,
	   406,   150,
	   405,   149,
	   406,   150,
	   130,   150,
	   130,   149,
	   129,   150,
	   130,   149,
	   406,   150,
	   405,   149,
	   131,   149,
	   406,   149,
	   131,   149,
	   131,   149,
	   130,   150,
	   130,   150,
	   130,   149,
	   406,   150,
	   130,   150,
	   130,   149,
	   406,   150,
	   130,   149,
	   405,   151,
	   130,   151,
	   134,   149,
};
/*{
	 46805,   261,
	    84,    99,
	   222,    98,
	    84,   106,
	    78,   106,
	   146,    98,
	    84,    99,
	   289,    98,
	    85,    99,
	    84,    99,
	   219,    99,
	   152,    99,
	   152,    98,
	   152,    99,
	   152,    99,
	   152,   104,
	    78,   106,
	    78,    99,
	    84,    98,
	    84,   105,
	    78,    98,
	   153,    99,
	   220,    99,
	   219,    99,
	    84,    99,
	 22819,   268,
	    78,   105,
	   216,    98,
	    85,    99,
	    84,    99,
	   151,    99,
	    84,   105,
	   282,    98,
	    84,   105,
	    78,   106,
	   213,    99,
	   152,   106,
	   146,    98,
	   152,   106,
	   146,    99,
	   151,    99,
	    84,    99,
	    85,    99,
	    83,    99,
	    84,    99,
	    84,    99,
	   151,   100,
	   220,   106,
	   213,    99,
	    84,    99,
	  1758,   268,
	    78,   106,
	   216,    98,
	    85,    98,
	    84,    99,
	   152,    98,
	    85,    99,
	   289,    99,
	    83,    99,
	    84,   105,
	   213,    98,
	   152,    99,
	   152,    99,
	   151,    99,
	   152,    99,
	   152,    98,
	    84,    99,
	    84,    99,
	    84,    98,
	   152,    99,
	    84,    98,
	   153,    99,
	   221,    99,
	   288,    99,
	    85,   105
};
*/

// USART Stuff
void usart_init(uint16_t baudin, uint32_t clk_speedin);
void usart_send(uint8_t data);
uint8_t usart_recv(void);
uint8_t usart_istheredata(void);

void printString(char *string);
void printLine(char *string);

void startTransmit(void);

#define led_on()  PORTB &= ~_BV(PB5)
#define led_off() PORTB |= _BV(PB5)
#define led_toggle() PORTB ^= _BV(PB5)

uint16_t currentpulse = 0;

int main (void)
{
	DDRB |= _BV(PB5);
	led_off();

	usart_init(9600, 16000000);
	sei();

	int index, i;
	for (;;) {
		usart_recv();
		// if (!(PORTB & _BV(PB5))) {
		// 	printLine("Turning off");
		// 	led_off();
		// }
		// else {
		// 	printLine("TUrning on");
		// 	led_on();

		// }
		printLine("Emmitting Code!");
		startTransmit();
	}

	return 0;
}


void startTransmit(void)
{
	// Timer 1 is the 16 bit timer to time the 
	// on off cycles of the led
	// timer overflow mode
	TCCR1A = 0x00;
	// timer clk = system clk / 64
	TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10);
	TCCR1C = 0x00;
	TCNT1 = 0x000;

	currentpulse = 0;
	OCR1A = code[currentpulse];

	led_off();

	// Output Compare interrupt enable 1 A
	TIMSK1 |= _BV(OCIE1A);
	// clear previous timer overflow
	TIFR1 |= _BV(OCF1A);

	// Timer 0 does the pwm modulation for when the LED is high
	// Enable CTC mode
	TCCR0A = 0x02;
	TCCR0B |= _BV(CS01);
	TCNT0 = 0x00;
	OCR0A = 26;
	TIMSK0 |= _BV(OCIE0A);
	TIFR0 |= _BV(OCF0A);
}

ISR(TIMER0_COMPA_vect)
{
	led_toggle();
}

ISR(TIMER1_COMPA_vect)
{
	currentpulse++;
	if (currentpulse < pulseCount) {
		OCR1A = code[currentpulse];
		TCNT1 = 0x000;
		// Enable the timer0 output compare A
		// check to see if we should be outputting a high or low signal
		if (currentpulse % 2) {
			// Start the pwm
			// led_on();
			TCNT0 = 0x00;
			TIMSK0 |= _BV(OCIE0A);
		}
		else {
			// dont pwm 
			TIMSK0 &= ~_BV(OCIE0A);
			led_off();
		}
	}
	else {
		printLine("Done with code!");
		TIMSK0 &= ~_BV(OCIE0A);
		led_off();
		TIMSK1 &= ~_BV(OCIE1A);
	}
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

// void outputPulses(uint16_t pulses[100][2], uint8_t pulseCount)
// {
// 	uint8_t i;
// 	char number_string[10];
// 	printLine("{");
// 	for (i = 0; i < pulseCount; i++) {
// 		usart_send('\t');
// 		// print the high value
// 		sprintf(number_string, "%6u", pulses[i][0]);
// 	    printString(number_string);
// 		usart_send(',');
// 		// print the low value
// 		sprintf(number_string, "%6u", pulses[i][1]);
// 	    printLine(number_string);
// 	}
// 	printLine("}");
// }

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

/* ,function check to see if there is data to be received
	@return true is there is data ready to be read */
uint8_t  usart_istheredata(void)
{
	 return (UCSR0A & (1<<RXC0));
}
