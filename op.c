#include "include/op.h"
#include <stdio.h>

unsigned char parse_op(unsigned short opcode) {

    // stores the first 4 bits of the operation
    unsigned char tmp = (opcode & 0xF000) >> 8;

    switch(tmp) {
        // right now this is the add w/o carry instruction
        case 0x00:
            return ADD;

        // right now this is the add w/ carry instruction
        case 0x01:
            return ADC;

        // all operations that begin with this are LDI operations
        case 0xE0:
            return LDI;
        
    }
}

inline void adc(unsigned char rd, unsigned char rr) {
    unsigned short sum = data_reg[rd] + data_reg[rr] + (SREG & (1<<SREG_C));
    // check whether there was a carry in the msb
    if(sum > 255) SREG |= (1<<SREG_C);
    else SREG &= ~(1<<SREG_C);
    // check whether the msb of the result is set to see if it is signed
    if(sum & 0x0080) SREG |= (1<<SREG_N);
    else SREG &= ~(1<<SREG_N);
    // check whether the addition of two positive signed numbers has yielded a negative (two's complement overflow)
    if(!(data_reg[rd] & 0x80) && !(data_reg[rr] & 0x80) && (sum & 0x0080)) SREG |= (1<<SREG_V);
    else SREG &= ~(1<<SREG_V);
    // check whether bit 3 had a carry
    if(((data_reg[rd] & 0x0F) + (data_reg[rr] & 0x0F)) > 15) SREG |= (1<<SREG_H);
    else SREG &= ~(1<<SREG_H);
    // check whether the result was 0
    if(sum == 0) SREG |= (1<<SREG_Z);
    else SREG &= ~(1<<SREG_Z);
    // check whether the result was signed or not
    if(((SREG & (1<<SREG_N)) << 1) ^ (SREG & (1<<SREG_V))) SREG |= (1<<SREG_S);
    else SREG &= ~(1<<SREG_S);

    // set the destination register to the sum
    data_reg[rd] = sum & 0x00FF;

    pc++;
}

inline void add(unsigned char rd, unsigned char rr) {
    unsigned short sum = data_reg[rd] + data_reg[rr];
    // check whether there was a carry in the msb
    if(sum > 255) SREG |= (1<<SREG_C);
    else SREG &= ~(1<<SREG_C);
    // check whether the msb of the result is set to see if it is signed
    if(sum & 0x0080) SREG |= (1<<SREG_N);
    else SREG &= ~(1<<SREG_N);
    // check whether the addition of two positive signed numbers has yielded a negative (two's complement overflow)
    if(!(data_reg[rd] & 0x80) && !(data_reg[rr] & 0x80) && (sum & 0x0080)) SREG |= (1<<SREG_V);
    else SREG &= ~(1<<SREG_V);
    // check whether bit 3 had a carry
    if(((data_reg[rd] & 0x0F) + (data_reg[rr] & 0x0F)) > 15) SREG |= (1<<SREG_H);
    else SREG &= ~(1<<SREG_H);
    // check whether the result was 0
    if(sum == 0) SREG |= (1<<SREG_Z);
    else SREG &= ~(1<<SREG_Z);
    // check whether the result was signed or not
    if(((SREG & (1<<SREG_N)) << 1) ^ (SREG & (1<<SREG_V))) SREG |= (1<<SREG_S);
    else SREG &= ~(1<<SREG_S);

    // printf("half sum: %d\n", ((data_reg[rd] & 0x0F) + (data_reg[rr] & 0x0F)));
    // printf("lower 4 of rd: %d\n", (data_reg[rd] & 0x0F));
    // printf("lower 4 of rr: %d\n", (data_reg[rr] & 0x0F));

    // set the destination register to the sum
    data_reg[rd] = sum & 0x00FF;

    pc++;
}

inline void ldi(unsigned char rd, unsigned char k) {
    data_reg[rd] = k;

    pc++;
}