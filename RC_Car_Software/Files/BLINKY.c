#include <avr/io.h>
#include "../Headers/DEFINES.h"
#include "../Headers/BLINKY.h"
#include <util/delay.h>

void blink_LED() {
	// blinking LED //
	PORTB = ~(1 << myLED) & PORTB; // on led
	_delay_ms(500);
	PORTB = (1 << myLED) | PORTB; //off led
	_delay_ms(500);
}