#ifndef MEM_H
#define MEM_H

#define RAMSTART 0x0100
#define RAMEND 0x8FF

// the atmega328 has 2K of SRAM
unsigned char sram[2048];

// the atmega328 has 1K of EEPROM memory
unsigned char eeprom[1024];

// the atmega328 has 32K of on-chip flash
unsigned char chip_flash[32768];

// will initialize memory to its default state
void init_mem();

#endif