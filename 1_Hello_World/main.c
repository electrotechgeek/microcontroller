// Jason Dreisbach
// Jason Desenza
// Project 1 (Hello World)

#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRB |= (1<<PB5);
    
    lcd_init();

    // infinite loop
    while(1) {
        // Write PB0 high
        PORTB |= (1<<PB5);
        // 250 us
        _delay_ms(250);
        // Turn PB0 OFF
        PORTB &= ~(1<<PB5);
        // wait for the rest of the second
        _delay_ms(750);
    }

	return 0;
}


