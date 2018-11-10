#include <avr/io.h>
#include <avr/interrupt.h>

#include "Headers/ADC.h"
#include "Headers/DEFINES.h"
#include "Headers/BLINKY.h"
#include "Headers/USART.h"
#include "Headers/TWI.h"
#include "Headers/PID_CONTROLLER.h"
#include "Headers/EE_INTEGRATION.h"

#include <util/delay.h>

int BYTE_COUNT = 0; // address to be written to EEPROM, also the count of bytes already written
int OVERFLOW_COUNT = 0; // counter for the number of overflows
ui8 ADC_VALUE_0 = -1, ADC_VALUE_1 = -1, ADC_VALUE_2 = -1; // used to store 2 channel adc values
int TWI_MEMORY_LOCATION = 0; // twi memory location from 0 -> 16000 (64 kbits of EEPROM * 2)
int ALLOW_TWI_WRITE = 0; // determines if TWI should be written after 2 values are read
enum Mode OPERATION_MODE = IDLE; // determine operation mode of the car


ISR(ADC_vect) {
	static int ADC_SELECT = 0; // choose between ADC0 and ADC1
	ADMUX = ADMUX & 0b11110000; // mask bits in order to switch channels
	
	if (OPERATION_MODE == CPE_PID) {
		ADMUX |= (1 << MUX1); // set channel to ADC2 to read potentiometer output
		ADC_VALUE_2 = ReadADC(); 
	} else if (OPERATION_MODE == EE_PID) {
		switch(ADC_SELECT) {
			case 0: // current channel is ADC0
				ADC_VALUE_0 = ReadADC();
				ADC_SELECT = 1; // switch channel to ADC1
				ADMUX |= (1 << MUX0); // set MUX0 bit to select ADC1
				ALLOW_TWI_WRITE = 0; // do not allow twi to transmit as both channels are not sampled
				ADCSRA |= (1 << ADSC); // start next conversion
				break;
			default: // current channel is ADC1
				ADC_VALUE_1 = ReadADC();
				ADC_SELECT = 0; // switch back to channel ADC0
				ALLOW_TWI_WRITE = 1; // allow twi to transmit as both channels ARE sampled
				break;
		}
	}

	/* Uncomment to transmit to ONCHIP EEPROM */
	/* -------------------------------------- */
	//if (BYTE_COUNT < 512) {
		//// write to EEPROM the adc value, and store in location BYTE_COUNT
		//EEPROM_ONCHIP_write(BYTE_COUNT, ReadADC());
		//BYTE_COUNT++; 
        //ADCSRA |= (1 << ADSC); // start next conversion
	//}else {
		//// 512 samples are done, reset BYTE_COUNT to 0, 
		//// DO NOT START NEXT CONVERSION
		//BYTE_COUNT = 0;
	//}
	/* -------------------------------------- */
    
}

ISR(INT0_vect) {

	/* Uncomment to transmit to ONCHIP EEPROM */
	/* -------------------------------------- */
	//if (BYTE_COUNT < 512) {	// write no more than 512 bytes to EEPROM
		//EEPROM_ONCHIP_write(BYTE_COUNT, OVERFLOW_COUNT);
		//BYTE_COUNT++;
	//} else {
		//PORTB = ~PORTB; // toggle led when done
	//}
	/* -------------------------------------- */

	//HEX_USART(OVERFLOW_COUNT); // transmit overflow count in hex
	OVERFLOW_COUNT = 0; // reset overflow counter for timer
}

ISR(TIMER0_OVF_vect) {
	TCNT0 = TIMER0_START; // reset timer start to 150
	if (OVERFLOW_COUNT <= 254) {
		OVERFLOW_COUNT++; // increment overflow count if below 255
	} else {
		OVERFLOW_COUNT = OVERFLOW_CLAMP; // clamp overflow count to 0xFE
	}
}

void ISR_Init() {
	sei(); // enable global interrupts
	TIMER0_Init();
	INT0_Init();
	ADC_Init();
}

void TIMER0_Init() {
	TCCR0 |= (1 << CS02); // prescale at 256
	TCNT0 = TIMER0_START; // timer start value at 150
	TIMSK |= (1 << TOIE0); // enable timer interrupt
}

