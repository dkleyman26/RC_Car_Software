#include <avr/io.h>
#include "../Headers/DEFINES.h"
#include "../Headers/EEPROM.h"
#include "../Headers/TWI.h"
#include <util/delay.h>

void EEPROM_ONCHIP_write(ui16 address, ui8 data) {
	while(EECR & (1 << EEWE)); // busy wait for prev write
	EEAR = address; // set up address register
	EEDR = data; // set up data register
	EECR |= (1 << EEMWE); // write 1 to EEMWE
	EECR |= (1 << EEWE); // start eeprom write by setting EEWE
}

ui8 EEPROM_ONCHIP_read(ui16 address) {
	while(EECR & (1 << EEWE)); // busy wait for prev write
	EEAR = address; // set up address register
	EECR |= (1 << EERE); // start eeprom read by setting EEWE
	return EEDR; // return data from data register
}

void EEPROM_write(ui16 address, ui8 data, ui8 CHIP_SELECT) {
	ui8 addressHigh =	((address & 0xFF00) >> 8); // upper 8 address bits
	ui8 addressLow =	(address & 0x00FF); // lower 8 address bits
	
	TWI_Start(); // start TWI transmission
	TWI_Write(CHIP_SELECT); // send SLA_W to eeprom selected chip
	while(TWI_Status() != SLA_W_ACK) { // check if SLA_W and ack received
		TWI_Start(); // resend start again
		while (TWI_Status() != REPEATED_START); // verify restart
		TWI_Write(CHIP_SELECT); // resend SLA_W to eeprom selected chip
	}
	
	TWI_Write(addressHigh); // write MSB address
	while (TWI_Status() != DATA_ACK); // wait for ack
	TWI_Write(addressLow);	// write LSB address
	while (TWI_Status() != DATA_ACK); // wait for ack
	
	TWI_Write(data); // write data
	while (TWI_Status() != DATA_ACK); // wait for ack
	_delay_us(100);
	TWI_Stop(); // send stop condition
}

ui8 EEPROM_read(ui16 address, ui8 CHIP_SELECT_WRITE, ui8 CHIP_SELECT_READ) {
	ui8 addressHigh =	((address & 0xFF00) >> 8); // upper 8 address bits
	ui8 addressLow =	(address & 0x00FF); // lower 8 address bits
	ui8 data;
	
	TWI_Start(); // start TWI transmission
	TWI_Write(CHIP_SELECT_WRITE); // send SLA_W to eeprom selected chip
	while(TWI_Status() != SLA_W_ACK) { // check if SLA_W and ack received
		TWI_Start(); // resend start again
		while (TWI_Status() != REPEATED_START); // verify restart
		TWI_Write(CHIP_SELECT_WRITE); // resend SLA_W to eeprom selected chip
	}
	
	TWI_Write(addressHigh); // write MSB address
	while (TWI_Status() != DATA_ACK); // wait for ack
	TWI_Write(addressLow);	// write LSB address
	while (TWI_Status() != DATA_ACK); // wait for ack
	
	TWI_Start(); // re-send start this time to read
	TWI_Write(CHIP_SELECT_READ); // send SLA_R to eeprom selected chip
	data = TWI_ReadNACK();
	//_delay_us(100);
	TWI_Stop();
	return data;
}


