/*
 * ADC.c
 *
 * Created: 9/12/2018 11:08:52 AM
 *  Author: VladBaciu
 */ 
#include <avr/io.h>

void adc_init()
{
	
	ADMUX = (1<<REFS0);
	
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t adc_read(uint8_t ch) {
	// select the corresponding channel 0~7
	// ANDing with '7' will always keep the value
	// of 'ch' between 0 and 7
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;
	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);
	// wait for conversion to complete
	// ADSC becomes '0' again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	return (ADC);
}