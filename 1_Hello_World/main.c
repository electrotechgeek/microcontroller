// Jason Dreisbach
// Jason Descenzo
// Project 1 (Hello World)

#include <avr/io.h>
#include <util/delay.h>
#include "../lib/lcd.h"

int main()
{
	// DDRB |= (1<<PB5);
    DDRC = 0x00;
    PORTC = 0xFF;
    lcd_init();
    // _delay_ms(10);
    
    while(1) {
        lcd_clear();
        if (PINC & 0x1) {
            lcd_printString("hello world");
        }
        else {
            lcd_printString("ouch!");
        }
        _delay_ms(500);
    }

	return 0;
}


