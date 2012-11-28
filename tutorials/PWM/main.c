// PWM tutorial 
// Jason Dreisbach
// Jason Descenzo

#include <avr/io.h>
#include <util/delay.h>

void initPWM();

int main(void)
{
	initPWM();// initialize fast PWM to PD5 and PD6

	// PB2 is an input
	DDRB &= ~_BV(PB2);

	PORTB |= _BV(PB2);
	
	for(;;) {
		if (!(PINB & _BV(PB2))) {
			// change OCR0A & OCR0B
			if (OCR0A > 230)
				OCR0A = 0;
			OCR0A += 25;

			if (OCR0B < 25) 
				OCR0B = 0;
			OCR0B -= 25;
		}
		_delay_ms(500);
	}

	return 0;
}

void initPWM()// initializes motors
{
	DDRD = 0xFF; 			//set port D as outputs
	TCCR0A = 0b10100011; 	//timer set to fast pwm
	TCCR0B = 0x02; 			//timer clk = system clk / 64; 
							//outputs 16E6/64/255 = 980Hz PWM
	OCR0A = 25; 			//compare value => 20% duty cycle to PD6
	OCR0B = 191; 			//compare value => 75% duty cycle to PD5
}