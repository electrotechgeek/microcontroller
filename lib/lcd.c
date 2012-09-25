// LCD library implementation
//

#include <avr/io.h>
#include <util/delay.h>

void lcd_write(unsigned char data);
void lcd_cmd(unsigned char cmd);
void lcd_init(void);

void lcd_write(unsigned char data)
{
	//Set E high
	PORTB |= (1<<PB2);
	//Write data to bus
	unsigned char lower_six = (data<<2);
	unsigned char upper_two = ((data & 0xC0)>>6);
	PORTD = lower_six | (PORTD & 0x03);
	PORTB = upper_two | (PORTB & 0xFC);
	_delay_us(1);
	//Set E low
	PORTB &= ~(1<<PB2);
}

void lcd_cmd(unsigned char cmd)
{	
	// Set RS low 
	PORTB &= ~(1<<PB4);
	lcd_write(cmd);
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