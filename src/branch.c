#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "utility.h"
#include "data_processing.h"
#include "machine.h"
#include "branch.h"



void get_arg(unsigned int offset, struct pipeline *pip) {
  pip->decoded_args = (uint32_t*) malloc (sizeof(uint32_t));
  *(pip->decoded_args) = offset;
}

void branch(uint32_t *arg, struct machine_state *mach, struct pipeline *pip) {
  mach->registers[15] += arg[0];
  //printf("Got to here branch function");
  pip->fetched = 0;
  pip->halt = 0;
  pip->decoded = &do_nothing;
  pip->decoded_args = 0;
  //printf("The branching worked fine, pipeline cleared");
}

void decode_branch(uint32_t instr, struct pipeline *pip, struct machine_state
                    *mach) {
  //printf("Got to decode branch");
  unsigned int offset = extract_bits(instr, 0, 24);
  offset <<= 2;
  uint32_t move = offset;
  
  get_arg(move, pip);
  pip->decoded = &branch; 
}
