#include "include/op.h"
#include "include/reg.h"
#include "include/cpu.h"
#include "include/mem.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void init_regs() {
    memset(data_reg, 0, 32);

    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;

    DDRA = 0x00;
    DDRB = 0x00;
    DDRC = 0x00;
    DDRD = 0x00;

    SREG = 0x00;
}

void reset() {
    pc = 0;

    cur_op = 0;
}

// reads the hex file and writes it to the flash buffer
// returns -1 on failure, or the number of bytes written to flash otherwise
int copy_to_flash(FILE *file) {
    if(file == NULL) return -1;

    char *buff = malloc(1024);
    memset(buff, 0, 1024);

    // reset the flash buffer to 0
    memset(CHIP_FLASH, 0, 32768);

    int bytes_written = 0;

    while(fgets(buff, 1024, file) != NULL) {
        // all valid lines must start with an ASCII ':' character
        // buff[7] and buff[8] contain the record type, and we only care about 00 (data)
        if(buff[0] != ':' || buff[7] != '0' || buff[8] != '0') continue;

        // not sure if this needs to be 5, but I did anyways in case the null terminator would cause a buffer overrun
        char tmp[5];
        memset(tmp, 0, 5);
        // copy the bytes at buff[1] and buff[2] to the temp buffer
        // according to the intel hex format, this is the byte count for the the line
        strncpy(tmp, buff + 1, 2);
        short byte_cnt = (short) strtol(tmp, NULL, 16);

        // reset the temp buffer and find the offset for the entry
        // this starts at buff[3] and goes to buff[6]
        memset(tmp, 0, 5);
        strncpy(tmp, buff + 3, 4);
        int offset = (int) strtol(tmp, NULL, 16);

        for(int i = 0; i < byte_cnt; i++) {
            memset(tmp, 0, 5);
            // start at buff[9] as that is where the data entry starts
            // have to scale by 2, because two ascii characters = 1 byte of hex data
            strncpy(tmp, buff + (2 * i) + 9, 2);
            unsigned char data = (unsigned char) strtol(tmp, NULL, 16);
            CHIP_FLASH[offset + i] = data;
            bytes_written++;
        }
    }

    return bytes_written;
}

int main() {
    reset();
    init_regs();

    FILE *prog = fopen("test.hex", "r");

    int flash_len = copy_to_flash(prog);
    if(flash_len < 0) {
        printf("Error reading hex file to memory\n");
        return -1;
    }

    for(int flash_index = 0; flash_index < flash_len; flash_index += 2) {

        cur_op = CHIP_FLASH[flash_index + 1];
        cur_op <<= 8;
        cur_op += CHIP_FLASH[flash_index];

        printf("cur_op: %x\n", cur_op);

        unsigned char inst = parse_op(cur_op);
        // not all of these will be used with every instruction
        // destination register
        unsigned char rd;
        // second register, such as in the add instruction
        unsigned char rr;
        // constant value, such as in the ldi instruction
        unsigned char k;
        // this seems kinda bad but idk
        switch(inst) {
            case ADC:
                rd = (cur_op & 0x0100) >> 4;
                rd += (cur_op & 0x00F0) >> 4;
                rr = (cur_op & 0x0200) >> 5;
                rr += (cur_op & 0x000F);
                printf("rd: %d, rr: %d\n", rd, rr);
                adc(rd, rr);
                break;

            case ADD:
                // parse rd and rr from the word read from the file
                rd = (cur_op & 0x0100) >> 4;
                rd += (cur_op & 0x00F0) >> 4;
                rr = (cur_op & 0x0200) >> 5;
                rr += (cur_op & 0x000F);
                printf("rd: %d, rr: %d\n", rd, rr);
                add(rd, rr);
                break;

            // this adds a constant to one of the upper register pairs (24+25, 26+27, 28+29, 30+31)
            case ADIW:
                rd = (cur_op & 0x0030) >> 4;
                // multiply the value by two since it is in two register increments
                rd *= 2;
                // starting register is r24, so set that as 0
                rd += 24;
                k = (cur_op & 0x00C0) >> 2;
                k += (cur_op & 0x000F);
                adiw(rd, k);
                break;

            case AND:
                rd = (cur_op & 0x0100) >> 4;
                rd += (cur_op & 0x00F0) >> 4;
                rr = (cur_op & 0x0200) >> 5;
                rr += (cur_op & 0x000F);
                and(rd, rr);
                break;

            case ANDI:
                rd = (cur_op & 0x00F0) >> 4;
                rd += 16;
                k = (cur_op & 0x0F00) >> 4;
                k += (cur_op & 0x000F);
                andi(rd, k);
                break;

            case LDI:
                rd = (cur_op & 0x00F0) >> 4;
                // this operation only works on registers 16-31, so add 16 so that 0 is r16
                rd += 16;
                k = (cur_op & 0x0F00) >> 4;
                k += (cur_op & 0x000F);
                ldi(rd, k);
                break;

        }

        printf("r16: %d, r17: %d\n", data_reg[16], data_reg[17]);
        printf("SREG: %d\n", SREG);
        
    }

    return 0;
}