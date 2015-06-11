#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "utility.h"

#define INST_LGTH (32)
//reading start from right to left. begin with index 0
bool read_bit(int number, int index) { 
  return extract_bits(number, index, 1) == 1;
}

// start is the start bit from where we extract the bits
// length is how many bits we want to extract
// instruction is from where we want to extract
unsigned int extract_bits(int number, int start, int length) {       
  if (start > 31 || start < 0) {                           
    printf("%s\n", "take int error");                   
    printf("start = %d, length = %d\n", start, length);
    return 0;
  } else {
      number >>= start;
      int mask = (1 << length) - 1;
      return mask & number;     
  }
}

uint32_t set_bit(int32_t number, int bit) {
  return number |= (1 << bit);
}

uint32_t clear_bit(int32_t number, int bit) {
  return number &= ~(1 << bit);
}

//NULL function so to say
void do_nothing (uint32_t *args, struct machine_state *mach, struct pipeline *pip) {
  return;
}


uint32_t lsl(int reg, int shiftvalue, struct machine_state *mach) {
  if (shiftvalue != 0) {
    mach->shifter_carryout = read_bit(reg, INST_LGTH - shiftvalue);
    return reg << shiftvalue;
  } else {
    return reg;
  }
}

uint32_t lsr(int reg, int shiftvalue, struct machine_state *mach) {
  if (shiftvalue != 0) {
    mach->shifter_carryout = read_bit(reg, shiftvalue - 1);
    uint32_t res = reg >> shiftvalue;
    res = clear_bit(res,31);
    res = clear_bit(res,30);
    return res;
  } else {
    return reg;
  }
}

uint32_t asr(int reg, int shiftvalue, struct machine_state *mach) {
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

uint32_t ror(int reg, int shiftvalue, struct machine_state *mach) {
  if (shiftvalue != 0) {
    mach->shifter_carryout = read_bit(reg, shiftvalue-1);
    int firstYbits = reg << (32 - shiftvalue);
    int lastbits   = reg >> shiftvalue;
    return (firstYbits | lastbits);
  } else {
    uint32_t ret = reg;
    return ret;
  }   
}

void get_mem(struct machine_state *mach, uint32_t *args) {
 uint32_t res = 0;
  for(int i=3; i > 0; i--) {
    res += mach->memory[args[1] + i];
    res <<= 8;
  }
  res += mach->memory[args[1]];
  mach->registers[args[0]] = res;
}
void put_mem(struct machine_state *mach, uint32_t *args) {
  uint32_t word = mach->registers[args[0]];
  for(int i=3; i >= 0; i--) {
    mach->memory[args[1]+i] = extract_bits(word, 8*i, 8);
  }
}

