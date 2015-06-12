#ifndef UTILITY_H
#define UTILITY_H

#include <stdint.h>
#include <stdbool.h>

#define INSTRUCTION_LENGTH (32)
#define INSTRUCTION_LENGTH (32)
#define COND_LENGTH (4)
#define COND_END (31)
#define PC_DIFF 8

bool read_bit(int instruction, int index);

unsigned int extract_bits(int instruction, int start, int length);

uint32_t set_bit(int32_t number, int bit);

uint32_t clear_bit(int32_t number, int bit);

uint32_t set_field(uint32_t instr, int value, int end, int length);

void remove_spaces(char* source);
#endif
