// This file holds all the global defines of the program

#ifndef DEFINES_H_
#define DEFINES_H_

#define F_CPU					8000000UL
#define TIMER0_START			0x96	// 150
#define TIMER1_START			0xE0	// 224
#define OVERFLOW_CLAMP			0xFE	// 254
#define LEFT_SWITCH				PIND6	// EE PID enable pin
#define RIGHT_SWITCH			PIND7	// CPE PID enable pin
#define CPE_SWITCH_ENABLED		PIND & (1 << RIGHT_SWITCH)
#define EE_SWITCH_ENABLED		PIND & (1 << LEFT_SWITCH)
#define CPE_SWITCH_DISABLED		!(PIND & (1 << RIGHT_SWITCH))
#define EE_SWITCH_DISABLED		!(PIND & (1 << LEFT_SWITCH))

/* type definitions */
typedef unsigned char	UC;
typedef unsigned int	UI;
typedef uint8_t			ui8;
typedef uint16_t		ui16;
typedef uint32_t		ui32;

enum Mode {IDLE = 1, EE_PID = 2, CPE_PID = 3};

/*
char	-> ui8
UC		-> ui8
int		-> ui16
UI		-> ui16
*/

#endif /* DEFINES_H_ */