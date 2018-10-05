// This file holds all the global defines of the program

#ifndef DEFINES_H_
#define DEFINES_H_

#define TIMER_START 0x96
#define OVERFLOW_CLAMP 0xFE

/* type definitions */
typedef unsigned char UC;
typedef unsigned int UI;

#define PA_8255 0x6000
#define PB_8255 0x6001
#define PC_8255 0x6002
#define CR_8255 0x6003
#define CR_WORD_PA_Out_PB_IN 0x82

#define CLK PB2
#define SOC PB3
#define OE PB1
#define CS PB4
#define MOSI PB5
#define MISO PB6
#define SCK PB7

#define READ 0x03
#define WRITE 0x02
#define WRDI 0x04
#define WREN 0x06
#define RDSR 0x05
#define WRSR 0x01

#endif /* DEFINES_H_ */