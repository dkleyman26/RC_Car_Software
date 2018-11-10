#ifndef EE_INTEGRATION_H_
#define EE_INTEGRATION_H_


#define RELAY_PIN					DDB2
#define CPE_BOARD_ENABLE_RELAY		PORTB = ~(1 << RELAY_PIN) & PORTB
#define EE_BOARD_ENABLE_RELAY		PORTB = (1 << RELAY_PIN) | PORTB

void CPE_RELAY_Init(); 
void CPE_RELAY_Test();

#endif /* EE_INTEGRATION_H_ */