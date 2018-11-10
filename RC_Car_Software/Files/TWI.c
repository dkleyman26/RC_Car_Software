#include <avr/io.h>
#include "../Headers/DEFINES.h"
#include "../Headers/TWI.h"
#include "../Headers/USART.h"
#include <util/delay.h>

void TWI_Init() {
	TWBR |= 0x08; // 8 -> bit rate register (TWBR)
	// default at 1 -> bit rate prescaler (TWPS)
	// TWSR |= (1 << TWPS1) | (1 << TWPS0); 
}

void TWI_Start() {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // send start condition
	while (!(TWCR & (1 << TWINT))); // wait for interrupt to be set
}

void TWI_Stop() {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // send stop condition
}

void TWI_Write(uint8_t data) {
	TWDR = data; // sets data register to "data"
	TWCR = (1 << TWINT) | (1 << TWEN); // reset interrupt and re-enable
	while(!(TWCR & (1 << TWINT))); // wait for interrupt to be set
}

uint8_t TWI_Status() {
	return (TWSR & 0xF8); // return status register masked lower 3 bits
}

// transmit ack signal after byte transfer
uint8_t TWI_ReadACK() {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while(!(TWCR & (1 << TWINT))); 
	return TWDR;
}

// transmit nack signal after byte transfer
uint8_t TWI_ReadNACK() {
	TWCR = (1 << TWINT) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
	return TWDR;
}