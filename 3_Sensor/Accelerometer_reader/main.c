#include <stdlib.h>					// Standard C library
#include <avr/io.h>					// Input-output ports, special registers
#include <util/delay.h>
#include <stdio.h>
#define BAUD_PRESCALE 103

void adc_init(void)
{
  ADCSRA = 0x87;  //Turn On ADC and set prescaler (CLK/128--62 kHz)
                  //MAX A/D conversion rate 5 kHz @ 62 kHz frequency    
  ADCSRB = 0x00;  //Set gain & turn off autotrigger
  ADMUX = 0x00;   //Set ADC channel ADC0 with 1X gain
}

void usart_init(uint16_t baudin, uint32_t clk_speedin)
{
	uint32_t ubrr = (clk_speedin/16UL)/baudin-1;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*UBRR0H = (BAUD_PRESCALE>>8);
	UBRR0L = BAUD_PRESCALE;*/
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	// UCSR0C = (0<<USBS0)|(3<<UCSZ00);
	/* Set frame format: 8data, 2stop bits */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	UCSR0A &= ~(1<<U2X0);
}

/*the send function will put 8bits on the trans line. */
void usart_send( uint8_t data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

/* the receive data function. Note that this a blocking call
Therefore you may not get control back after this is called 
until a much later time. It may be helpfull to use the 
istheredata() function to check before calling this function
	@return 8bit data packet from sender
*/
uint8_t  usart_recv(void)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

/* function check to see if there is data to be received
	@return true is there is data ready to be read */
uint8_t  usart_istheredata(void)
{
	 return (UCSR0A & (1<<RXC0));
}

int main (void)
{
	usart_init(9600, 16000000);
	char number_string[5];
	int bytes_written, i, voltage;
	adc_init();

	while(1)
	{
		bytes_written = 0;
		ADMUX = 0x00; // ADC 0 (X)
		ADCSRA = 0xC7;        // start conversion
	    _delay_us(260);     // ensure max sampling rate not exceeded
	    voltage = ADC & 0x3FF;        // read voltage
	    usart_send('<'); bytes_written++;
	    sprintf(number_string, "%04d", voltage);
	    for (i = 0; number_string[i] != '\0'; i++) {
		    usart_send(number_string[i]);
		}

		usart_send(','); bytes_written++;

		ADMUX = 0x01; // ADC 1 (Y)
		ADCSRA = 0xC7;        // start conversion
	    _delay_us(260);     // ensure max sampling rate not exceeded
	    voltage = ADC & 0x3FF;        // read voltage
		sprintf(number_string, "%04d", voltage);
	    for (i = 0; number_string[i] != '\0'; i++) {
		    usart_send(number_string[i]);
		}

		usart_send(',');

		ADMUX = 0x02; // ADC 1 (Z)
		ADCSRA = 0xC7;        // start conversion
	    _delay_us(260);     // ensure max sampling rate not exceeded
	    voltage = ADC & 0x3FF;        // read voltage
		sprintf(number_string, "%04d", voltage);
	    for (i = 0; number_string[i] != '\0'; i++) {
		    usart_send(number_string[i]);
		}
		usart_send('>');
		// usart_send('\r');
		// usart_send('\n');
		_delay_ms(500);

	}
	return 0;
}