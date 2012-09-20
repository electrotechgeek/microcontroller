/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

// registers locations and some other things
#include <avr/io.h>
// tells compiler the freq of your procesor
//#define F_CPU 16000000UL
// software delay functions
#include <util/delay.h>


int main(void){
    DDRB |= (1<<PB5);
    
    // infinite loop
    while(1) {
        // Write PB0 high
        PORTB |= (1<<PB5);
        // 250 us
        _delay_us(250);
        // Turn PB0 OFF
        PORTB &= ~(1<<PB5);
        // wait for the rest of the second
        _delay_us(750);
    }
    return 1;
}