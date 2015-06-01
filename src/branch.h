#ifndef BRANCH_H
#define BRANCH_H

int32_t* get_arg(int offset);

void branch(int32_t *arg, struct machine_state *mach);

void decode_branch (int32_t instr, struct pipeline *pip, struct machine_state
                      *mach);

#endif
