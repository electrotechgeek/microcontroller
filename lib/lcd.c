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
	
	unsigned char pb = PORTB & 0xE0;
	pb |= data >> 3;
	PORTB = pb;
	
	unsigned char pd = PORTD & 0x1F;
	pd |= (data & 0x07) << 5;
	PORTD = pd;

	_delay_us(10);
	//Set E low
	PORTD &= ~(1<<4);
}

void lcd_cmd(unsigned char cmd)
{	
	// Set RS low 
	PORTD &= ~(1<<2);
	lcd_write(cmd);
	_delay_ms(15);
}

void lcd_lowerLine()
{
	lcd_cmd(0xC0);
}

void lcd_upperLine()
{
	lcd_cmd(0x80);
}

void lcd_on()
{
	lcd_cmd(0x0F);
}

void lcd_off()
{
	lcd_cmd(0x08);
}

void lcd_clear()
{
	lcd_cmd(0x01);
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
	_delay_ms(50);
	DDRD |= 0xFC; 
	DDRB |= 0x1F;
	//Enable low 
	PORTD &= ~(1<<4);

	lcd_cmd(0x3C);
	
	lcd_on();

	lcd_clear();

	// Set entry mode
	lcd_cmd(0x06);


	// lcd_off();
	// _delay_ms(15);
	// lcd_clear();
	// _delay_ms(15);
	// lcd_cmd(0x06);
	// _delay_ms(15);
	// lcd_on();
	
}