// Jason Dreisbach
// Jason Desenza
// Project 1 (Hello World)
void lcd_cmd(unsigned char cmd)
{	// Set RS low 
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
{	DDRB |= 0xFC; 
	DDRD |= 0x1F;
	PORTB &= ~(1<<PB2);
	_delay_ms(15);
	lcd_cmd(0x38);
	_delay_us(1);
}


int main()
{
	return 0;
}