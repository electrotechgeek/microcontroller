// Emitter

#include <stdlib.h>					// Standard C library
#include <avr/io.h>					// Input-output ports, special registers
#include <avr/interrupt.h>
#include "usart.h"

uint8_t send_status = 0;
uint8_t currentpulse = 0;

unsigned int *code;
uint8_t code_length;

#define led_on()  PORTB &= ~_BV(PB5)
#define led_off() PORTB |= _BV(PB5)
#define led_toggle() PORTB ^= _BV(PB5)

ISR(TIMER0_COMPA_vect)
{
	led_toggle();
}

ISR(TIMER1_COMPB_vect)
{
	currentpulse++;
	if (currentpulse < code_length) {
		OCR1B = code[currentpulse];
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
		send_status = code_length;
		TIMSK0 &= ~_BV(OCIE0A);
		led_off();
		TIMSK1 &= ~_BV(OCIE1A);
	}
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
	OCR1B = code[currentpulse];

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

void emitCode()
{
	char buffer[12];

	uint8_t c, codeIndex = 0, bufferIndex = 0;
	// read data from the usart
	
	code_length = usart_recv();
	code = malloc(code_length * sizeof(unsigned int));

	for (;;) {
		for (; codeIndex < code_length;) {
			c = usart_recv();
			if(c == ',' && bufferIndex > 0) {
				buffer[bufferIndex] = '\0';
				
				sscanf(buffer, "%u", &code[codeIndex]);
				codeIndex++;
				bufferIndex = 0;
			}
			else if (bufferIndex > 11) {
				bufferIndex = 0;
			}
			else {
				buffer[bufferIndex] = c;
				bufferIndex++;
			}
		}
	}

	if (code_length > 0) {
		send_status = 0;
		startTransmit();
		for (;send_status == 0;);
	}


	free(code);
	code = NULL;
}