// Project 1 (Hello World)
// Jason Dreisbach
// Jason Descenzo

#include <arduino.h>

int main()
{
    DDRC = 0x00;
    PORTC = 0xFF;
    lcd_init();
    
    while(1) {
        lcd_clear();
        if (PINC & 0x1) {
            lcd_printString("hello world");
        }
        else {
            lcd_printString("ouch!");
        }
        _delay_ms(1000);
    }

	return 0;
}


