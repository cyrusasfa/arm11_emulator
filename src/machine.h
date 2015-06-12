#ifndef EMULATION_STRUCTS
#define EMULATION_STRUCTS
 
 
struct machine_state {
  uint8_t *memory; //Array to represent the memory
  uint32_t *registers; //Array to represents the registers
  bool alu_carryout; //variable to represent the ALU-carryout bit
  bool shifter_carryout; //variable to represent the carryout of the shifter
};

struct pipeline {
  void (*decoded) (uint32_t *args, struct machine_state *mach,
                                                         struct pipeline *pip);
  uint32_t *decoded_args;
  uint32_t *fetched; 
  unsigned int halt : 1;
};

#endif
