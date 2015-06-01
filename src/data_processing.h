#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H
#include "machine.h"

uint32_t* process_args(uint32_t instr, struct machine_state *mach);

void decode_data_proc(uint32_t instr, struct pipeline *pip, struct machine_state 
                      *mach);

uint32_t lsl(int reg, int shiftvalue, struct machine_state *mach);

uint32_t lsr(int reg, int shiftvalue, struct machine_state *mach);

uint32_t asr(int reg, int shiftvalue, struct machine_state *mach);

uint32_t ror(int reg, int shiftvalue, struct machine_state *mach);

void and(uint32_t* args, struct machine_state *mach);

void eor(uint32_t* args, struct machine_state *mach);

void sub(uint32_t* args, struct machine_state *mach);

void rsb(uint32_t* args, struct machine_state *mach);

void add(uint32_t* args, struct machine_state *mach);

void tst(uint32_t* args, struct machine_state *mach);

void teq(uint32_t* args, struct machine_state *mach);

void cmp(uint32_t* args, struct machine_state *mach);

void orr(uint32_t* args, struct machine_state *mach);

void mov(uint32_t* args, struct machine_state *mach);

void set_flags_logic(struct machine_state *mach, uint32_t res);

void set_flags_arithmetic(struct machine_state *mach, uint32_t res);

#endif
