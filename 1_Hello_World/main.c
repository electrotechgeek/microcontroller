// Jason Dreisbach
// Jason Desenza
// Project 1 (Hello World)

#include <avr/io.h>
#include <util/delay.h>

void lcd_cmd(unsigned char cmd);
void lcd_init();

void lcd_cmd(unsigned char cmd)
{	
	// Set RS low 
	PORTB &= ~(1<<PB4);
	//Set E high
	PORTB |= (1<<PB2);
	//Write data to bus
	unsigned char lower_six = (cmd<<2);
	unsigned char upper_two = ((cmd & 0xC0)>>6);
	PORTD = lower_six | (PORTD & 0x03);
	PORTB = upper_two | (PORTB & 0xFC);
	_delay_us(1);
	//Set E low
	PORTB &= ~(1<<PB2);

}

void lcd_init()
{	
	DDRB |= 0xFC; 
	DDRD |= 0x1F;
	PORTB &= ~(1<<PB2);
	_delay_ms(15);
	lcd_cmd(0x38);
	_delay_us(1);
}


int main()
{
	DDRB |= (1<<PB5);
    
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


