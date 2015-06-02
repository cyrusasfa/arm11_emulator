#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "utility.h"
#include "data_processing.h"
#include "machine.h"
#include "multiply.h"

void get_args(struct pipeline *pip, int rd, int rn, int rs, int rm, struct machine_state *mach) {
  pip->decoded_args = malloc (4 * sizeof(uint32_t));
  *(pip->decoded_args) = mach->registers[rm];
  *(pip->decoded_args + 1) = mach->registers[rs];
  *(pip->decoded_args + 2) = mach->registers[rn];
  *(pip->decoded_args + 3) = rd; 
}

void multiply(uint32_t *args, struct machine_state *mach, struct pipeline *pip) {
  mach->registers[args[3]] = args[0] * args[1];  
}

void multiply_acc(uint32_t *args, struct machine_state *mach, struct pipeline *pip) {
  mach->registers[args[3]] = args[0] * args[1] + args[2];
}

void decode_multiply(uint32_t instr, struct pipeline *pip, 
                      struct machine_state *mach) {
  const bool bitA = read_bit(instr, 21);
  const bool bitS = read_bit(instr, 20);
  const int Rd     = extract_bits(instr, 16, 4);
  const int Rn     = extract_bits(instr, 12, 4);  
  const int Rs     = extract_bits(instr, 8, 4);
  const int Rm     = extract_bits(instr, 0, 4);
  
  void (*mult_ptr[2]) (uint32_t*, struct machine_state*, struct pipeline*);
  mult_ptr[0] = &multiply;
  mult_ptr[1] = &multiply_acc;

  pip->decoded = mult_ptr[bitA];
  get_args(pip, Rd, Rn, Rs, Rm, mach);
  
  //Setting the flags
  if (bitS == 1) {
    mach -> registers[16] = 
      changebit(mach -> registers[16], 31, read_bit(mach -> registers[Rd], 31));
    if (mach -> registers[Rd] == 0) {
      mach -> registers[16] = changebit(mach -> registers[16], 30, 1);
    }   
  }
}

//change the value of bit at index to x
int changebit(int num, int index, _Bool x) {
    return num ^= (-x ^ num) & (1 << index);
}
