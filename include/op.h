#ifndef OP_H
#define OP_H

#include "reg.h"
#include "cpu.h"

// all of these are defined in alphabetical order
// these get returned from the parse_op() function
#define ADC 1
#define ADD 2
#define ADIW 3
#define AND 4
#define ANDI 5

#define LDI 69

#define OUT 84

#define RJMP 90

// these are the hex representations of the various instructions
// 0's often represent parts that normally contain a parameter
#define ADC_OP 0x1C
#define ADD_OP 0x0C
#define ADIW_OP 0x96
#define AND_OP 0x20
#define ANDI_OP 0x70

#define LDI_OP 0xE0

#define OUT_OP 0xB8

#define RJMP_OP 0xC0

unsigned char parse_op(unsigned short opcode);

void adc(unsigned char rd, unsigned char rr);
void add(unsigned char rd, unsigned char rr);
void adiw(unsigned char rd, unsigned char k);
void and(unsigned char rd, unsigned char rr);
void andi(unsigned char rd, unsigned char k);
void ldi(unsigned char rd, unsigned char k);
void out(unsigned char a, unsigned char rr);
void rjmp(short dest);

#endif