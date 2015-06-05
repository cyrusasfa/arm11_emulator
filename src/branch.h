#ifndef BRANCH_H
#define BRANCH_H

uint32_t* get_arg(unsigned int offset);

void branch(uint32_t *arg, struct machine_state *mach);

void decode_branch (uint32_t instr, struct pipeline *pip, struct machine_state
                      *mach);

#endif
