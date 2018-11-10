#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

#define NO_PRESCALE (1 << CS10);
#define PRESCALE_8 (1 << CS11);
#define PRESCALE_64 (1 << CS11) | (1 << CS10);
#define PRESCALE_256 (1 << CS12);
#define PRESCALE_1024 (1 << CS12) | (1 << CS10);

void Phase_Corrected_PWM_Init();

#endif /* PID_CONTROLLER_H_ */