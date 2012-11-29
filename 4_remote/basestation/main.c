// Base station
// Jason Dreisbach
// Jason Descenzo
// Project 4

#include <avr/interrupt.h>
#include "usart.h"

void recordCode();
void emitCode();

int main (void)
{
	usart_init(9600, 16000000);
	sei();

	uint8_t command;
	for (;;) {
		command = usart_recv();
		switch (command) {
			case 'r':
			case 'R':
				recordCode();
				break;
			case 'p':
			case 'P':
				emitCode();
				break;
			default:
				break;
		}
	}

	return 0;
}




