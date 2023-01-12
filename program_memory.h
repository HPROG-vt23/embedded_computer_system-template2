/********************************************************************************
* program_memory.h: Contains function declarations and macro definitions
*                   for implementation of a 1 kB program memory, capable
*                   of storing up to 1024 24-bit instructions. 
*                   Since C doesn't support unsigned 24-bit integers (without
*                   using structs or unions), the program memory is set to
*                   32 bits.
********************************************************************************/
#ifndef PROGRAM_MEMORY_H_
#define PROGRAM_MEMORY_H_

/* Include directives: */
#include "cpu.h"

/* Macrodefinitions: */
#define PROGRAM_MEMORY_DATA_WIDTH    24   /* 24 bits per instruction. */
#define PROGRAM_MEMORY_ADDRESS_WIDTH 1024 /* Capacity for storage of 1024 instructions. */

/********************************************************************************
* program_memory_read: Returns the instruction at specified address. If an
*                      invalid address is specified (should be impossible as
*                      long as the program memory address width isn't increased)
*                      no operation (0x00) is returned.
*
*                      - address: Address to instruction in program memory.
********************************************************************************/
uint32_t program_memory_read(const uint8_t address);

/********************************************************************************
* program_memory_subroutine_name: Returns the name of the subroutine at
*                                 specified address.
*
*                                 - address: Address within the subroutine.
********************************************************************************/
const char* program_memory_subroutine_name(const uint8_t address);


#endif /* PROGRAM_MEMORY_H_ */