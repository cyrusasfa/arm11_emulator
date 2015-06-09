#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "utility.h"
#include "data_processing.h"

#define INST_LGTH (32)
void set_flags_logic(struct machine_state *mach, uint32_t res) {
  if(mach->shifter_carryout) {     
    mach->registers[16] = set_bit(mach->registers[16], 29);//Set C flag
  } else {
    mach->registers[16] = clear_bit(mach->registers[16], 29);//Clear C flag
  }

  if(res == 0) {
    mach->registers[16] = set_bit(mach->registers[16], 30);//Set Z flag
  } else {
    mach->registers[16] = clear_bit(mach->registers[16], 30);//Set Z flag
  }

  if (read_bit(res, 31)) {
    mach->registers[16] = set_bit(mach->registers[16], 31);//Set N flag
  } else {
    mach->registers[16] = clear_bit(mach->registers[16], 31);//Clear N flag
  }
}

void set_flags_arithmetic(struct machine_state *mach, uint32_t res) {
  //printf("RES is : %i\n", res);
  //printf("RES bit 31 = %i\n", read_bit(res, 31));
  if(mach->alu_carryout) {
    mach->registers[16] = set_bit(mach->registers[16], 29);//Set C flag
  } else {
    mach->registers[16] = clear_bit(mach->registers[16], 29);//Clear C flag
  }
  if(res == 0) {
    mach->registers[16] = set_bit(mach->registers[16], 30);//Set Z flag
    //printf("The Z flag : %i\n", read_bit(mach->registers[16], 30));
  } else {
    mach->registers[16] = clear_bit(mach->registers[16], 30);//Set Z flag
  }

  if (read_bit(res, 31)) {
    mach->registers[16] = set_bit(mach->registers[16], 31);//Set N flag
  } else {
    mach->registers[16] = clear_bit(mach->registers[16], 31);//Clear N flag
  }
  //printf("The CPSR : %i\n", mach->registers[16]);
}

void and(uint32_t* args, struct machine_state *mach, struct pipeline *pip) {
  // printf("ARG : 0 %i\n", args[0]);
  // printf("ARG : 1 %i\n", args[1]);
  // printf("ARG : 2 %i\n", args[2]);
  // printf("ARG : 3 %i\n", args[3]);
  uint32_t res = mach->registers[args[0]] & args[1];
  mach->registers[args[2]] = res;
  if(args[3] == 1) {
    set_flags_logic(mach, res);
  }
}

void eor(uint32_t* args, struct machine_state *mach, struct pipeline *pip) {
  uint32_t res = mach->registers[args[0]] ^ args[1];
  mach->registers[args[2]] = res;
  if(args[3]) {
    set_flags_logic(mach, res);
  }
}

void sub(uint32_t* args, struct machine_state *mach, struct pipeline *pip) {
  args[1] = ~args[1] + 1;
  add(args, mach, pip);
}

void rsb(uint32_t* args, struct machine_state *mach, struct pipeline *pip) {
  uint32_t tmp = args[1];
  uint32_t tmp0 = mach->registers[args[0]];
  args[1] = tmp0;
  mach->registers[args[0]] = tmp;
  sub(args, mach, pip);
  mach->registers[args[0]] = tmp0;
}

void add(uint32_t* args, struct machine_state *mach, struct pipeline *pip) {
  // printf("ARG : 0 %i\n", args[0]);
  // printf("ARG : 1 %i\n", args[1]);
  // printf("ARG : 2 %i\n", args[2]);
  // printf("ARG : 3 %i\n", args[3]);
  uint32_t res = mach->registers[args[0]] + args[1];
  //Overflow check
  if(mach->registers[args[0]] > 0 && args[1] > ((pow(2,31) - 1) - mach->registers[args[0]])) {
    mach->alu_carryout = true; 
  }
  mach->registers[args[2]] = res;
  if(args[3] == 1) {
    set_flags_arithmetic(mach, res);
  }
}

void tst(uint32_t* args, struct machine_state *mach, struct pipeline *pip) {
  uint32_t res = mach->registers[args[0]] & args[1];
  if (args[3] == 1) {
    set_flags_logic(mach, res);
  }
}

