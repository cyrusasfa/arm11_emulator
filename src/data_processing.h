#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

int* process_args(int32_t instr, struct machine_state *mach);

void decode_data_proc(int32_t instr, struct pipeline *pip, struct machine_state 
                      *mach);

int32_t lsl(int32_t num, int shift_val);

int32_t lsr(int32_t num, int shift_val);

int32_t asr(int32_t num, int shift_val);

int32_t ror(int32_t num, int shift_val);

void and(int32_t* args, struct machine_state *mach);

void eor(int32_t* args, struct machine_state *mach);

void sub(int32_t* args, struct machine_state *mach);

void rsb(int32_t* args, struct machine_state *mach);

void add(int32_t* args, struct machine_state *mach);

void tst(int32_t* args, struct machine_state *mach);

void teq(int32_t* args, struct machine_state *mach);

void cmp(int32_t* args, struct machine_state *mach);

void orr(int32_t* args, struct machine_state *mach);

void mov(int32_t* args, struct machine_state *mach);

void set_flags_logic(struct machine_state *mach, int32_t res);

void set_flags_arithmetic(struct machine_state *mach, int32_t res);

#endif
