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
  uint32_t *args = (uint32_t*) malloc (4 * sizeof(uint32_t));
  //printf("WEIRD ASS OFFSET : %i\n", *arg);
  *(args) = 15;
  *(args+1) =  *arg;
  *(args+2) = 15;
  *(args+3) = 0;
  add(args, mach, pip);
  //printf("THE PC IS %i\n", mach->registers[15]);
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
  uint32_t move = offset << 2;
  int sign_bit = read_bit(move, 25); // After left shift of 2 sign bit is 26
  //printf(" THE SIGN BIT : %i\n", sign_bit); 
  if (sign_bit) {
    for(int i = 26; i < 32; i++) {
      move = set_bit(move, i);
    }
  }

  get_arg(move, pip);
  pip->decoded = &branch; 
}
