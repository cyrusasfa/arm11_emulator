#ifndef SINGLEDATATRANSFER_H
#define SINGLEDATATRANSFER_H

struct trans_instr {
  unsigned int offset : 12;
  unsigned int rd : 4;
  unsigned int rn : 4;
  unsigned int load : 1;
  unsigned int up : 1;
  unsigned int pre : 1;
  unsigned int immediate : 1;
};

int32_t* get_address(int32_t instr, struct machine_state *mach);

void transfer (int32_t* args, struct machine_state *mach);

void decode_data_trans(int32_t instr, struct pipeline *pip, 
    struct machine_state *mach);

#endif
