// ADC Tutorial
// Jason Dreisbach
// Jason Descenzo

#include <avr/io.h>
#include <util/delay.h>

void Do_0(void);
void Do_1(void);
void Do_2(void);

void Initialize_ADC0(void);
void Initialize_ADC1(void);

void Initialize_ADC0(void)
{
  ADCSRA = 0x87;  //Turn On ADC and set prescaler (CLK/128--62 kHz)
                  //MAX A/D conversion rate 5 kHz @ 62 kHz frequency    
  ADCSRB = 0x00;  //Set gain & turn off autotrigger
  ADMUX = 0x00;   //Set ADC channel ADC0 with 1X gain
}

void Do_0()
{
  int i, voltage = 0;
  char toggle = 0;
      
  DDRB = 1 << 5;                // Set PB5 output

  Initialize_ADC0();            // Initialize ADC0 for manual trigger

  for (;;) {
    //Initialize_ADC0();   // Initialize ADC0 for manual trigger
    ADCSRA = 0xC7;        // start conversion
    _delay_us(260);     // ensure max sampling rate not exceeded
    voltage = ADC & 0x3FF;        // read voltage
    
    toggle = toggle ^ 1;
    PORTB = (toggle & 0x01) << 5; // toggle LED
    for (i =0; i<voltage; i = i + 10) {// wait (0 - 1023)/10 mS
      _delay_ms(1);
    }
  }
}

void Initialize_ADC1(void)
{
  ADCSRA = 0x87;  //Turn On ADC and set prescaler (CLK/128--62 kHz)
                  //MAX A/D conversion rate 5 kHz @ 62 kHz frequency    
  ADCSRB = 0x00;  //Set gain & turn off autotrigger
  ADMUX = 0x01;   //Set ADC channel ADC0 with 1X gain
}

void Do_1(void)
{
  int i, voltage = 0;
  char toggle = 0;
      
  DDRB = 1 << 5;                // Set PB5 output

  Initialize_ADC1();            // Initialize ADC0 for manual trigger

  for (;;) {
    //Initialize_ADC0();   // Initialize ADC0 for manual trigger
    ADCSRA = 0xC7;        // start conversion
    _delay_us(260);     // ensure max sampling rate not exceeded
    voltage = ADC & 0x3FF;        // read voltage
    
    toggle = toggle ^ 1;
    PORTB = (toggle & 0x01) << 5; // toggle LED
    for (i =0; i<voltage; i = i + 10) {// wait (0 - 1023)/10 mS
      _delay_ms(1);
    }
  }
}

void Do_2(void) 
{
  int i, voltage = 0;
  char toggle = 0;
      
  DDRB = 1 << 5;      // Set PB5 output

  Initialize_ADC0();  // Initialize ADC0 for manual trigger

  for (;;) {
    
    ADCSRA = 0xC7;    // start conversion
    // _delay_us(260);   // ensure max sampling rate not exceeded
    voltage = ADC & 0x3FF;        // read voltage
    
    toggle = toggle ^ 1;
    PORTB = (toggle & 0x01) << 5; // toggle LED

    if (voltage < 205) {
      _delay_ms(1000);
    }
    else if (voltage < 410) {
      _delay_ms(500);
    }
    else if (voltage < 615) {
      _delay_ms(250);
    }
    else if (voltage < 820) {
      _delay_ms(125);
    }
    else {
      _delay_ms(62);
    }
  }
}


int main(void)
{
  Do_2();
  return 0;
}