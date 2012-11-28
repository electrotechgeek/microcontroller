// Pin change interrupt tutorial
// Jason Dreisbach
// Jason Descenzo

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//function prototype
void initPCINT0(void);

int main(void)
{
      initPCINT0();

      DDRB |= 1<<PB5; // LED as output

      sei();	 //enable global interrupts

      while(1); //stuck here forever :)

      return 0;

} // end main

void initPCINT0(void)
{
      PCICR  = 0x04;  	// enable PCINT23..16
      PCMSK2 = 0x04; 	// enable PCINT18 interrupt
      PCIFR  = 0x04;  	// clear previous interrupts
}

ISR(PCINT2_vect)
{
      PORTB |= 1<<PB5;
      _delay_ms(500);
      PORTB &= ~(1<<PB5);
}