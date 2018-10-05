#include "../Headers/DEFINES.h"
#include "../Headers/USART.h"
#include <avr/io.h>

void hex2ascii(unsigned char hexval, unsigned char *c1, unsigned char *c2) {
	(*c1) = asctab[(hexval>>4)&0x0F];
	(*c2) = asctab[(hexval)&0x0F];
}

UC ascii2hex(UC ch) {
	UC temp;
	if (ch < 'A')
	{
		temp = ch - '0';
	}
	else
	{
		temp = ch - '7';
	}
	return temp;
}

void PutC(UC data) {
	while (!(UCSRA & (1 << UDRE))) // busy wait for empty transmit buffer
	;
	UDR = data; // put data into buffer and send it
}

UC GetC(void) {
	while (!(UCSRA & (1 << RXC))) // busy wait for buffer to be received
	;
	return UDR; // return received data
}

UC EchoC(void) {
	UC ch = GetC();
	PutC(ch);
	return ch;
}

void PutS(char *msg) {
	int k = 0;
	char ch;
	while (msg[k] != 0) {
		ch = msg[k];
		k = k + 1;
		PutC(ch);
	}
}

void USART_Init(UI baud) {
	// set baud rate
	UBRRH = (UC)((baud & 0xFF00) >> 8);
	UBRRL = (UC)((baud & 0x00FF));
	
	//enable receiver and transmitter
	UCSRB = (1 << RXEN) | (1 << TXEN);
	
	// set format to 8 bits 2 stop bits
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void HEX_USART(UC data) {
	char c1, c2;
	
	hex2ascii(data, &c1, &c2);
	PutC(c1);
	PutC(c2);
	PutC(',');

}
