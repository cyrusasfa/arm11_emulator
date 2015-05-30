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
  const int bit_I = 25;
  const int bit_P = 24;
  const int bit_U = 23;
  const int bit_L = 20;
  const int rn_strt = 16;
  const int rd_strt = 12;
  const int offset_strt = 0;

  int rn = extract_bits(instruction, rn_strt, 4);
  int rd = extract_bits(instruction, rd_strt, 4);

  int offset;

  if (read_bit(instruction, bit_I)) { // I = 1 so offset is shifted reg
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
    printf("I = 0\n");
    unsigned int imm    = extract_bits(instruction, offset_strt, 8);
    unsigned int rotate = extract_bits(instruction, 8, 4);
    rotate              <<= 2;
    offset              = ror(imm, rotate); // rotate right
  }

  if (read_bit(instruction, bit_P)) {
    // offset added/subtracted before transfer
    if (read_bit(instruction, bit_U)) {
      // add to base reg
      offset += mach -> registers[rn];
      if (read_bit(instruction, bit_L)) {
        // load from memory
        mach -> registers[rd] = mach -> memory[offset];
      } else {
        // store in memory
        mach -> memory[offset] = mach -> registers[rd];
      }
    } else {
      // subtract from base reg
      offset -= mach -> registers[rn];
      if (read_bit(instruction, bit_L)) {
        // load from memory
        mach -> registers[rd] = mach -> memory[offset];
      } else {
        mach -> memory[offset] = mach -> registers[rd];
      }
    }

  } else {
    // offset added/subtracted after transfer
    if (read_bit(instruction, bit_U)) {
      // add to base reg
      if (read_bit(instruction, bit_L)) {
        // load from memory
        mach -> registers[rd] = mach -> memory[mach -> registers[rn]];
      } else {
        // store in memory
        mach -> memory[mach -> registers[rn]] = mach -> registers[rd];
      }
      mach -> registers[rn] += offset;
    } else {
      // subtract from base reg
      if (read_bit(instruction, bit_L)) {
        // load from memory
        mach -> registers[rd] = mach -> memory[mach -> registers[rn]];
      } else {
        mach -> memory[mach -> registers[rn]] = mach -> registers[rd];
      }
      mach -> registers[rn] -= offset;
    }    

  }
}