#ifndef BRANCH_H
#define BRANCH_H

void get_arg(unsigned int offset, struct pipeline *pip);

void branch(uint32_t *arg, struct machine_state *mach, struct pipeline *pip);

void decode_branch (uint32_t instr, struct pipeline *pip, 
                                                struct machine_state *mach);

#endif
