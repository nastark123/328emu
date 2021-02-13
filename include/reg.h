#ifndef REG_H
#define REG_H

// 32 data registers
unsigned char data_reg[32];

// status register
unsigned char SREG;

// offsets for various bits in the status register
#define SREG_C 0
#define SREG_Z 1
#define SREG_N 2
#define SREG_V 3
#define SREG_S 4
#define SREG_H 5
#define SREG_T 6
#define SREG_I 7

// port registers for I/O
unsigned char PORTA;
unsigned char PORTB;
unsigned char PORTC;
unsigned char PORTD;

// data direction registers for I/O
unsigned char DDRA;
unsigned char DDRB;
unsigned char DDRC;
unsigned char DDRD;

// will initialize all registers to their default states
void init_regs();

#endif