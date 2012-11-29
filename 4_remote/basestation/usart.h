// Usart fuctions
#include <avr/io.h>

void usart_init(uint16_t baudin, uint32_t clk_speedin);
void usart_send(uint8_t data);
uint8_t usart_recv(void);
uint8_t usart_istheredata(void);

void printString(char *string);
void printLine(char *string);
