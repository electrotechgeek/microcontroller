// Project 2 (Timers, SPI, DAC)
// Waveform generator
// Jason Dreisbach
// Jason Descenzo

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FrequencyCount 5
typedef enum {
	f100,
	f200,
	f300,
	f400,
	f500
} Freqency;
Freqency currentFrequency = f100;

#define WaveTypeCount 3
#define SampleCount   40
int waveForms[WaveTypeCount][SampleCount] = { 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF,0xFFF},
	{2048, 2368, 2680, 2978, 3251, 3496, 3704, 3872, 3995, 4070, 4095, 4070, 3995, 3872, 3704, 3496, 3251, 2977, 2680, 2368, 2048, 1727, 1415, 1118, 844, 600, 391, 223, 100, 25, 0, 25, 100, 223, 391, 600, 844, 1118, 1415, 1727},
	{0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900}
};

typedef enum {
	square_wave  = 0,
	sine_wave    = 1,
	saw_tooth    = 2
} WaveFormType;
WaveFormType currentWaveformType = square_wave;

#define MOSI 3  				// PB pin 3
#define SCK  5  				// PB pin 5
#define SS   2  				// PB pin 2

void Initialize_SPI_Master(void)
{
	DDRB = 1<<MOSI | 1<<SCK | 1<<SS;    // make MOSI, SCK and SS outputs

    SPCR = (0<<SPIE) | 		//No interrupts
    (1<<SPE) | 			    //SPI enabled
    (0<<DORD) | 			//shifted out LSB
    (1<<MSTR) | 			//master
    (1<<CPOL) | 			//rising leading edge
    (1<<CPHA) | 			//sample leading edge
    (0<<SPR1) | (0<<SPR0);  //clock speed
    SPSR = (0<<SPIF) | 		//SPI interrupt flag
    (0<<WCOL) | 			//Write collision flag
    (0<<SPI2X) ; 			//Doubles SPI clock
    PORTB = 1 << SS;  		//make sure SS is high
}

void Transmit_SPI_Master(int Data)
{
	PORTB = 0 << SS;						// assert the slave select
	SPDR = ((Data >> 8) & 0xF) | 0x70;   	// Start transmission, send high byte first
 	while (!(SPSR & (1<<SPIF)));			// Wait for transmission complete
	SPDR = 0xFF & Data; 					// send low byte next
	while (!(SPSR & (1<<SPIF)));			// Wait for transmission complete
	PORTB = 1 << SS; 						// de-assert slave select
}

void setTimer(Freqency freq)
{
	// timer overflow mode (CTC mode)
	TCCR0A = 0x02;
	// clear previous timer overflow
	TIFR0 = 0x02;
	// timer overflow interrupt enabled
	TIMSK0 = 0x02;

	switch (freq) {
		case f200:
			TCCR0B = 0x03;
			OCR0A = 30;
		break;
		case f300:
			TCCR0B = 0x03;
			OCR0A = 20;
		break;
		case f400:
			TCCR0B = 0x02;
			OCR0A = 124;
		break;
		case f500:
			TCCR0B = 0x02;
			OCR0A = 100;
		break;
		case f100:
			TCCR0B = 0x03;
			OCR0A = 61;
		default:
		break;
	}
}

double scale = 1.0;
int offset = 0;
char idx = 0;
ISR(TIMER0_COMPA_vect)
{
	if (idx >= SampleCount) {
		idx = 0;
	}
	Transmit_SPI_Master(waveForms[currentWaveformType][idx++] * scale + offset);
}


#define FREQ_BTN    _BV(PC0)
#define WAVE_BTN    _BV(PC1)
#define DUTY_BUTTON _BV(PC2)

void main()
{
	Initialize_SPI_Master();
	setTimer(currentFrequency);
	// Setup buttons
	DDRC &= ~(FREQ_BTN|WAVE_BTN|DUTY_BUTTON);
	PORTC |= FREQ_BTN|WAVE_BTN|DUTY_BUTTON;

	sei();



	for (;;) {

		if (!(PINC & FREQ_BTN)) {
			if (currentFrequency + 1 == FrequencyCount) {
				currentFrequency = f100;
			}
			else {
				currentFrequency++;
			}
			setTimer(currentFrequency);
		}


		if (!(PINC & WAVE_BTN)) {
			if (currentWaveformType + 1 == WaveTypeCount) {
				currentWaveformType = square_wave;
			}
			else {
				currentWaveformType++;
			}
		}

		if (!(PINC & DUTY_BUTTON)) {
			// TODO: duty
			if (currentWaveformType == square_wave) {
				int i = 0;
				for (; i < SampleCount; i++) {
					if (waveForms[square_wave][i] != 0) {
						break;
					}
				}

				if (i == SampleCount) {
					for (i = 0; i < SampleCount; i++) {
						waveForms[square_wave][i] = 0xFFF;
					}
				}
				else { 
					waveForms[square_wave][i] = 0x000;
					waveForms[square_wave][i+1] = 0x000;
					waveForms[square_wave][i+2] = 0x000;
					waveForms[square_wave][i+3] = 0x000;
				}
			}
			else {
				scale = 0.5;
				offset += 128;
				if (offset > 2048){
					offset = 0;
				}
			}
		}

		_delay_ms(500);		
	}
}