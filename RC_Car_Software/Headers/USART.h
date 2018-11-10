#ifndef USART_H_
#define USART_H_

static const char asctab[] = "0123456789ABCDEF";

#define F_CPU	8000000UL
#define BAUD	9600
#define myBaud	F_CPU / 16 / BAUD - 1

void hex2ascii(unsigned char hexval, unsigned char *c1, unsigned char *c2);
UC ascii2hex(UC ch);
void PutC(UC data);
UC GetC(void);
UC EchoC(void);
void PutS(char *msg);
void USART_Init(UI baud);
void HEX_USART(UC data);

#endif /* USART_H_ */