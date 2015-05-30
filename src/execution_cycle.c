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

void halt (int32_t *args, struct machine_state *mach) {
  return;
}

void fetch(int32_t pc, struct pipeline *pipeline, struct machine_state mach) {
  *(pipeline->fetched) = 0;
  for(int i=3; i > 0; --i) {
    *(pipeline->fetched) += mach.memory[pc + i];
    *(pipeline->fetched) <<= 8;
  }
  *(pipeline->fetched) += mach.memory[pc];
}

void decode(int32_t *instr, struct pipeline *pipeline,
               struct machine_state *mach) {
  if (!instr) {
    return;
  }

  if (*instr == 0) {
      return;
  }

  int cond_code = extract_bits(*instr, 28,4);
  if (!(condition(cond_code, *mach))) {
    return;
  }

  if (read_bit(*instr, 27)) {
    decode_branch(*instr, pipeline, mach);
  } else if (read_bit(*instr,26)) {
    decode_data_trans(*instr, pipeline, mach);
  } else if (!(read_bit(*instr, 25)) && read_bit(*instr, 4) && read_bit(*instr,
            7)) {
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

bool condition(int cond_code, struct machine_state mach) {
  bool (*check_cpsr[15])(struct machine_state);
 
  check_cpsr[0] = &equal;
  check_cpsr[1] = &not_equal;
  check_cpsr[10] = &greater_equal;
  check_cpsr[11] = &less;
  check_cpsr[12] = &greater;
  check_cpsr[13] = &less_equal;
  check_cpsr[14] = &always;
  
  return (*check_cpsr[cond_code])(mach);
}
