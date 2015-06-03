#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "utility.h"
#include "data_processing.h"
#include "machine.h"
#include "branch.h"


uint32_t* get_arg(unsigned int offset) {
  uint32_t *ret = (uint32_t*) malloc (sizeof(uint32_t));
  ret = &offset;
  return ret; 
}

void branch(uint32_t *arg, struct machine_state *mach) {
  mach->registers[15] += arg[0];
}

void decode_branch(uint32_t instr, struct pipeline *pip, struct machine_state
                    *mach) {
  unsigned int offset = extract_bits(instr, 0, 24);
  offset = offset << 2;
  uint32_t move = offset;
  
  pip->decoded_args = get_arg(move);
  pip->decoded = &branch; 
}
