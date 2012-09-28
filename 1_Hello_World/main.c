// Jason Dreisbach
// Jason Descenzo
// Project 1 (Hello World)

#include <avr/io.h>
#include <util/delay.h>
#include "../lib/lcd.h"

int main()
{
	// DDRB |= (1<<PB5);

    lcd_init();
    // _delay_ms(10);
    lcd_printString("hello world");
    lcd_lowerLine();
    lcd_printString("foo bar");
    _delay_ms(900);
    while(1) {
        lcd_clear();
        // Write PB0 high
        lcd_printString("Test");
        // 250 us
        _delay_ms(900);
        lcd_printString(" String");
        // Turn PB0 OFF
        // PORTB &= ~(1<<PB5);
        // lcd_printChar('a');
        // wait for the rest of the second
        _delay_ms(750);
        lcd_clear();
        lcd_printString("hello world");
        _delay_ms(1000);

        
    }

	return 0;
}


