#ifndef EMULATION_STRUCTS
#define EMULATION_STRUCTS

#define NUM_REGS (17)
 
struct machine_state {
  int8_t *memory; //Array to represent the memory
  int32_t registers[NUM_REGS]; //Array to represents the registers
  bool alu_carryout; //variable to represent the ALU-carryout bit
  bool shifter_carryout; //variable to represent the carryout of the shifter
};

struct pipeline {
  void (*decoded) (int32_t *args, struct machine_state *mach);
  int32_t *decoded_args;
  int32_t *fetched; 
};

#endif
