/********************************************************************************
* control_unit.c: Contains static variables and function definitions for 
*                 implementation of an 8-bit control unit.
********************************************************************************/
#include "control_unit.h"

/* Static variables: */
static uint32_t ir;    /* Instruction register, stores next instruction to execute. */
static uint8_t pc;     /* Program counter, stores address to next instruction to fetch. */
static uint8_t mar;    /* Memory address register, stores address for current instruction. */
static uint8_t sr;     /* Status register, stores status bits INZVC. */

static uint8_t op_code; /* Stores OP-code, for example LDI, OUT, JMP etc. */
static uint8_t op1;     /* Stores first operand, most often a destination. */
static uint8_t op2;     /* Stores second operand, most often a value or read address. */

static uint8_t reg[CPU_REGISTER_ADDRESS_WIDTH]; /* CPU-registers R0 - R31. */
static enum cpu_state state;                    /* Stores current state. */

/* Temporary memories: */
static uint8_t data_memory[2000];        /* Contains I/O-addresses and stores static variables. */

/********************************************************************************
* control_unit_reset: Resets control unit registers and corresponding program.
********************************************************************************/
void control_unit_reset(void)
{
   ir = 0x00;
   pc = 0x00;
   mar = 0x00;
   sr = 0x00;

   op_code = 0x00;
   op1 = 0x00;
   op2 = 0x00;

   state = CPU_STATE_FETCH;

   for (uint8_t i = 0; i < CPU_REGISTER_ADDRESS_WIDTH; ++i)
   {
      reg[i] = 0x00;
   }

   for (uint16_t i = 0; i < 2000; ++i)
   {
      data_memory[i] = 0x00;
   }

   return;
}

/********************************************************************************
* control_unit_run_next_state: Runs next state in the CPU instruction cycle:
********************************************************************************/
void control_unit_run_next_state(void)
{
   switch (state)
   {
      case CPU_STATE_FETCH:
      {
         ir = program_memory_read(pc); /* Fetches next instruction. */
         mar = pc;                     /* Stores address of current instruction. */
         pc++;                         /* Program counter points to next instruction. */
         state = CPU_STATE_DECODE;     /* Decodes the instruction during next clock cycle. */
         break;
      }
      case CPU_STATE_DECODE:
      {        
         op_code = ir >> 16;           /* Bit 23 downto 16 consist of the OP code. */
         op1 = ir >> 8;                /* Bit 15 downto 8 consists of the first operand. */
         op2 = ir;                     /* Bit 7 downto 0 constist of the second operand. */
         state = CPU_STATE_EXECUTE;    /* Executes the instruction during next clock cycle. */
         break;
      }
      case CPU_STATE_EXECUTE:
      {
         switch (op_code)              /* Executes specified operation. */
         {
            case NOP:               
            {
               break;
            }
            case LDI:              
            {
               reg[op1] = op2;
               break;
            }
            case MOV: 
            {
               reg[op1] = reg[op2];
               break;
            }
            case OUT:
            {
               data_memory[op1] = reg[op2];
               break;
            }
            case IN: 
            {
               reg[op1] = data_memory[op2];
               break;
            }
            case JMP: 
            {
               pc = op1;
               break;
            }
            default:
            {
               control_unit_reset();
               break;
            }
         }
         state = CPU_STATE_FETCH;      /* Fetches next instruction during next clock cycle. */
         break;
      }
      default:                         /* System reset if error occurs. */
      {
         control_unit_reset();
         break;
      }
   }
   return;
}

/********************************************************************************
* control_unit_run_next_state: Runs next CPU instruction cycle, i.e. fetches
*                              a new instruction from program memory, decodes
*                              and executes it.
********************************************************************************/
void control_unit_run_next_instruction_cycle(void)
{
   do
   {
      control_unit_run_next_state();
   } while (state != CPU_STATE_EXECUTE);
   return;
}

/********************************************************************************
* control_unit_print: Prints information about the processor, for instance
*                     current subroutine, instruction, state, content in
*                     CPU-registers and I/O registers DDRB, PORTB and PINB.
********************************************************************************/
void control_unit_print(void)
{
   printf("--------------------------------------------------------------------------------\n");
   printf("Current subroutine:\t\t\t\t%s\n", program_memory_subroutine_name(mar));
   printf("Current instruction:\t\t\t\t%s\n", cpu_instruction_name(op_code));
   printf("Current state:\t\t\t\t\t%s\n", cpu_state_name(state));

   printf("Program counter:\t\t\t\t%hu\n", pc);

   printf("Instruction register:\t\t\t\t%s ", get_binary((ir >> 16) & 0xFF, 8));
   printf("%s ", get_binary((ir >> 8) & 0xFF, 8));
   printf("%s\n", get_binary(ir & 0xFF, 8));

   printf("Status register (INZVC):\t\t\t%s\n\n", get_binary(sr, 5));

   printf("Content in CPU register R16:\t\t\t%s\n", get_binary(reg[R16], 8));
   printf("Content in CPU register R24:\t\t\t%s\n\n", get_binary(reg[R24], 8));

   printf("Content in data direction register DDRB:\t%s\n", get_binary(data_memory[DDRB], 8));
   printf("Content in data register PORTB:\t\t\t%s\n", get_binary(data_memory[PORTB], 8));
   printf("Content in pin input register PINB:\t\t%s\n", get_binary(data_memory[PINB], 8));

   printf("--------------------------------------------------------------------------------\n\n");
   return;
}


