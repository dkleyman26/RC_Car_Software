#ifndef TWI_H_
#define TWI_H_

#define		WRITE_EEPROM1	0b10100010	// write eeprom 1 -> lsb = 0 = write
#define		READ_EEPROM1	0b10100011	// write eeprom 1 -> lsb = 1 = read
#define		WRITE_EEPROM2	0b10100100	// write eeprom 2 -> lsb = 0 = write
#define		READ_EEPROM2	0b10100101	// write eeprom 2 -> lsb = 1 = read

/* status codes */ 
#define START			0x08
#define REPEATED_START	0x10
#define SLA_W_ACK		0x18
#define SLA_W_NACK		0x20
#define DATA_ACK		0x28
#define DATA_NACK		0x30
/* ------------ */

void TWI_Init();
void TWI_Start();
void TWI_Stop();
void TWI_Write(uint8_t data);
uint8_t TWI_Status();
uint8_t TWI_ReadACK();
uint8_t TWI_ReadNACK();

#endif /* TWI_H_ */