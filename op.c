#include "include/op.h"
#include <stdio.h>

unsigned char parse_op(unsigned short opcode) {

    // stores the first 4 bits of the operation
    unsigned char tmp = opcode >> 8;

    switch(tmp) {
        // rall of these are add w/o carry instruction
        case ADD_OP:
        case 0x0D:
        case 0x0E:
        case 0x0F:
            return ADD;

        // all of these are add w/ carry instructions        
        case ADC_OP:
        case 0x1D:
        case 0x1E:
        case 0x1F:
            return ADC;

        case ADIW_OP:
            return ADIW;

        case AND_OP:
        case 0x21:
        case 0x22:
        case 0x23:
            return AND;

        case ANDI_OP:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B:
        case 0x7C:
        case 0x7D:
        case 0x7E:
        case 0x7F:
            return ANDI;

        // all operations that begin with this are LDI operations
        // this seems very bad but I can't think of a better way
        case LDI_OP:
        case 0xE1:
        case 0xE2:
        case 0xE3:
        case 0xE4:
        case 0xE5:
        case 0xE6:
        case 0xE7:
        case 0xE8:
        case 0xE9:
        case 0xEA:
        case 0xEB:
        case 0xEC:
        case 0xED:
        case 0xEE:
        case 0xEF:
            return LDI;
        
    }
}

// same as add, but factors in the carry bit from SREG
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

// add two registers and store the result in rd
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

    // set the destination register to the sum
    data_reg[rd] = sum & 0x00FF;

    pc++;
}

// add a constant k onto one of the upper register pairs (r24-r31)
inline void adiw(unsigned char rd, unsigned char k) {
    unsigned int val = data_reg[rd + 1] << 8;
    val += data_reg[rd];

    val += k;

    // check if a two's complement overflow resulted from the addition
    if(!(data_reg[rd + 1] & 0x80) && (val & 0x8000)) SREG |= (1<<SREG_V);
    else SREG &= ~(1<<SREG_V);
    // check if the number is negative (in two's complement)
    if(data_reg[rd + 1] & 0x80) SREG |= (1<<SREG_N);
    else SREG &= ~(1<<SREG_N);
    // check if the result is signed
    if(((SREG & (1<<SREG_N)) << 1) ^ (SREG & (1<<SREG_V))) SREG |= (1<<SREG_S);
    else SREG &= ~(1<<SREG_S);
    // check if sum is zero
    if(val == 0) SREG |= (1<<SREG_Z);
    else SREG &= ~(1<<SREG_Z);
    // check if there is a carry in the MSB
    if(val > 65535) SREG |= (1<<SREG_C);
    else SREG &= ~(1<<SREG_C);

    data_reg[rd + 1] = (val & 0xFF00) >> 8;
    data_reg[rd] = val & 0x00FF;

    pc++;
}

// AND a register with another register
inline void and(unsigned char rd, unsigned char rr) {
    data_reg[rd] &= data_reg[rr];

    // Microchip's datasheet specifies that this should be cleared
    SREG &= ~(1<<SREG_V);
    // check if the number is negative under two's complement
    if(data_reg[rd] & 0x80) SREG |= (1<<SREG_N);
    else SREG &= ~(1<<SREG_N);
    // this bit should be set based on the outcome of the XOR between SREG_N and SREG_V
    // since SREG_V is always 0, its result is the same as SREG_N
    if(SREG & (1<<SREG_N)) SREG |= (1<<SREG_S);
    else SREG &= ~(1<<SREG_S);
    // set the zero flag if the result is zero
    if(data_reg[rd] == 0) SREG |= (1<<SREG_Z);
    else SREG &= ~(1<<SREG_Z);

    pc++;
}

// AND r16-r31 with a constant
inline void andi(unsigned char rd, unsigned char k) {
    data_reg[rd] &= k;

    // Microchip's datasheet specifies that this should be cleared
    SREG &= ~(1<<SREG_V);
    // check if the number is negative under two's complement
    if(data_reg[rd] & 0x80) SREG |= (1<<SREG_N);
    else SREG &= ~(1<<SREG_N);
    // this bit should be set based on the outcome of the XOR between SREG_N and SREG_V
    // since SREG_V is always 0, its result is the same as SREG_N
    if(SREG & (1<<SREG_N)) SREG |= (1<<SREG_S);
    else SREG &= ~(1<<SREG_S);
    // set the zero flag if the result is zero
    if(data_reg[rd] == 0) SREG |= (1<<SREG_Z);
    else SREG &= ~(1<<SREG_Z);

    pc++;
}

// load registers r16-r31 with a constant k
inline void ldi(unsigned char rd, unsigned char k) {
    data_reg[rd] = k;

    pc++;
}