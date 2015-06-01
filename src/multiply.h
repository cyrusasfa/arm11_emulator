#ifndef MULTIPLY_H
#define MULTIPLY_H
#include "machine.h"

void get_args(struct pipeline *pip, int rd, int rn, int rs, int rm, struct machine_state *mach);

void multiply(uint32_t *args, struct machine_state *mach);

void multiply_acc(uint32_t *args, struct machine_state *mach);

void decode_multiply(uint32_t instr, struct pipeline *pip, 
                        struct machine_state *mach);

int changebit(int num, int index, bool x);

#endif
