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
#include "singledatatransfer.h"


void decode_data_trans(struct machine_state *mach, uint32_t instruction) {
  bool i = read_bit(instruction, 25); // immediate offset. set -> shifted reg
  bool p = read_bit(instruction, 24); // pre/post bit. set -> pre
  bool u = read_bit(instruction, 23); // up bit. set -> add
  bool l = read_bit(instruction, 20); // load/store bit. set -> load

  int rn_strt = 16;
  int rd_strt = 12;
  int offset_strt = 0;
  int rn = extract_bits(instruction, rn_strt, 4);
  int rd = extract_bits(instruction, rd_strt, 4);

  int offset;

  if (i) { // I = 1 so offset is shifted reg
    printf("I = 1\n");
    int rm         = extract_bits(instruction, offset_strt, 4);
    int shifttype  = extract_bits(instruction, 5, 2);//4 possible shift codes
    int shiftvalue = 0;
    if (read_bit(instruction, 4)) { // if bit 4 is 1
      int rs         = extract_bits(instruction, 8, 4);
      int bottombyte = extract_bits(mach -> registers[rs], 24, 8); // last 8 bits
      shiftvalue     = bottombyte;
    } else { // if bit 4 is 0
      shiftvalue = extract_bits(instruction, 7, 5);
    }
    switch (shifttype) {
      case (0) :
        offset = lsl(mach -> registers[rm], shiftvalue);
        break;
      case (1) :
        offset = lsr(mach -> registers[rm], shiftvalue);
        break;
      case (2) :
        offset = asr(mach -> registers[rm], shiftvalue);
        break;
      case (3) :
        offset = ror(mach -> registers[rm], shiftvalue);
        break;
      default :
        printf("Error in choosing shift type\n");
        return;
    }
    printf("offset = %d\n", offset);  
  } else { // I = 0, offset is immediate
    offset = extract_bits(instruction, offset_strt, 12);
  }

  if (!u) { // check whether we are adding/subtracting
    offset = -offset;
  }

  if (p) {
    // pre-indexing
    // Need case where PC is the base register Rn
    if (l) {
      // load 
      mach->registers[rd] = load_mem_word(mach->registers[rn] + offset);
    } else {
      // store
      store_mem_word(mach->registers[rd], mach -> registers[rn] + offset);
    }
  } else {
    // post-indexing
    if (rn == rm) { 
      printf("Error: Rn cannot be the same as Rm for post-indexing str/ldr\n");
    } else {
      if (l) {
        // load 
        mach->registers[rd] = load_mem_word(mach->registers[rn]);
      } else {
        // store
        store_mem_word(mach->registers[rd], mach->registers[rn]);
      }
      mach->registers[rn] += offset; 
    }
  }
}

// extracts 32-bit word from memory starting from addr start
uint32_t load_mem_word(uint32_t start) { 
  uint32_t word = 0;
  // memory is little endian but instruction is read normally
  for (int i = 0; i < 4; i++) {
    word = word | ((uint32_t)mach->memory[start + i] << (8*i));
  }
  return word;
}

// stores 32-bit word in memory starting from startAddr
void store_mem_word(uint32_t word, uint32_t start) { 
  for (int i = 0; i < 4; i++) {
    mach->memory[start+i] = extract_bits(word, 8*i, 8);
  }
}