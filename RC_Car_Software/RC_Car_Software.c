#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Headers/ADC_INITIALIZATION.h"
#include "Headers/DEFINES.h"
#include "Headers/BLINKY.h"
#include "Headers/USART.h"

int BYTE_COUNT = 0; // address to be written to EEPROM, also the count of bytes already written
int OVERFLOW_COUNT = 0; // counter for the number of overflows

ISR(ADC_vect) {
	if (BYTE_COUNT < 512) {
		// write to EEPROM the adc value, and store in location BYTE_COUNT
		EEPROM_write(BYTE_COUNT, ReadADC());
		BYTE_COUNT++; 
		ADCSRA |= (1 << ADSC); // start next conversion
	}else {
		// 512 samples are done, reset BYTE_COUNT to 0, 
		// DO NOT START NEXT CONVERSION
		BYTE_COUNT = 0;
	}
}

ISR(INT0_vect) {
	HEX_USART(OVERFLOW_COUNT); // transmit overflow count in hex
	OVERFLOW_COUNT = 0; // reset overflow counter for timer
}

ISR(TIMER0_OVF_vect) {
	TCNT0 = TIMER_START; // reset timer start to 150
	if (OVERFLOW_COUNT <= 254) {
		OVERFLOW_COUNT++; // increment overflow count if below 255
	} else {
		OVERFLOW_COUNT = OVERFLOW_CLAMP; // clamp overflow count to 0xFE
	}
}

void Peripherals_Init(){
	DDRB = (1 << myLED);
	USART_Init(myBaud);
}

void ISR_Init() {
	sei(); // enable global interrupts
	TIMER0_Init();
	INT0_Init();
	//ADC_Init();
}

void TIMER0_Init() {
	TCCR0 |= (1 << CS02); // prescale at 256
	TCNT0 = TIMER_START; // timer start value at 150
	TIMSK |= (1 << TOIE0); // enable timer interrupt
}

void INT0_Init() {
	MCUCR |= (1 << ISC01) | (1 << ISC00); // configure int0 to interrupt on rising edge
	GICR |= (1 << INT0); // enable int0
	GIFR |= (1 << INTF0); // enable int0 interrupt
}

int main(void) {


	//DDRB |= (1<<SOC)|(1<<OE)|(1<<CLK);
	//PORTB |= (1<<SOC);
	//PORTB = PORTB & (~(1<<SOC));
	//UC *RAM_ptr2 = (UC *)0x2000;
	//UC data2, dtens, dones;
	//UI adr = 0x00;
	
	//UC *ptrCR = (UC *)CR_8255;
	//(*ptrCR) = CR_WORD_PA_Out_PB_IN;
	//UI nsamples = 1000;
	
	
	Peripherals_Init();
	ISR_Init();
	
	// cli(); clears global interrupts
	
	//for (BYTE_COUNT = 0; BYTE_COUNT < 512; BYTE_COUNT++) {
		//// read previously stored values from EEPROM and present them to PuTTy
		//HEX_USART(EEPROM_read(BYTE_COUNT));
		//PutC(',');
	//}
	
	// reset BYTE_COUNT to be used in interrupt
	//BYTE_COUNT = 0;
	
	while (1) {
		;
	}
}

