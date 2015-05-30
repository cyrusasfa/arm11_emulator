#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "utility.h"
#include "data_processing.h"
#include "machine.h"

#define INST_LGTH 32

void set_flags_logic(struct machine_state *mach, int32_t res) {
  if(mach->shifter_carryout) {     
    set_bit(mach->registers[16], 29);//Set C flag
  } else {
    clear_bit(mach->registers[16], 29);//Clear C flag
  }

  if(res == 0) {
    set_bit(mach->registers[16], 30);//Set Z flag
  } else {
    clear_bit(mach->registers[16], 30);//Set Z flag
  }

  if (read_bit(res, 31)) {
    set_bit(mach->registers[16], 31);//Set N flag
  } else {
    clear_bit(mach->registers[16], 31);//Clear N flag
  }
}

void set_flags_arithmetic(struct machine_state *mach, int32_t res) {
  if(mach->alu_carryout) {
    set_bit(mach->registers[16], 29);//Set C flag
  } else {
    clear_bit(mach->registers[16], 29);//Clear C flag
  }
  if(res == 0) {
    set_bit(mach->registers[16], 30);//Set Z flag
  } else {
    clear_bit(mach->registers[16], 30);//Set Z flag
  }

  if (read_bit(res, 31)) {
    set_bit(mach->registers[16], 31);//Set N flag
  } else {
    clear_bit(mach->registers[16], 31);//Clear N flag
  }
}

void and(int32_t* args, struct machine_state *mach) {
  int32_t res = args[0] & args[1];
  mach->registers[args[2]] = res;
  if(args[3] == 1) {
    set_flags_logic(mach, res);
  }
}

void eor(int32_t* args, struct machine_state *mach) {
  int32_t res = args[0] ^ args[1];
  mach->registers[args[2]] = res;
  if(args[3]) {
    set_flags_logic(mach, res);
  }
}

void sub(int32_t* args, struct machine_state *mach) {
  args[1] = -args[1];
  add(args, mach);
}

void rsb(int32_t* args, struct machine_state *mach) {
  int32_t tmp = args[1];
  args[1] = args[0];
  args[0] = tmp;
  sub(args, mach);
}

void add(int32_t* args, struct machine_state *mach) {
  int32_t res = args[0] + args[1];
  //Overflow check
  if(args[0] > 0 && args[1] > ((pow(2,31) - 1) - args[0])) {
    mach->alu_carryout = true; 
  }
  mach->registers[args[2]] = res;
  if(args[3] == 1) {
    set_flags_arithmetic(mach, res);
  }
}

void tst(int32_t* args, struct machine_state *mach) {
  int32_t res = args[0] & args[1];
  if (args[3] == 1) {
    set_flags_logic(mach, res);
  }
}

void teq(int32_t* args, struct machine_state *mach) {
  int32_t res = args[0] ^ args[1];
  if(args[3] ==1) {
    set_flags_logic(mach ,res);
  }
}

void cmp(int32_t* args, struct machine_state *mach) {
  int32_t res = args[0] - args[1];
  if (args[0] > 0 && (-args[1]) > ((pow(2,31) - 1) - args[1])){
    mach->alu_carryout = true;
  }
  if(args[3] == 1) {
    set_flags_arithmetic(mach, res);
  }
}

void orr(int32_t* args, struct machine_state *mach) {
  int32_t res = args[0] | args[1];
  mach->registers[args[2]] = res;
  if(args[3] == 1) {
    set_flags_logic(mach, res);
  }
}

void mov(int32_t* args, struct machine_state *mach) {
  mach->registers[args[2]] = args[0];
  int32_t res = mach->registers[args[2]];
  if(args[3] == 1) {
    set_flags_logic(mach, res);
  }
}

int lsl(int reg, int shiftvalue, struct machine_state *mach) {
  if (shiftvalue != 0) {
    mach->shifter_carryout = read_bit(reg, INST_LGTH - shiftvalue);
    return reg << shiftvalue;
  } else {
    return reg;
  }
}

int lsr(int reg, int shiftvalue, struct machine_state *mach) {
  if (shiftvalue != 0) {
    mach->shifter_carryout = read_bit(reg, shiftvalue - 1);
    return reg >> shiftvalue;
  } else {
    return reg;
  }
}

int asr(int reg, int shiftvalue, struct machine_state *mach) {
  if (shiftvalue != 0) {
    bool lastbit = read_bit(reg, INST_LGTH - 1);
    mach->shifter_carryout = read_bit(reg, shiftvalue - 1);
    int reglsr = lsr(reg, shiftvalue, mach);
    if (lastbit) {
      int mask = (1 << shiftvalue) - 1;
      return reglsr | mask;
    } else {
      return reglsr;
    }
  } else {
    return reg;
  }
}

int ror(int reg, int shiftvalue, struct machine_state *mach) {
  if (reg != 0) {
    mach->shifter_carryout = read_bit(reg, shiftvalue-1);
    int firstYbits = reg << (32 - shiftvalue);
    int lastbits   = reg >> shiftvalue;
    return (firstYbits | lastbits);
   } else {
    return reg;
   }   
}

int* process_args(int32_t instr, struct machine_state *mach) {
  int32_t rn = extract_bits(instr, 16, 4);
  int32_t rd = extract_bits(instr, 12, 4);
  int32_t o2 = extract_bits(instr,0, 12);
  bool imm = read_bit(instr, 25);

  int (*shift_ptr[4]) (int x, int y, struct machine_state *mach);
  shift_ptr[0] = lsl;
  shift_ptr[1] = lsr;
  shift_ptr[2] = asr;
  shift_ptr[3] = ror;

  if (imm) {
    o2 = ror(extract_bits(instr,0, 8), 2 * extract_bits(instr, 8, 4), mach);
  } else if (read_bit(instr,4)) {
    int reg = extract_bits(instr, 0, 4);
    int shift = extract_bits(instr, 5, 2);
    int sregv = extract_bits(mach->registers[extract_bits(instr, 8, 4)], 0, 8);
    o2 = (*shift_ptr[shift]) (mach->registers[reg], sregv, mach); 
  } else {
    int reg = extract_bits(instr, 0, 4);
    int shift = extract_bits(instr, 5, 2);
    uint8_t val = extract_bits(instr, 7, 5);
    o2 = (*shift_ptr[shift]) (mach->registers[reg], val, mach);
  }
  
  int32_t arr[4] = {rn, o2, rd, (int) read_bit(instr, 20)};
  int32_t *ret = arr;

  return ret;
}


void decode_data_proc(int32_t instr, struct pipeline *pip, struct machine_state
                        *mach) {
  void (*op_ptrs[14]) (int32_t* args, struct machine_state *mach);

  op_ptrs[0] = &and ;
  op_ptrs[1] = &eor;
  op_ptrs[2] = &sub;
  op_ptrs[3] = &rsb;
  op_ptrs[4] = &add;
  op_ptrs[8] = &tst;
  op_ptrs[9] = &teq;
  op_ptrs[10] = &cmp;
  op_ptrs[12] = &orr;
  op_ptrs[13] = &mov;

  pip->decoded_args = process_args(instr, mach);
  pip->decoded = op_ptrs[extract_bits(instr, 21, 4)];
}