void INT0_Init() {
	MCUCR |= (1 << ISC01) | (1 << ISC00); // configure int0 to interrupt on rising edge
	GICR |= (1 << INT0); // enable int0
	GIFR |= (1 << INTF0); // enable int0 interrupt
}

void Peripherals_Init() {
	DDRB = (1 << myLED);		// configure LED pin
	LED_OFF;			
	USART_Init(myBaud);			// init usart
	TWI_Init();					// init twi
	Phase_Corrected_PWM_Init();	// init PWM
	CPE_RELAY_Init();			// init CPE relay
}

void Transmit_ONCHIP_EEPROM() {
	for (BYTE_COUNT = 0; BYTE_COUNT < 512; BYTE_COUNT++) {
		// read previously stored values from EEPROM and present them to PuTTy
		HEX_USART(EEPROM_ONCHIP_read(BYTE_COUNT));
	}
	// reset BYTE_COUNT to be used in interrupt
	BYTE_COUNT = 0;
}

void Transmit_EEPROM(int byteStartAddress, int byteEndAddress, ui8 CHIP_SELECT_WRITE, ui8 CHIP_SELECT_READ, int fromEEPROM2) {
	int i = byteStartAddress;
	ui8 data;
	
	for (i = byteStartAddress; i < byteEndAddress; i++) {
		data = EEPROM_read(i, CHIP_SELECT_WRITE, CHIP_SELECT_READ);
		if (fromEEPROM2) {
			PutS("POTENTIOMETER VALUE:\t");
		} else {
			if (i % 2 == 0) {
				PutS("BATTERY SENSE:\t");
			} else {
				PutS("CPE FEEDBACK:\t");
			}
		}
		HEX_USART(data);
		PutC('\n');
	}
}

void Determine_Operation_Mode() {
	if (CPE_SWITCH_DISABLED && EE_SWITCH_DISABLED) {  
		OPERATION_MODE = IDLE;
		EE_BOARD_ENABLE_RELAY;
	} else if (CPE_SWITCH_ENABLED) { 
		OPERATION_MODE = CPE_PID;
		CPE_BOARD_ENABLE_RELAY;
	} else if (EE_SWITCH_ENABLED && CPE_SWITCH_DISABLED) {
		OPERATION_MODE = EE_PID;
		EE_BOARD_ENABLE_RELAY;
	}
}

int main(void) {
	Peripherals_Init();
	ISR_Init();
	PutS("\nTEST\n");
	// cli(); clears global interrupts
	

	while (1) {
		Determine_Operation_Mode();
		switch (OPERATION_MODE) {
			case IDLE:
				//Transmit_EEPROM(4, 800, WRITE_EEPROM1, READ_EEPROM1, 0); // not starting at 0
				//Transmit_EEPROM(4, 400, WRITE_EEPROM2, READ_EEPROM2, 1); // not starting at 0
				//PutS("IDLE");
				break;
			case EE_PID:
				TWI_MEMORY_LOCATION = 0;
				if (ALLOW_TWI_WRITE) {
					// 64 kbits of AT24C64 two wire serial EEPROM
					if (TWI_MEMORY_LOCATION < 8000) {										// write to EEPROM chip 1
						EEPROM_write(TWI_MEMORY_LOCATION++, ADC_VALUE_0, WRITE_EEPROM1);
						EEPROM_write(TWI_MEMORY_LOCATION++, ADC_VALUE_1, WRITE_EEPROM1);
					} else {
						TWI_MEMORY_LOCATION = 0;
					}
					ADCSRA |= (1 << ADSC); // start next conversion
				}
				//PutS("EE");
				break;
			case CPE_PID:
				TWI_MEMORY_LOCATION = 0;	
				if (TWI_MEMORY_LOCATION < 8000) {											// write to EEPROM chip 2
					EEPROM_write(TWI_MEMORY_LOCATION++, ADC_VALUE_2, WRITE_EEPROM2);
				} else {
					TWI_MEMORY_LOCATION = 0;
				}
				ADCSRA |= (1 << ADSC); // start next conversion
				//PutS("CPE");
				break;
			default:
				break;
		}
		ADCSRA |= (1 << ADSC);
		PutC('\n');
		HEX_USART(ReadADC());
	}
}

