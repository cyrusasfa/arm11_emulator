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


void decode_multiply(struct machine_state *mach, uint32_t instruction) {
  const _Bool bitA = read_bit(instruction, 21);
  const _Bool bitS = read_bit(instruction, 20);
  const int Rd     = extract_bits(instruction, 16, 4);
  const int Rn     = extract_bits(instruction, 12, 4);  
  const int Rs     = extract_bits(instruction, 8, 4);
  const int Rm     = extract_bits(instruction, 0, 4);
  if (bitA == 1) {
    mach -> registers[Rd] = mach -> registers[Rm] * mach -> registers[Rs] +
                                                         mach -> registers[Rn];
  }
  else {
    mach -> registers[Rd] = mach -> registers[Rm] * mach -> registers[Rs];
  }
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