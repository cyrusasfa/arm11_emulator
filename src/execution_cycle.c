#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "machine.h"
#include "execution_cycle.h"
#include "utility.h"
#include "data_processing.h"
#include "multiply.h"
#include "single_data_transfer.h"
#include "branch.h"


void fetch(uint32_t pc, struct pipeline *pipeline, struct machine_state *mach) {
  uint32_t instr = 0;
  for(int i = 3; i > 0; --i) {
    instr += mach->memory[pc + i];
    instr <<= 8;
  }

  instr                += mach->memory[pc];
  pipeline->fetched    = (uint32_t*) malloc(sizeof(uint32_t));
  *(pipeline->fetched) = instr;
}

void decode(int cycle, uint32_t *instr, struct pipeline *pipeline,
                                                  struct machine_state *mach) {
  if (!instr || cycle == 1) {
    return;
  }

  if (*instr == 0) {
    pipeline->halt = 1;
    return;
  }

  unsigned int cond_code = extract_bits(*instr, 28, 4);
  if (!(condition(cond_code, *mach))) {
    pipeline->decoded = &do_nothing;
    return;
  }

  if (read_bit(*instr, 27)) {
    decode_branch(*instr, pipeline, mach);
  } else if (read_bit(*instr,26)) {
    decode_data_trans(*instr, pipeline, mach);
  } else if (!(read_bit(*instr, 25)) && 
               read_bit(*instr, 4) && 
               read_bit(*instr, 7)) {
    decode_multiply(*instr, pipeline, mach);
  } else {
    decode_data_proc(*instr, pipeline, mach);
  }
}

bool equal(struct machine_state mach) {
  return read_bit(mach.registers[16], 30) == 1;
}

bool not_equal(struct machine_state mach) {
  return !equal(mach);
}

bool greater_equal(struct machine_state mach) {
  return (read_bit(mach.registers[16],31) == read_bit(mach.registers[16], 28));
}

bool less(struct machine_state mach) {
  return (read_bit(mach.registers[16],31) != read_bit(mach.registers[16], 28));
}

bool greater(struct machine_state mach) {
  return (not_equal(mach) && greater_equal(mach));
}

bool less_equal(struct machine_state mach) {
  return (equal(mach) && less(mach));
}

bool always(struct machine_state mach) {
  return true;
}

bool condition(unsigned int cond_code, struct machine_state mach) {
  bool (*check_cpsr[15])(struct machine_state);
 
  check_cpsr[0]  = &equal;
  check_cpsr[1]  = &not_equal;
  check_cpsr[10] = &greater_equal;
  check_cpsr[11] = &less;
  check_cpsr[12] = &greater;
  check_cpsr[13] = &less_equal;
  check_cpsr[14] = &always;
  
  return (*check_cpsr[cond_code])(mach);
}
