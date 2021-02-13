#ifndef CPU_H
#define CPU_H

// the atmega328 has a 16 bit word for each operation
unsigned short cur_op;

// program counter
unsigned long pc;

void reset();

#endif