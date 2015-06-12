#ifndef UTILITY_H
#define UTILITY_H
#include "machine.h"
#include <stdint.h>
#include <stdbool.h>

#define INST_LGTH (32)
#define INSTRUCTION_LENGTH (32)
#define COND_LENGTH (4)
#define COND_END (31)
#define OPCODE_LENGTH (4)
#define OPCODE_END (24)
#define PC_DIFF (8)
#define NUM_MNEMONICS (23)


bool read_bit(int instruction, int index);

unsigned int extract_bits(int instruction, int start, int length);

uint32_t set_bit(int32_t number, int bit);

uint32_t clear_bit(int32_t number, int bit);

void do_nothing (uint32_t *args, struct machine_state *mach, struct pipeline *pip);
												                                 
uint32_t lsl(int reg, int shiftvalue, struct machine_state *mach);

uint32_t lsr(int reg, int shiftvalue, struct machine_state *mach);

uint32_t asr(int reg, int shiftvalue, struct machine_state *mach);

void get_mem(struct machine_state *mach, uint32_t *args);

void put_mem(struct machine_state *mach, uint32_t *args);

uint32_t set_field(uint32_t instr, int value, int end, int length);

void remove_spaces(char* source);

#endif
