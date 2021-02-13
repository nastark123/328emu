#ifndef OP_H
#define OP_H

#include "reg.h"
#include "cpu.h"

// all of these are defined in alphabetical order
// these get returned from the parse_op() function
#define ADC 1
#define ADD 2

#define LDI 69

// these are the hex representations of the various instructions
// 0's often represent parts that normally contain a parameter
#define ADC_OP 0x10
#define ADD_OP 0x00

#define LDI_OP 0xE0

unsigned char parse_op(unsigned short opcode);

void adc(unsigned char rd, unsigned char rr);
void add(unsigned char rd, unsigned char rr);
void ldi(unsigned char rd, unsigned char k);

#endif