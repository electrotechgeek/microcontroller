// Jason Dreisbach
// Jason Descenzo
// Project 1 (Library)

#include <avr/io.h>
#include <util/delay.h>

void lcd_write(unsigned char data)
{
	//Set E high
	PORTD |= (1<<4);
	//Write data to bus
	
	PORTB &= 0xE0;
	PORTB |= (data & (1<<0)) << 4;
	PORTB |= (data & (1<<1)) << 3;
	PORTB |= (data & (1<<2)) << 2;
	PORTB |= (data & (1<<3)) << 1;
	PORTB |= (data & (1<<4)) << 0;

	PORTD &= 0x03;
	PORTD |= (data & (1<<5)) << 7;
	PORTD |= (data & (1<<6)) << 6;
	PORTD |= (data & (1<<7)) << 5;

	_delay_us(1);
	//Set E low
	PORTD &= ~(1<<4);
}

void lcd_cmd(unsigned char cmd)
{	
	// Set RS low 
	PORTD &= ~(1<<2);
	lcd_write(cmd);
	/// Do we need to turn it
}

void lcd_on()
{
	lcd_cmd(0x0C);
}

void lcd_off()
{
	lcd_cmd(0x08);
}

void lcd_printChar(char character)
{
	// set RS high
	PORTD |= (1<<2);
	lcd_write(character);
	_delay_us(50);
}

void lcd_printString(char *string)
{
	int i = 0;
	for (; string[i] != '\0';) {
		lcd_printChar(string[i++]);
	}
}

void lcd_init()
{	
	DDRB |= 0xFC; 
	DDRD |= 0x1F;
	//Enable low 
	PORTD &= ~(1<<4);
	_delay_ms(15);
	lcd_cmd(0x38);
	_delay_us(1);
}