#ifndef UTILITY_H
#define UTILITY_H
#include "machine.h"

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
#endif
