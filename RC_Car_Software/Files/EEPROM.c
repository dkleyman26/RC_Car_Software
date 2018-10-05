#include "../Headers/DEFINES.h"
#include "../Headers/EEPROM.h"
#include <avr/io.h>

void EEPROM_write(UI address, UC data) {
	while(EECR & (1 << EEWE)) // busy wait for prev write
	;
	EEAR = address; // set up address register
	EEDR = data; // set up data register
	EECR |= (1 << EEMWE); // write 1 to EEMWE
	EECR |= (1 << EEWE); // start eeprom write by setting EEWE
}

UC EEPROM_read(UI address) {
	while(EECR & (1 << EEWE)) // busy wait for prev write
	;
	EEAR = address; // set up address register
	EECR |= (1 << EERE); // start eeprom read by setting EEWE
	return EEDR; // return data from data register
}