void teq(uint32_t* args, struct machine_state *mach, struct pipeline *pip) {
  uint32_t res = mach->registers[args[0]] ^ args[1];
  if(args[3] ==1) {
    set_flags_logic(mach ,res);
  }
}

void cmp(uint32_t* args, struct machine_state *mach, struct pipeline *pip) {
  //printf("ARGs[3] = %i\n", args[3]);
  uint32_t res = mach->registers[args[0]] - args[1];
  //printf("The RESULT : %i\n", res);
  //if (mach->registers[args[0]] > 0 && args[1] > ((pow(2,31) - 1) - args[1])){
  if(args[0] >= args[1]) {
    mach->alu_carryout = true;
  }
  if(args[3] == 1) {
    set_flags_arithmetic(mach, res);
  }
}

void orr(uint32_t* args, struct machine_state *mach, struct pipeline *pip) {
  uint32_t res = mach -> registers[args[0]] | args[1];
  mach->registers[args[2]] = res;
  if(args[3] == 1) {
    set_flags_logic(mach, res);
  }
}

void mov(uint32_t* args, struct machine_state *mach, struct pipeline *pip) {
  //printf("Got to the mov function to be executed\n");
  //printf("MOV : %i args[1]\n", args[1]);
  //printf("MOV : %i args[2]\n", args[2]);
  //printf("MOV : %i args[3]\n", args[3]);
  mach->registers[args[2]] = args[1];
  uint32_t res = mach->registers[args[2]];
  if(args[3] == 1) {
    set_flags_logic(mach, res);
  }
}


void process_args(uint32_t instr,struct pipeline *pip, struct machine_state
   *mach) {
  uint32_t rn = extract_bits(instr, 16, 4);
  uint32_t rd = extract_bits(instr, 12, 4);
  uint32_t o2 = extract_bits(instr,0, 12);
  bool imm = read_bit(instr, 25);
  uint32_t flagS = read_bit(instr, 20);

  uint32_t (*shift_ptr[4]) (int x, int y, struct machine_state *mach);
  shift_ptr[0] = lsl;
  shift_ptr[1] = lsr;
  shift_ptr[2] = asr;
  shift_ptr[3] = ror;

  if (imm) {
    //printf("%i before ror", extract_bits(instr, 0, 4));
    o2 = ror(extract_bits(instr,0, 8), 2 * extract_bits(instr, 8, 4), mach);
  } 
  if (!imm && read_bit(instr,4)) {
    int reg = extract_bits(instr, 0, 4);
    int shift = extract_bits(instr, 5, 2);
    int sregv = extract_bits(mach->registers[extract_bits(instr, 8, 4)], 0, 8);
    o2 = (*shift_ptr[shift]) (mach->registers[reg], sregv, mach); 
  } 
  if (!imm && !read_bit(instr, 4)) {
    //printf("Got to this branch \n");
    int reg = extract_bits(instr, 0, 4);
    int shift = extract_bits(instr, 5, 2);
    uint8_t val = extract_bits(instr, 7, 5);
    o2 = (*shift_ptr[shift]) (mach->registers[reg], val, mach);
  }

  pip->decoded_args = (uint32_t*) malloc(4* sizeof(int32_t));
  *(pip->decoded_args) = rn;
  *(pip->decoded_args + 1) = o2;
  *(pip->decoded_args + 2) = rd;
  *(pip->decoded_args + 3) = flagS;
  //printf("%i, dest register as about to be returned\n", pip->decoded_args[2]);
}


void decode_data_proc(uint32_t instr, struct pipeline *pip, struct machine_state
                        *mach) {
  void (*op_ptrs[14]) (uint32_t* args, struct machine_state *mach, struct pipeline*);

  op_ptrs[0]  = &and;
  op_ptrs[1]  = &eor;
  op_ptrs[2]  = &sub;
  op_ptrs[3]  = &rsb;
  op_ptrs[4]  = &add;
  op_ptrs[8]  = &tst;
  op_ptrs[9]  = &teq;
  op_ptrs[10] = &cmp;
  op_ptrs[12] = &orr;
  op_ptrs[13] = &mov;
  
  process_args(instr, pip, mach);
  pip->decoded = op_ptrs[extract_bits(instr, 21, 4)];
  //printf("%u passed dest reg\n", pip->decoded_args[2]);
}






