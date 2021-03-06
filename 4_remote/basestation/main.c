// Base station
// Project 4
// Jason Dreisbach
// Jason Descenzo

#include <avr/interrupt.h>
#include "usart.h"

void initRecorder();
void recordCode();

void initEmitter();
void emitCode();

int main (void)
{
	initRecorder();
	initEmitter();
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




