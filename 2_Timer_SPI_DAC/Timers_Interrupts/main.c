// Assignment 1 (Timers and Interrupts)
// Jason Dreisbach
// Jason Descenzo

#include <arduino.h>

//function prototype
void initTimer1K(void);
void initTimer10K(void);
// global variable declaration!
volatile unsigned char myCounter;

int main(void)
{
	myCounter = 1;
	initTimer10K();

	DDRB |= 1<<PB5;

	//enable global interrupts
	sei();

	for(;;) { 
		//stuck here forever :)
	} 
	return 0;

}

void initTimer1K(void)
{
	// timer overflow mode (CTC mode)
	TCCR0A = 0x00;
	// timer clk = system clk / 8
	TCCR0B = 0x02;
	// clear previous timer overflow
	TIFR0 = 0x01;
	// Set output compare register A
	OCR0A = 25;
	// timer overflow interrupt enabled
	TIMSK0 = 0x02;
}


void initTimer10K(void)
{
	// timer overflow mode
	TCCR0A = 0x02;
	// timer clk = system clk / 8
	TCCR0B = 0x02;
	// clear previous timer overflow
	TIFR0 = 0x02;
	// Set output compare register A
	OCR0A = 25;
	// timer overflow interrupt enabled
	TIMSK0 = 0x02;
}


ISR(TIMER0_COMPA_vect)
{
	// This is where the 8 comes frome
	if (myCounter == 8) { 
		myCounter = 0;
		PORTB &= ~(1<<PB5);
	}
	else if (myCounter == 4) {
		PORTB |= 1<<PB5;
	}
	myCounter++;
}