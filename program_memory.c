/********************************************************************************
* program_memory.c: Contains function definitions and macro definitions for
*                   implementation of a 6 kB program memory, capable of storing
*                   up to 256 24-bit instructions. Since C doesn't support
*                   unsigned 24-bit integers (without using structs or unions),
*                   the program memory is set to 32 bits data width, but only
*                   24 bits are used.
********************************************************************************/
#include "program_memory.h"

/* Macro definitions: */
#define main      0x08 /* Start address for subroutine main. */
#define main_loop 0x0D /* Start address for loop in subroutine main. */
#define end       0x11 /* End address for current program. */

#define LED1 PORTB0 /* LED 1 connected to pin 8 (PORTB0). */
#define LED2 PORTB1 /* LED 2 connected to pin 9 (PORTB1). */
#define LED3 PORTB2 /* LED 3 connected to pin 10 (PORTB2). */

/********************************************************************************
* data: Program memory with capacity for storing 256 instructions at address
*       0 - 255. Currently the code is written directly as machine code.
*       Preferably an assembler should be used to convert from assembly
*       to machine code.
********************************************************************************/
static const uint32_t data[PROGRAM_MEMORY_ADDRESS_WIDTH] = 
{
   /********************************************************************************
   * RESET_vect: Reset vector and start address for the program. A jump is made
   *             to the main subroutine in order to start the program.
   ********************************************************************************/
   0x160800, /* 0x00 => JMP main */
   0x000000, /* 0x01 => NOP */
   0x000000, /* 0x02 => NOP */
   0x000000, /* 0x03 => NOP */
   0x000000, /* 0x04 => NOP */
   0x000000, /* 0x05 => NOP */
   0x000000, /* 0x06 => NOP */
   0x000000, /* 0x07 => NOP */

   /********************************************************************************
   * main: Initiates the system at start. The program is kept running as long
   *       as voltage is supplied. The leds connected to PORTB0 - PORTB2 are
   *       blinkning continuously. Values for enabling each LED is stored in
   *       CPU registers R16 - R18 for direct write to data register PORTB.
   ********************************************************************************/
   0x011007, /* 0x08 => LDI R16, (1 << LED1) | (1 << LED2) | (1 << LED3) */
   0x030010, /* 0x09 => OUT DDRB, R16 */
   0x011001, /* 0x0A => LDI R16, (1 << LED1) */
   0x011102, /* 0x0B => LDI R17, (1 << LED2) */
   0x011204, /* 0x0C => LDI R18, (1 << LED3) */

   /********************************************************************************
   * main_loop: Blinks the leds in a loop continuously.
   ********************************************************************************/
   0x030110, /* 0x0D => OUT PORTB, R16 */
   0x030111, /* 0x0E => OUT PORTB, R17 */
   0x030112, /* 0x0F => OUT PORTB, R18 */
   0x160D00, /* 0x10 => JMP main_loop */
};

/********************************************************************************
* program_memory_read: Returns the instruction at specified address. If an
*                      invalid address is specified (should be impossible as
*                      long as the program memory address width isn't increased)
*                      no operation (0x00) is returned.
*
*                      - address: Address to instruction in program memory.
********************************************************************************/
uint32_t program_memory_read(const uint8_t address)
{
   if (address < PROGRAM_MEMORY_ADDRESS_WIDTH)
   {
      return data[address];
   }
   else
   {
      return 0x00;
   }
}

/********************************************************************************
* program_memory_subroutine_name: Returns the name of the subroutine at
*                                 specified address.
*
*                                 - address: Address within the subroutine.
********************************************************************************/
const char* program_memory_subroutine_name(const uint8_t address)
{
   if (address >= RESET_vect && address < main) return "RESET_vect";
   else if (address >= main && address < end)   return "main";
   else                                         return "Unknown";
}