// Message recorder
#include <stdlib.h>					// Standard C library
#include <avr/io.h>					// Input-output ports, special registers
#include <avr/interrupt.h>
#include <stdio.h>
#include "usart.h"


int status = 0;
uint8_t timeoutCounter = 0;

uint16_t pulses[255][2];
uint8_t pulseIndex = 0;

void outputPulses(uint16_t pulses[255][2], uint8_t pulseCount)
{
	uint8_t i;
	char number_string[10];
	for (i = 0; i < pulseCount; i++) {
		// print the high value
		sprintf(number_string, "%6u,", pulses[i][0]);
	    printString(number_string);
		// print the low value
		sprintf(number_string, "%6u,", pulses[i][1]);
	    printString(number_string);
	}
	printLine("|");
}

ISR(TIMER1_COMPA_vect)
{
	if (pulseIndex != 0) {
		// set status bit
		status = pulseIndex;
	}
	else {
		timeoutCounter++;
		// If we've been waiting for over 10 seconds
		// 10 seconds/ .25 seconds = 40
		if (timeoutCounter > 40) {
			status = -1;
		}
	}
	pulseIndex = 0;
}

//     0,0     0,1       1,0       1,1
// [highTime][lowTime][highTime][lowTime]
ISR(PCINT2_vect)
{
	// Elapsed time is the pulse length
	uint16_t pulseLength = TCNT1;

	// If the detector reads a high
	if (bit_is_clear(PIND, PD2)) {
		pulses[pulseIndex][0] = pulseLength;
	}
	else {
		pulses[pulseIndex][1] = pulseLength;
		pulseIndex++;
	}
	// Reset the counter
	TCNT1 = 0x000;
}

void initRecorder(void)
{
	DDRD &= ~_BV(PD2);
	status = 0;
	timeoutCounter = 0;

	// timer overflow mode
	TCCR1A = 0x00;
	// timer clk = system clk / 64
	TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10);
	TCCR1C = 0x00;
	// 16 Mhz / 64 / 62500 = 4 Hz => .25 s 
	OCR1A = 62500;
	TCNT1 = 0x000;
	// timer overflow interrupt enabled
	TIMSK1 |= _BV(OCIE1A);
	// clear previous timer overflow
	TIFR1 |= _BV(OCF1A);

	// Enable the pin change interrupt
	PCICR  |= 0x04;  	// enable PCINT23..16
	PCMSK2 |= 0x04; 	// enable PCINT18 interrupt
    PCIFR  |= 0x04;  	// clear previous interrupts
}

void recordCode()
{
	initRecorder();

	// wait for the code to come in
	for (;status == 0;);


	if (status < 0) {
		// We've timed out or something went wrong
		printLine("err");
	}
	else {
		outputPulses(pulses, pulseIndex);
	}

	// turn of pin change interrupt
	PCICR &= ~ 0x04;
	// turn off timer 1
	TIMSK1 &= ~_BV(OCIE1A);
}


