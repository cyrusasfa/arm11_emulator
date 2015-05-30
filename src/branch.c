#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "utility.h"
#include "data_processing.h"
#include "machine.h"
#include "branch.h"


int32_t* get_arg(int offset) {
 return &offset; 
}

void branch(int32_t *arg, struct machine_state *mach) {
  mach->registers[15] += arg[0];
}

void decode_branch(struct machine_state *mach,struct pipeline *pip,
 uint32_t fetched) {
  int offset = extract_bits(fetched, 0, 24);
  offset = offset << 2;
  int32_t move = offset;
  
  pip->decoded_args = get_arg(move);
  pip->decoded = &branch; 
}
