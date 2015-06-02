#ifndef UTILITY_H
#define UTILITY_H
#include "machine.h"

bool read_bit(int instruction, int index);

unsigned int extract_bits(int instruction, int start, int length);

uint32_t set_bit(int32_t number, int bit);

uint32_t clear_bit(int32_t number, int bit);

void do_nothing (uint32_t *args, struct machine_state *mach, struct pipeline *pip); 

#endif
