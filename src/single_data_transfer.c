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
#include "single_data_transfer.h"

uint32_t* get_address(uint32_t instr, struct pipeline *pip, struct machine_state *mach) {
  uint32_t *ret = (uint32_t*) malloc(2 * sizeof(int32_t));
  
  struct trans_instr instruction;
  
  instruction.immediate = read_bit(instr, 25);
  instruction.pre = read_bit(instr, 24);
  instruction.up = read_bit(instr, 23);
  instruction.load = read_bit(instr, 20);
  instruction.rn = extract_bits(instr, 16, 4);
  instruction.rd = extract_bits(instr, 12, 4);
  instruction.offset = extract_bits(instr, 0, 12);
 
  uint32_t address;
  //printf ("Unprocessed offset = %i", instruction.offset); 
  if (instruction.immediate) {
    instr = clear_bit(instr,25);
    process_args(instr, pip, mach);
    uint32_t processed_offset = *(pip->decoded_args + 1);
    address 
      = mach->registers[instruction.rn] - (1 - 2 * instruction.up) * 
        processed_offset;
    //printf("Address is : %i\n", address);
    //printf("OFFSET BE LIKE : %i\n",processed_offset);
  } else { 
    //printf("This branch");
    address = mach->registers[instruction.rn] - (1 - 2 * instruction.up) * 
              instruction.offset;
  }
  uint32_t arg2;
  if (instruction.pre ==1) {
    arg2 =  address; 
  } else {
    arg2 = mach->registers[instruction.rn]; 
    mach->registers[instruction.rn] = address; 
  }
  //Taking the pipeline into account;
  if (instruction.rn == 15) {
    arg2 += 4;
  }
  int32_t arr[2] = {instruction.rd, arg2};
  for (int i=0; i < 3; i++) {
    *(ret + i) = arr[i];
  }

  return ret;
}

void load_word(uint32_t* args, struct machine_state *mach, struct pipeline *pip) {
 if (args[1] > 64 *1024) {
    printf("Error: Out of bounds memory access at address %0#10x\n", args[1]);
    return;
 }
 uint32_t res = 0;
  for(int i=3; i > 0; i--) {
    res += mach->memory[args[1] + i];
    res <<= 8;
  }
  res += mach->memory[args[1]];
  mach->registers[args[0]] = res;
}

void store_word(uint32_t* args, struct machine_state *mach, struct pipeline *pip) {
  uint32_t word = mach->registers[args[0]];
  for(int i=3; i >= 0; i--) {
    mach->memory[args[1]+i] = extract_bits(word, 8*i, 8);
  }
}


void decode_data_trans(uint32_t instr, struct pipeline *pip, struct 
  machine_state *mach) {
  void (*load_ptr[2]) (uint32_t*, struct machine_state*, struct pipeline*);
  load_ptr[0] = &store_word;
  load_ptr[1] = &load_word;

  pip->decoded_args = get_address(instr, pip,  mach);
  pip->decoded = load_ptr[read_bit(instr, 20)];

}
