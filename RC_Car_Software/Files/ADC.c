#include <avr/io.h>
#include "../Headers/DEFINES.h"
#include "../Headers/ADC.h"

void ADC_Init() {
	ADMUX |= (1 << ADLAR); // left adjust adc result to get 8 bit precision from 10 bit output
	
	// enable adc, start conversion, enable interrupt, set prescaler to 64
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1);
}

uint8_t ReadADC(){
	return ADCH;
}
