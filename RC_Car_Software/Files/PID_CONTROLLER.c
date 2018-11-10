#include <avr/io.h>
#include "../Headers/DEFINES.h"
#include "../Headers/PID_CONTROLLER.h"

void Phase_Corrected_PWM_Init() {
	DDRB |= (1 << DDB1);					// set PB1 to be PWM output
	TCCR1A = (1 << WGM10) | (1 << COM1A1);	// set phase corrected PWM and non inverting mode
	TCCR1B |= NO_PRESCALE;					// no prescaler
	OCR1A = 63;								// 25% duty cycle			
}

float PID(ui8 desired_value, ui8 measured_value) {
	static ui8 previous_error = 0; // static value to keep track of previous error
	ui8 error = desired_value - measured_value; // current error
}


