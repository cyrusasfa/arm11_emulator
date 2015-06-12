#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H
#include "machine.h"


#define INST_LGTH (32)

void process_args(uint32_t instr,struct pipeline *pip, 
												                          struct machine_state *mach);

void decode_data_proc(uint32_t instr, struct pipeline *pip, 
											                           	struct machine_state *mach);

uint32_t ror(int reg, int shiftvalue, struct machine_state *mach);

void and(uint32_t* args, struct machine_state *mach, struct pipeline *pip);

void eor(uint32_t* args, struct machine_state *mach, struct pipeline *pip);

void sub(uint32_t* args, struct machine_state *mach, struct pipeline *pip);

void rsb(uint32_t* args, struct machine_state *mach, struct pipeline *pip);

void add(uint32_t* args, struct machine_state *mach, struct pipeline *pip);

void tst(uint32_t* args, struct machine_state *mach, struct pipeline *pip);

void teq(uint32_t* args, struct machine_state *mach, struct pipeline *pip);

void cmp(uint32_t* args, struct machine_state *mach, struct pipeline *pip);

void orr(uint32_t* args, struct machine_state *mach, struct pipeline *pip);

void mov(uint32_t* args, struct machine_state *mach, struct pipeline *pip);

void set_flags_logic(struct machine_state *mach, uint32_t res);

void set_flags_arithmetic(struct machine_state *mach, uint32_t res);

#endif
