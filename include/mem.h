#ifndef MEM_H
#define MEM_H

// the atmega328 has 2K of SRAM
unsigned char SRAM[2048];

// the atmega328 has 1K of EEPROM memory
unsigned char EEPROM[1024];

// the atmega328 has 32K of on-chip flash
unsigned char CHIP_FLASH[32768];

// will initialize memory to its default state
void init_mem();

#endif