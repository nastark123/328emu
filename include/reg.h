#ifndef REG_H
#define REG_H

// register space - not all of these are used
// does not include the CPU's data registers
unsigned char regs[256];

// 32 data registers
unsigned char data_reg[32];

// status register
#define SREG 0x3F

// offsets for various bits in the status register
#define SREG_C 0
#define SREG_Z 1
#define SREG_N 2
#define SREG_V 3
#define SREG_S 4
#define SREG_H 5
#define SREG_T 6
#define SREG_I 7

// stack pointer registers
#define SPH 0x3E
#define SPL 0x3D

// port registers for I/O
#define PORTB 0x05
#define PORTC 0x08
#define PORTD 0x0B

// data direction registers for I/O
#define DDRB 0x04
#define DDRC 0x07
#define DDRD 0x0A

// input registers
#define PINB 0x03
#define PINC 0x06
#define PIND 0x09

// will initialize all registers to their default states
void init_regs();

#endif