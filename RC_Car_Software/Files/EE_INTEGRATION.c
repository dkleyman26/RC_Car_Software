#include <avr/io.h>
#include "../Headers/DEFINES.h"
#include "../Headers/EE_INTEGRATION.h"
#include <util/delay.h>

void CPE_RELAY_Init() {
	DDRB |= (1 << RELAY_PIN); // enable cpe relay pin
}

void CPE_RELAY_Test() {
	PORTB = ~(1 << RELAY_PIN) & PORTB;	// on CPE Board off EE Board
	_delay_ms(500);
	PORTB = (1 << RELAY_PIN) | PORTB;	// on EE Board off CPE Board
	_delay_ms(500);
}
