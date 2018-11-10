#ifndef BLINKY_H_
#define BLINKY_H_

#define myLED PB0
#define LED_ON PORTB = ~(1 << myLED) & PORTB; // on led
#define LED_OFF PORTB = (1 << myLED) | PORTB; // off led

void blink_LED();

#endif /* BLINKY_H_ */